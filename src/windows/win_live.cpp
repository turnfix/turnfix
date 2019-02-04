#include "_global.h"
#include <QSqlQuery>
#include <QSettings>
#include "win_live.h"

_global _gfkt;

LiveWindow::LiveWindow(int wnr) {
    wknr = wnr;
    setupUi(this);
    QSqlQuery query;
    query.prepare("SELECT int_runde, int_hauptwettkampf, bol_rundenwettkampf FROM tfx_veranstaltungen WHERE int_veranstaltungenid=? LIMIT 1");
    query.bindValue(0,wknr);
    query.exec();
    query.next();
    int hwk = 0;
    int runde = 1;
    if (query.value(2).toBool()) {
        runde = query.value(0).toInt();
        hwk = query.value(1).toInt();
    } else {
        runde = 1;
        hwk = 0;
    }
    QSettings settings("turnfix.ini", QSettings::IniFormat);
    settings.setIniCodec("ISO 8859-1");
    _gfkt.setWkVars(wknr, runde, hwk,settings.value("Database/Typ").toInt());
}
