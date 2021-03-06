#include <QSqlQuery>
#include <QKeyEvent>
#include <QStyle>
#include <QSqlRecord>
#include "header/mdl_einteilen.h"
#include "../global/header/_global.h"

int QEinteilTableModel::rowCount(const QModelIndex &) const {
    return tabledata.size();
}

int QEinteilTableModel::columnCount(const QModelIndex &) const {
    return 6;
}

QVariant QEinteilTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "StNr."; break;
        case 1: return "Name"; break;
        case 2: return "Geb."; break;
        case 3: return "m/w"; break;
        case 4: return "Verein"; break;
        case 5: return "WK"; break;
        }
    } else {
        return section+1;
    }
    return QVariant();
}

QVariant QEinteilTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DisplayRole) {
        return tabledata.at(index.row()).at(index.column());
    }
    return QVariant();
}



Qt::ItemFlags QEinteilTableModel::flags(const QModelIndex &index) const  {
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
}

QStringList QEinteilTableModel::takeRow(int row) {
    QStringList lst = tabledata.at(row);
    beginRemoveRows(QModelIndex(),row,row);
    tabledata.removeAt(row);
    endRemoveRows();
    return lst;
}

void QEinteilTableModel::insertRow(QStringList data) {
    beginInsertRows(QModelIndex(),this->rowCount(),this->rowCount());
    tabledata.insert(this->rowCount(),data);
    QSqlQuery query2;
    query2.prepare("UPDATE tfx_wertungen SET var_riege=? WHERE int_wertungenid=?");
    query2.bindValue(0,riege);
    query2.bindValue(1,data.at(6));
    query2.exec();
    QSqlQuery query4;
    query4.prepare("SELECT int_mannschaftenid FROM tfx_wertungen WHERE int_wertungenid=?");
    query4.bindValue(0,data.at(6));
    query4.exec();
    query4.next();
    if (query4.value(0).toString() != "" && _global::querySize(query4) > 0) {
        QSqlQuery query3;
        query3.prepare("UPDATE tfx_mannschaften SET var_riege=? WHERE int_mannschaftenid=?");
        query3.bindValue(0, riege);
        query3.bindValue(1, query4.value(0).toInt());
        query3.exec();
        query3.prepare("UPDATE tfx_wertungen SET var_riege=? WHERE int_mannschaftenid=?");
        query3.bindValue(0, riege);
        query3.bindValue(1, query4.value(0).toInt());
        query3.exec();
    }
    endInsertRows();
}

void QEinteilTableModel::setTableData() {
    QString extra = " OR var_riege IS NULL";
    if (riege != "") extra ="";
    QSqlQuery query2;
    query2.prepare("SELECT tfx_wertungen.int_startnummer, CASE WHEN tfx_gruppen.int_gruppenid IS NULL THEN tfx_teilnehmer.var_vorname || ' ' || tfx_teilnehmer.var_nachname ELSE tfx_gruppen.var_name END || CASE WHEN bol_ak='true' THEN ' (AK)' ELSE '' END, "+_global::date("dat_geburtstag",4)+", CASE WHEN int_geschlecht=0 THEN 'w' ELSE CASE WHEN int_geschlecht=1 THEN 'm' ELSE 'Gruppe' END END, tfx_vereine.var_name, tfx_wettkaempfe.var_nummer, tfx_wertungen.int_wertungenid, tfx_wertungen.var_riege FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid WHERE int_veranstaltungenid=? AND int_runde=? AND (var_riege=?"+extra+") ORDER BY tfx_wettkaempfe.var_nummer, "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, tfx_teilnehmer.var_nachname, tfx_teilnehmer.var_vorname");
    query2.bindValue( 0, _global::checkHWK());
    query2.bindValue( 1, _global::getRunde());
    query2.bindValue( 2, riege);
    query2.exec();
    tabledata.clear();
    while (query2.next()) {
        QStringList lst;
        for (int i=0;i<query2.record().count()-1;i++) {
            lst << query2.value(i).toString();
        }
        tabledata.append(lst);
    }
    this->reset();
}

void QEinteilTableModel::setRiege(QString rg) {
    riege = rg;
    setTableData();
}

void QEinteilTableModel::updateRiege(QString rg) {
    riege = rg;
    for (int i=0;i<tabledata.size();i++) {
        QSqlQuery query2;
        query2.prepare("UPDATE tfx_wertungen SET var_riege=? WHERE int_wertungenid=?");
        query2.bindValue(0,riege);
        query2.bindValue(1,tabledata.at(i).at(6));
        query2.exec();
        QSqlQuery query4;
        query4.prepare("SELECT int_mannschaftenid FROM tfx_wertungen WHERE int_wertungenid=?");
        query4.bindValue(0,tabledata.at(i).at(6));
        query4.exec();
        query4.next();
        if (query4.value(0).toString() != "" && _global::querySize(query4) > 0) {
            QSqlQuery query3;
            query3.prepare("UPDATE tfx_mannschaften SET var_riege=? WHERE int_mannschaftenid=?");
            query3.bindValue(0, riege);
            query3.bindValue(1, query4.value(0).toInt());
            query3.exec();
            query3.prepare("UPDATE tfx_wertungen SET var_riege=? WHERE int_mannschaftenid=?");
            query3.bindValue(0, riege);
            query3.bindValue(1, query4.value(0).toInt());
            query3.exec();
        }
    }
}
