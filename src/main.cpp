#include <QApplication>
#include <QIcon>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <QTranslator>
#include "dialogs/others/header/dlg_login.h"
#include "windows/header/mainwindow.h"
#include <QDebug>

//NEW
#include "view/dialogs/logindialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QTranslator translator;
    translator.load("qt_de.qm", ":/translations/");
    app.installTranslator(&translator);
    if (!QFile("license.ini").exists()) {
        QMessageBox msg(QMessageBox::Warning,"Keine Lizenz!","Es liegt keine Lizenzdatei vor. Bitte beantragen sie eine kostenlose Lizenz über www.turnfix.de. TurnFix wird jetzt beendet.");
        msg.exec();
        return 0;
    } else {
        QSettings license("license.ini", QSettings::IniFormat);
        license.setIniCodec("ISO 8859-1");
        QString createhash = "TurnFix::Serial=" + license.value("License/Serial").toString() + "::Organisation=" + license.value("License/Organisation").toString() + "::Name=" + license.value("License/Name").toString() + "::Mail=" + license.value("License/Mail").toString() + "::Type=" + license.value("License/Type").toString() + "::TurnFix";
        QByteArray hash = QCryptographicHash::hash(createhash.toLatin1(),QCryptographicHash::Md5);
        if (hash.toHex() != license.value("License/Hash").toString()) {
            QMessageBox msg(QMessageBox::Warning,"Fehlerhafte Lizenz!","Die Lizenzdatei ist fehlerhaft. Bitte wenden sie sich über das Forum beim TurnFix-Team. TurnFix wird jetzt beendet.");
            msg.exec();
            return 0;
        }
    }


    /*Login_Dialog *dialog = new Login_Dialog();*/
    LoginDialog dialog;
    if (dialog.exec() != 1) {
        return 0;
    } else {
        MainWindow mw;
        mw.show();
        return app.exec();
    }
}

