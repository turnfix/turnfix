#include "header/_global.h"
#include "header/settings.h"
#include "model/entity/event.h"
#include <math.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QVariant>

int _global::dbtyp = 0;

QStringList _global::fields = QStringList();

void _global::initFields() {

    QStringList f;
    f <<  "Veranstaltungsname" << "Veranstaltungsdatum" << "Veranstaltungsort"
            << "Name" << "Verein" << "Platz" << "Punkte"
            << "Wettkampfbezeichnung" << "Wettkampfbezeichnung mit Jahrgang"
            << "Turnkreis/-gau" << "Verband" << "Land" << "Ausdruck-Typ" << "Summe Platzziffern"
            << "Mannschaftsnamen" << "Wettkampfnummer";

    fields = f;
}

void _global::setDBTyp(int typ) {
    dbtyp = typ;
}

int _global::getDBTyp() {
    return dbtyp;
}

QStringList _global::getFields() {
    return fields;
}

QString _global::wkBez(Event *event, QString swknr) {
    QSqlQuery query;
    query.prepare("SELECT bol_ak_anzeigen, yer_von, yer_bis, dat_von FROM tfx_wettkaempfe INNER JOIN tfx_veranstaltungen USING (int_veranstaltungenid) WHERE int_veranstaltungenid=? AND var_nummer=? ORDER BY var_nummer LIMIT 1");
    query.bindValue(0, event->mainEvent()->id());
    query.bindValue(1,swknr);
    query.exec();
    query.next();
    QString jahr1;
    QString jahr2;
    if (!query.value(0).toBool()) {
        jahr1 = "Jg. " + query.value(1).toString();
        jahr2 = query.value(2).toString();
    } else {
        jahr1 = "AK " + QString().setNum(query.value(3).toString().left(4).toInt()-query.value(1).toInt());
        jahr2 = QString().setNum(query.value(3).toString().left(4).toInt()-query.value(2).toInt());
    }
    QString to;
    switch (query.value(2).toInt()) {
    case 1  : to = QString(" und älter"); break;
    case 2  : to = QString(" und jünger"); break;
    default : to = QString(" - " + jahr2);  break;
    }
    if (query.value(1).toString() == query.value(2).toString()) {
        to = "";
    }
    if (query.value(2).toInt() == 3) {
        jahr1 = "Jahrgangsoffen";
        to = "";
    }
    return " " + jahr1 + to;
}

void _global::updateRgDis(Event *event) {
    QSqlQuery query;
    query.prepare("SELECT var_riege FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? GROUP BY var_riege");
    query.bindValue(0, event->mainEvent()->id());
    query.bindValue(1, event->round());
    query.exec();
    QSqlQuery query2;
    query2.prepare("SELECT int_disziplinenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? AND var_riege=? GROUP BY int_disziplinenid ORDER BY int_disziplinenid");
    query2.bindValue(0, event->mainEvent()->id());
    query2.bindValue(1, event->round());
    query2.exec();
    QSqlQuery query3;
    query3.prepare("SELECT int_riegen_x_disziplinenid FROM tfx_riegen_x_disziplinen WHERE int_veranstaltungenid=? AND int_runde=? AND int_disziplinenid=? AND var_riege=?");
    query3.bindValue(0, event->mainEvent()->id());
    query3.bindValue(1, event->round());
    QSqlQuery query4;
    query4.prepare("INSERT INTO tfx_riegen_x_disziplinen (int_veranstaltungenid,int_disziplinenid,var_riege,int_runde,int_statusid,bol_erstes_geraet) VALUES (?,?,?,?,?,?)");
    query4.bindValue(0, event->mainEvent()->id());
    query4.bindValue(3, event->round());
    query4.bindValue(4, 1);
    query4.bindValue(5, false);
    while(query.next()) {
        query2.bindValue(2,query.value(0).toString());
        query2.exec();
        while (query2.next()) {
            query3.bindValue(2, query2.value(0).toInt());
            query3.bindValue(3, query.value(0).toString());
            query3.exec();
            if (_global::querySize(query3) == 0) {
                query4.bindValue(1, query2.value(0).toInt());
                query4.bindValue(2, query.value(0).toString());
                query4.exec();
            }
        }
    }
    QSqlQuery query5;
    query5.prepare("DELETE FROM tfx_riegen_x_disziplinen WHERE int_veranstaltungenid=? AND var_riege NOT IN (SELECT var_riege FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? GROUP BY var_riege) AND int_disziplinenid NOT IN (SELECT int_disziplinenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? AND var_riege=tfx_riegen_x_disziplinen.var_riege GROUP BY int_disziplinenid ORDER BY int_disziplinenid)");
    query5.bindValue(0, event->mainEvent()->id());
    query5.bindValue(1, event->mainEvent()->id());
    query5.bindValue(2, event->round());
    query5.bindValue(3, event->mainEvent()->id());
    query5.bindValue(4, event->round());
    query5.exec();
}

