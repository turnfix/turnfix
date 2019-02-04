#include <QMessageBox>
#include <QProgressDialog>
#include <QSqlQuery>
#include <QSettings>
#include "header/dlg_mail.h"
#include "../select/header/dlg_select_club.h"
#include "../../global/header/_global.h"
#include "../../global/header/settings.h"
#include "../../misc/header/trd_progress.h"

Mail_Dialog::Mail_Dialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
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

void Mail_Dialog::selClubs() {
    Select_Club_Dialog *vn = new Select_Club_Dialog(this);
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

void Mail_Dialog::sendMails() {
    QList<bool> checked;
    for (int i=0;i<lst_attach->count();i++) {
        if (lst_attach->item(i)->checkState() == Qt::Checked) {
            checked.append(true);
        } else {
            checked.append(false);
        }
    }
    Progress_Thread *progress = new Progress_Thread();
    dlg = new QProgressDialog("PDF-Dateien werden erzeugt...",0,0,0,this);
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

void Mail_Dialog::updateDialogLabel(QString text) {
    dlg->setLabelText(text);
}

void Mail_Dialog::closeDialog() {
    dlg->close();
}

void Mail_Dialog::setDetailInfo(int di) {
    detailinfo = di;
}

void Mail_Dialog::message(QString typ, QString titel, QString text) {
    QMessageBox::Icon icon;
    if (typ == "Critical") {
        icon = QMessageBox::Critical;
    } else {
        icon = QMessageBox::Warning;
    }
    QMessageBox msg(icon,titel,text);
    msg.exec();
}

