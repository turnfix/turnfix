#include "app/logindialog.h"
#include "app/mainwindow.h"
#include "model/entitymanager.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("TurnFix");
    QCoreApplication::setApplicationName("TurnFix");

    QApplication app(argc, argv);
    QTranslator translator;
    translator.load("qt_de.qm", ":/translations/");
    app.installTranslator(&translator);

    EntityManager em;

    LoginDialog dialog(&em);
    if (dialog.exec() != 1) {
        return 0;
    } else {
        MainWindow mw;
        mw.show();
        return app.exec();
    }
}
