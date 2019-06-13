#include <QApplication>
#include <QIcon>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <QTranslator>
#include "windows/header/mainwindow.h"
#include <QDebug>

//NEW
#include "view/dialogs/logindialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QTranslator translator;
    translator.load("qt_de.qm", ":/translations/");
    app.installTranslator(&translator);

    LoginDialog dialog;
    if (dialog.exec() != 1) {
        return 0;
    } else {
        MainWindow mw;
        mw.show();
        return app.exec();
    }
}