double _global::calcLeistung(QString val) {
    double leistung;
    if (val.contains(":")) {
        QStringList split = val.split(":");
        leistung = split.at(0).toDouble()*60 + split.at(1).toDouble();
    } else {
        leistung = val.toDouble();
    }
    return leistung;
}

QList<QVariant> _global::nameSplit(QString name) {
    QList<QVariant> names;
    if (name.contains(", ")) {
        names.append(name.split(", ").at(1));
        names.append(name.split(", ").at(0));
    } else {
        int space = name.indexOf(" ");
        names.append(name.left(space));
        names.append(name.right(name.length()-space-1));
    }
    return names;
}

QString _global::strLeistung(double lst, QString einheit, QString maske, int nk) {
    QString meldeleistung;
    if ((lst > 59.59 && einheit != "m") || maske == "00:00.00") {
        int minutes = (int)(lst / 60);
        double seconds = lst - (minutes*60);
        meldeleistung = QString("%1").arg(minutes,2,'f',0,'0') + QString(":") + QString("%1").arg(seconds,5,'f',nk,'0');
    } else {
        meldeleistung = QString("%1").arg(lst, maske.length(), 'f', nk, '0');
    }
    return meldeleistung;
}


QString _global::nameFormat() {
    switch (Settings::nameFormat) {
    case 0: return "tfx_teilnehmer.var_vorname || ' ' || tfx_teilnehmer.var_nachname";
    case 1: return "upper(tfx_teilnehmer.var_nachname) || ' ' || tfx_teilnehmer.var_vorname";
    case 2: return "tfx_teilnehmer.var_nachname || ', ' || tfx_teilnehmer.var_vorname";
    }
    return "tfx_teilnehmer.var_vorname || ' ' || tfx_teilnehmer.var_nachname";
}

QString _global::intListToString(QList<int> clubs) {
    if (clubs.size()==0) return "0";
    QString clubString;
    for (int i=0;i<clubs.size();i++) {
        clubString += QString::number(clubs.at(i));
        if (i<clubs.size()-1) clubString += ",";
    }
    return clubString;
}

QList<int> _global::splitColorArray(QString array) {
    QString brackets = array.left(array.length()-1).right(array.length()-2);
    QStringList colors = brackets.split(",");
    QList<int> colary;
    for (int i=0;i<colors.size();i++) {
        colary.append(colors.at(i).toInt());
    }
    return colary;
}

int _global::querySize(QSqlQuery query) {
    if (dbtyp == 1) {
        int i=0;
        query.exec();
        while (query.next()) {
            i++;
        }
        query.exec();
        return i;
    } else {
        return query.size();
    }
}

QString _global::substring(QString field, QString from) {
    if (_global::getDBTyp() == 0) {
        return "substring("+field+" from "+from+")";
    } else {
        return "substr("+field+","+from+")";
    }
}

QString _global::date(QString field, int length) {
    if (length == 2) {
        if (_global::getDBTyp() == 0) {
            return "to_char("+field+",'YY')";
        } else {
            return "substr(strftime('%Y', "+field+"),3,2)";
        }
    } else if (length == 10) {
        if (_global::getDBTyp() == 0) {
            return "to_char("+field+",'dd.mm.yyyy')";
        } else {
            return "strftime('%d.%m.%Y', "+field+")";
        }
    } else {
        if (_global::getDBTyp() == 0) {
            return "to_char("+field+",'YYYY')";
        } else {
            return "strftime('%Y', "+field+")";
        }
    }
}
