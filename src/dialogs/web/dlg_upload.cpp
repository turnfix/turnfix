#include "header/dlg_upload.h"
#include "ui_dlg_upload.h"
#include "../../web/header/web_upload.h"
#include "../../global/header/settings.h"
#include "../../global/header/_global.h"
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

Upload_Dialog::Upload_Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Upload_Dialog) {
    ui->setupUi(this);
    connect(ui->bbx_done,SIGNAL(accepted()),this,SLOT(upload()));
    connect(ui->bbx_done,SIGNAL(rejected()),this,SLOT(close()));
}

Upload_Dialog::~Upload_Dialog() {
    delete ui;
}

void Upload_Dialog::upload() {
    if (Settings::toaPass.length() == 0 || Settings::toaUser.length() == 0) {
        QMessageBox::warning(this,"Fehlende Angaben","In den Einstellungen wurden nicht alle benötigten Daten eingegeben.");
        return;
    }
    ui->bbx_done->setEnabled(false);
    errorShown = false;
    TOA_Upload *upload = new TOA_Upload();
    ui->lbl_progress->setText("PDF-Dateien werden erzeugt");
    ui->pbar->setMaximum(0);
    ui->pbar->setMaximum(0);
    ui->pbar->setEnabled(true);
    connect(upload, SIGNAL(textChanged(QString)), this, SLOT(updateDialogLabel(QString)));
    connect(upload, SIGNAL(uploadFiles(QStringList,QString)), this, SLOT(uploadFiles(QStringList,QString)));
    connect(upload, SIGNAL(numberChanged(int,int)), this, SLOT(setStatus(int,int)));
    upload->setRndRes(ui->chk_rnd->isChecked());
    upload->setDetailInfo(ui->cmb_detail->currentIndex());
    upload->setHeadFootID(0);
    upload->setAWExtra(ui->chk_aw->isChecked());
    upload->start();
}

void Upload_Dialog::updateDialogLabel(QString text) {
    ui->lbl_progress->setText(text);
}

void Upload_Dialog::setStatus(int curr, int total) {
    ui->pbar->setMaximum(total);
    ui->pbar->setValue(curr);
}

void Upload_Dialog::uploadDone(QNetworkReply * reply) {
    ui->pbar->setValue(ui->pbar->value()+1);
    if (reply->readAll() == "Login Failed" && !errorShown) {
        errorShown = true;
        QMessageBox::critical(this,"Fehlerhafte Benutzerdaten","Die Login-Daten für die TurnFix-Website sind falsch. Bitte überprüfen Sie ihre Angaben.");
    }
    reply->close();
    if (ui->pbar->value() == ui->pbar->maximum()) {
        for (int i=0;i<fileList.size();i++) {
            QFile(fileList.at(i)).remove();
        }
        updateDialogLabel("Upload wurde abgeschlossen");
        ui->bbx_done->setEnabled(true);
    }
}

void Upload_Dialog::uploadFiles(QStringList files, QString xml) {
    fileList = files;
    setStatus(0,files.size());
    updateDialogLabel("Upload der Dateien...");

    QSqlQuery query;
    query.prepare("SELECT dat_von FROM tfx_veranstaltungen WHERE int_veranstaltungenid=?");
    query.bindValue(0,_global::getWkNr());
    query.exec();
    query.next();

    QString filename(query.value(0).toString() + "-" + Settings::toaUser + ".tres");
    if (ui->chk_rnd->isChecked()) {
        filename = "RND" + filename;
    }

    nw = new QNetworkAccessManager(this);
    connect(nw, SIGNAL(finished(QNetworkReply*)), this, SLOT(uploadDone(QNetworkReply*)));

    for (int i=0;i<files.size();i++) {
        QString crlf="\r\n";
        QString boundary="---------------------------193971182219750";
        QString endBoundary=crlf+"--"+boundary+"--"+crlf;
        QString contentType="multipart/form-data; boundary="+boundary;
        boundary="--"+boundary+crlf;
        QByteArray bond=boundary.toAscii();

        QByteArray send;
        send.append(bond);
        boundary=crlf+boundary;
        bond=boundary.toAscii();
        send.append(QString("Content-Disposition: form-data; name=\"filename\""+crlf));
        send.append(crlf.toAscii());
        if (i==0) {
            send.append(filename.toAscii());
        } else {
            send.append(files.at(i).toAscii());
        }
        send.append(bond);
        send.append(QString("Content-Disposition: form-data; name=\"uname\""+crlf));
        send.append(crlf.toAscii());
        send.append(Settings::toaUser);
        send.append(bond);
        send.append(QString("Content-Disposition: form-data; name=\"pass\""+crlf));
        send.append(crlf.toAscii());
        send.append(Settings::toaPass);
        send.append(bond);
        send.append(QString("Content-Disposition: form-data; name=\"upload\""+crlf));
        send.append(crlf.toAscii());
        send.append("Uploader");
        send.append(bond);
        send.append(QString("Content-Disposition: form-data; name=\"dir\""+crlf));
        send.append(crlf.toAscii());
        send.append(filename.left(filename.length()-5));
        send.append(bond);
        if (i>0) {
            send.append(QString("Content-Disposition: form-data; name=\"tres\"; filename=\""+files.at(i).toAscii()+"\""+crlf));
        } else {
            send.append(QString("Content-Disposition: form-data; name=\"tres\"; filename=\""+filename+"\""+crlf));
        }
        send.append(QString("Content-Type: application/octet-stream"+crlf+crlf).toAscii());
        if (i>0) {
            QFile f(files.at(i));
            f.open(QIODevice::ReadOnly);
            send.append(f.readAll());
            f.close();
        } else {
            send.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" + xml.toUtf8());
        }
        send.append(endBoundary.toAscii());
        QNetworkRequest req;
        req.setUrl(QUrl("http://ergebnisse.turnfix.de/upload.php"));
        req.setHeader(QNetworkRequest::ContentTypeHeader, contentType.toAscii());
        req.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(send.size()).toString());
        nw->post(req,send);
    }
}
