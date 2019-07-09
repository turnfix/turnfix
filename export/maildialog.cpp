#include "maildialog.h"
#include "mailthread.h"
#include "model/objects/event.h"
#include "selectclubdialog.h"
#include "src/global/header/_global.h"
#include "src/global/header/settings.h"
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>
#include <QSqlQuery>

MailDialog::MailDialog(Event *event, QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    this->event = event;

    if (Settings::smtpFrom.length() == 0 ||
        Settings::smtpMail.length() == 0 ||
        Settings::smtpPass.length() == 0 ||
        Settings::smtpServer.length() == 0 ||
        Settings::smtpUser.length() == 0) {
        QMessageBox::warning(this,"Fehlende Angaben","In den Einstellungen wurden nicht alle benötigten Daten eingetragen. Bevor sie Emails versenden können, müssen diese Daten gepflegt werden.");
        close();
    }
    txt_from->setText(Settings::smtpFrom + " <" + Settings::smtpMail + ">");
    connect(but_to, SIGNAL(clicked()), this, SLOT(selClubs()));
    connect(but_send, SIGNAL(clicked()), this, SLOT(sendMails()));
}

void MailDialog::selClubs() {
    SelectClubDialog *vn = new SelectClubDialog(this->event, this);
    vn->exec();
    vereine = vn->returnVereine();
    QString to;
    for (int i=0;i<vereine.size();i++) {
        QSqlQuery query;
        query.prepare("SELECT var_vorname || ' ' || var_nachname, tfx_vereine.var_name, var_email FROM tfx_vereine INNER JOIN tfx_personen USING (int_personenid) WHERE int_vereineid=?");
        query.bindValue(0,vereine.at(i));
        query.exec();
        query.next();
        to = to + query.value(0).toString() + " (" + query.value(1).toString() + ") <" + query.value(2).toString() + ">\n";
    }
    txt_to->setPlainText(to);
}

void MailDialog::sendMails() {
    QList<bool> checked;
    for (int i=0;i<lst_attach->count();i++) {
        if (lst_attach->item(i)->checkState() == Qt::Checked) {
            checked.append(true);
        } else {
            checked.append(false);
        }
    }
    MailThread *progress = new MailThread(this->event);
    dlg = new QProgressDialog("PDF-Dateien werden erzeugt...", nullptr, 0, 0, this);
    dlg->setLabelText("PDF-Dateien werden erzeugt...");
    QObject::connect(progress, SIGNAL(textChanged(QString)), this, SLOT(updateDialogLabel(QString)));
    QObject::connect(progress, SIGNAL(finished()), this, SLOT(closeDialog()));
    QObject::connect(progress, SIGNAL(message(QString,QString,QString)), this, SLOT(message(QString,QString,QString)));
    dlg->show();
    progress->setChecked(checked);
    progress->setVereine(vereine);
    progress->setDetailinfo(detailinfo);
    progress->setMailVars(txt_subject->text(),txt_text->toPlainText());
    progress->start();
}

void MailDialog::updateDialogLabel(QString text) {
    dlg->setLabelText(text);
}

void MailDialog::closeDialog() {
    dlg->close();
}

void MailDialog::setDetailInfo(int di) {
    detailinfo = di;
}

void MailDialog::message(QString typ, QString titel, QString text) {
    QMessageBox::Icon icon;
    if (typ == "Critical") {
        icon = QMessageBox::Critical;
    } else {
        icon = QMessageBox::Warning;
    }
    QMessageBox msg(icon,titel,text);
    msg.exec();
}

