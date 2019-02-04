#include <QApplication>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <QDate>
#include "dialogs/others/header/dlg_tool_login.h"
#include "windows/header/win_eingabe.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    if (!QFile("license_eingabe.ini").exists()) {
        QMessageBox msg(QMessageBox::Warning,"Keine Lizenz!","Es liegt keine Lizenzdatei vor. Bitte beantragen sie eine kostenlose Lizenz über www.turnfix.de. TurnFix wird jetzt beendet.");
        msg.exec();
        return 0;
    } else {
        QSettings license("license_eingabe.ini", QSettings::IniFormat);
        license.setIniCodec("ISO 8859-1");
        QString createhash = "TurnFix::Serial=" + license.value("License/Serial").toString() + "::Organisation=" + license.value("License/Organisation").toString() + "::Name=" + license.value("License/Name").toString() + "::Mail=" + license.value("License/Mail").toString() + "::Expire=" + license.value("License/Expire").toString() + "::TurnFix";
        QByteArray hash = QCryptographicHash::hash(createhash.toLatin1(),QCryptographicHash::Md5);
        if (hash.toHex() != license.value("License/Hash").toString()) {
            QMessageBox msg(QMessageBox::Warning,"Fehlerhafte Lizenz!","Die Lizenzdatei ist fehlerhaft. Bitte wenden sie sich über das Forum beim TurnFix-Team. TurnFix wird jetzt beendet.");
            msg.exec();
            return 0;
        } else {
            if (license.value("License/Expire").toString() != "nie") {
                QDate expire = QDate::fromString(license.value("License/Expire").toString(),"yyyy-MM-dd");
                QDate today = QDate::currentDate();
                if (expire.daysTo(today)>0) {
                    QMessageBox msg(QMessageBox::Information,"Demoversion","Die Demoversion ist abgelaufen.");
                    msg.exec();
                    return 0;
                } else {
                    QMessageBox msg(QMessageBox::Information,"Demoversion","Die Demoversion ist noch "+QString::number(-expire.daysTo(today))+" Tage gültig.");
                    msg.exec();
                }
            }
        }
    }
    Tool_Login_Dialog *dialog = new Tool_Login_Dialog();
    if (dialog->exec() != 1) {
        return 0;
    } else {
        Eingabe_Tool st;
        st.show();
        return app.exec();
    }
}

