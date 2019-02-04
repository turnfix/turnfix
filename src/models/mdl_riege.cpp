#include <QSqlQuery>
#include <QSqlRecord>
#include <QKeyEvent>
#include <math.h>
#include "header/mdl_riege.h"
#include "../global/header/_global.h"
#include "../libs/fparser/fparser.hh"

#include <QDebug>

QRiegenTableModel::QRiegenTableModel(QObject *parent) : QAbstractTableModel(parent) {
    geraet=-1;
}

int QRiegenTableModel::rowCount(const QModelIndex &) const {
    return starter.size()*versuche;
}

int QRiegenTableModel::columnCount(const QModelIndex &) const {
    return 5+extraColumns.size();
}

QVariant QRiegenTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (index.row() >= starter.size()*versuche)
        return QVariant();
    if (starter.size() > 0) {
        int row = floor(index.row()/versuche);
        if (role == Qt::DisplayRole) {
            if (index.column() < 4) {
                return starter.at(row).at(index.column());
            } else {
                double wert=0;
                if (index.column() == this->columnCount()-1) {
                    wert = endwerte.value(starter.at(row).at(4).toInt()).value((index.row()%versuche)+1);
                } else {
                    wert = detailwerte.value(starter.at(row).at(4).toInt()).value((index.row()%versuche)+1).value(extraColumns.at(index.column()-4));
                }
                return _global::strLeistung(wert,disinfo.value(0).toString(),disinfo.value(1).toString(),disinfo.value(2).toInt());
            }
        } else if (role == Qt::BackgroundColorRole && index.column() == columnCount()-1) {
            QSqlQuery check;
            check.prepare("SELECT rel_max, var_formel FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_disziplinen ON tfx_wettkaempfe_x_disziplinen.int_disziplinenid = tfx_disziplinen.int_disziplinenid WHERE tfx_disziplinen.int_disziplinenid=? AND int_veranstaltungenid=? AND var_nummer=?");
            check.bindValue(0,geraet);
            check.bindValue(1,_global::checkHWK());
            check.bindValue(2,starter.at(row).at(3));
            check.exec();
            check.next();
            FunctionParser fparser;
            fparser.Parse(check.value(1).toString().replace(",",".").toStdString(),"x");
            double Vars[] = {endwerte.value(starter.at(row).at(4).toInt()).value((index.row()%versuche)+1)};
            if (fparser.Eval(Vars)>check.value(0).toDouble() && check.value(0).toDouble()>0) {
                return Qt::red;
            }
        }
    }
    return QVariant();
}

QVariant QRiegenTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "StNr."; break;
        case 1: return "Name"; break;
        case 2: return "Verein"; break;
        case 3: return "WK"; break;
        }
        if (geraet>0 && section == this->columnCount()-1) {
            if (disinfo.isValid()) return disinfo.value(3).toString();
        } else if (geraet>0) {
            return extraColumnNames.at(section-4);
        }
    }
    return QVariant();
}

Qt::ItemFlags QRiegenTableModel::flags(const QModelIndex &index) const  {
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    if (index.column() > 3) {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool QRiegenTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        int row = floor(index.row()/versuche);
        int selectedPEItem = starter.at(row).at(4).toInt();
        int v=(index.row()%versuche)+1;
        int kp=0;
        if (kuer) kp = 1;
        QSqlQuery disziplin;
        disziplin.prepare("SELECT var_maske, int_versuche FROM tfx_disziplinen WHERE int_disziplinenid=? LIMIT 1");
        disziplin.bindValue(0,geraet);
        disziplin.exec();
        disziplin.next();
        double leistung = _global::calcLeistung(value.toString());
        if (index.column() == this->columnCount()-1) {
            if (disziplin.value(0).toString() == value || leistung == 0) {
                QSqlQuery query6;
                query6.prepare("DELETE FROM tfx_wertungen_details WHERE int_wertungenid=? AND int_disziplinenid=? AND int_versuch=? AND int_kp=?");
                query6.bindValue(0,selectedPEItem);
                query6.bindValue(1,geraet);
                query6.bindValue(2,v);
                query6.bindValue(3,kp);
                query6.exec();
                query6.prepare("DELETE FROM tfx_jury_results WHERE int_wertungenid=? AND int_disziplinen_felderid IN (SELECT int_disziplinen_felderid FROM tfx_disziplinen_felder WHERE int_disziplinenid=? AND bol_enabled='true') AND int_versuch=? AND int_kp=?");
                query6.bindValue(0,selectedPEItem);
                query6.bindValue(1,geraet);
                query6.bindValue(2,v);
                query6.bindValue(3,kp);
                query6.exec();
            } else {
                QSqlQuery check;
                check.prepare("SELECT int_wertungen_detailsid FROM tfx_wertungen_details WHERE int_wertungenid=? AND int_disziplinenid=? AND int_versuch=? AND int_kp=? LIMIT 1");
                check.bindValue(0,selectedPEItem);
                check.bindValue(1,geraet);
                check.bindValue(2,v);
                check.bindValue(3,kp);
                check.exec();
                QSqlQuery query6;
                if (_global::querySize(check)==0) {
                    query6.prepare("INSERT INTO tfx_wertungen_details (int_wertungenid,int_disziplinenid,int_versuch,rel_leistung,int_kp) VALUES (?,?,?,?,?)");
                } else {
                    check.next();
                    query6.prepare("UPDATE tfx_wertungen_details SET int_wertungenid=?,int_disziplinenid=?,int_versuch=?,rel_leistung=?, int_kp=? WHERE int_wertungen_detailsid=?");
                    query6.bindValue(5,check.value(0).toString());
                }
                query6.bindValue(0,selectedPEItem);
                query6.bindValue(1,geraet);
                query6.bindValue(2,v);
                query6.bindValue(3,leistung);
                query6.bindValue(4,kp);
                query6.exec();
            }
            endwerte[selectedPEItem][v] = leistung;
        } else {
            QSqlQuery query;
            query.prepare("SELECT int_juryresultsid FROM tfx_jury_results WHERE int_wertungenid=? AND int_disziplinen_felderid=? AND int_versuch=? AND int_kp=?");
            query.bindValue(0, selectedPEItem);
            query.bindValue(1, extraColumns.at(index.column()-4));
            query.bindValue(2, v);
            query.bindValue(3,kp);
            query.exec();
            if (leistung > 0) {
                QSqlQuery query2;
                if (_global::querySize(query) == 0) {
                    query2.prepare("INSERT INTO tfx_jury_results (int_wertungenid,int_disziplinen_felderid,int_versuch,rel_leistung,int_kp) VALUES (?,?,?,?,?)");
                    query2.bindValue(0,selectedPEItem);
                    query2.bindValue(1,extraColumns.at(index.column()-4));
                    query2.bindValue(2,v);
                    query2.bindValue(3,leistung);
                    query2.bindValue(4,kp);
                } else {
                    query2.prepare("UPDATE tfx_jury_results SET rel_leistung=? WHERE int_juryresultsid=?");
                    query.next();
                    query2.bindValue(0, leistung);
                    query2.bindValue(1, query.value(0).toInt());
                }
                query2.exec();
            } else {
                if (_global::querySize(query) > 0) {
                    query.next();
                    QSqlQuery query4;
                    query4.prepare("DELETE FROM tfx_jury_results WHERE int_juryresultsid=?");
                    query4.bindValue(0, query.value(0).toInt());
                    query4.exec();
                }
            }
            detailwerte[selectedPEItem][v][extraColumns.at(index.column()-4)] = leistung;
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void QRiegenTableModel::setTableData(QString rg, int g, int v, bool k, bool j) {
    riege = rg;
    geraet = g;
    kuer = k;
    versuche = v;

    QSqlQuery query4;
    query4.prepare("SELECT tfx_wertungen.int_startnummer, CASE WHEN tfx_gruppen.int_gruppenid IS NULL THEN var_vorname || ' ' || var_nachname ELSE tfx_gruppen.var_name END, tfx_vereine.var_name, tfx_wettkaempfe.var_nummer, tfx_wertungen.int_wertungenid, tfx_wertungen.int_wettkaempfeid, int_pos FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid LEFT JOIN tfx_mannschaften ON tfx_mannschaften.int_mannschaftenid = tfx_wertungen.int_mannschaftenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid LEFT JOIN tfx_startreihenfolge ON tfx_startreihenfolge.int_wertungenid=tfx_wertungen.int_wertungenid AND tfx_startreihenfolge.int_disziplinenid=? AND tfx_startreihenfolge.int_kp=? WHERE int_veranstaltungenid=? AND tfx_wertungen.var_riege=? AND int_runde=? AND bol_startet_nicht='false' AND ((SELECT COUNT(*) FROM tfx_wettkaempfe_x_disziplinen WHERE int_disziplinenid=? AND int_wettkaempfeid=tfx_wettkaempfe.int_wettkaempfeid)>0 AND (NOT EXISTS (SELECT int_wertungen_x_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=tfx_wertungen.int_wertungenid) OR EXISTS (SELECT int_wertungen_x_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE tfx_wertungen_x_disziplinen.int_wertungenid=tfx_wertungen.int_wertungenid AND tfx_wertungen_x_disziplinen.int_disziplinenid=?))) AND (tfx_wettkaempfe.bol_kp='true' OR ?='true' OR (SELECT bol_kp FROM tfx_wettkaempfe_x_disziplinen WHERE int_wettkaempfeid=tfx_wettkaempfe.int_wettkaempfeid AND int_disziplinenid=?)='true') ORDER BY int_pos, tfx_wettkaempfe.var_nummer, tfx_mannschaften.int_nummer, tfx_mannschaften.int_mannschaftenid, tfx_wertungen.int_startnummer");
    query4.bindValue(0, geraet);
    query4.bindValue(1, (int)kuer);
    query4.bindValue(2, _global::checkHWK());
    query4.bindValue(3, riege);
    query4.bindValue(4, _global::getRunde());
    query4.bindValue(5, geraet);
    query4.bindValue(6, geraet);
    query4.bindValue(7, !kuer);
    query4.bindValue(8, geraet);
    query4.exec();
    starter.clear();
    if (_global::querySize(query4)>0)  {
        bool skip=false;
        query4.last();
        if (query4.value(6).toInt()>0) skip = true;
        query4.first();
        if (query4.value(6).toInt()>0) skip = true;
        query4.seek(-1);
        while (query4.next()) {
            if (query4.value(6).toInt()==0 && skip) continue;
            QStringList lst;
            for (int i=0;i<query4.record().count()-1;i++) {
                lst << query4.value(i).toString();
            }
            starter.append(lst);
        }
    }
    extraColumns.clear();
    extraColumnNames.clear();
    if (j) {
        QSqlQuery juryFields;
        juryFields.prepare("SELECT int_disziplinen_felderid, tfx_disziplinen_felder.var_name FROM tfx_disziplinen_felder INNER JOIN tfx_disziplinen USING (int_disziplinenid) WHERE int_disziplinenid=? AND bol_endwert = 'false' AND bol_enabled='true' ORDER BY int_sortierung");
        juryFields.bindValue(0,geraet);
        juryFields.exec();
        while(juryFields.next()) {
            extraColumns.append(juryFields.value(0).toInt());
            extraColumnNames.append(juryFields.value(1).toString());
        }
    }

    disinfo.prepare("SELECT var_einheit, var_maske, int_berechnung, var_kurz1 FROM tfx_disziplinen WHERE int_disziplinenid=?");
    disinfo.bindValue(0,geraet);
    disinfo.exec();
    disinfo.next();

    int kp=0;
    if (kuer) kp = 1;

    endwerte.clear();
    QSqlQuery endwerteQuery;
    endwerteQuery.prepare("SELECT rel_leistung, int_wertungenid, int_versuch FROM tfx_wertungen_details INNER JOIN tfx_wertungen USING (int_wertungenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_disziplinenid=? AND var_riege=? AND int_kp=?");
    endwerteQuery.bindValue(0,_global::checkHWK());
    endwerteQuery.bindValue(1,geraet);
    endwerteQuery.bindValue(2,riege);
    endwerteQuery.bindValue(3,kp);
    endwerteQuery.exec();
    while (endwerteQuery.next()) {
        endwerte[endwerteQuery.value(1).toInt()][endwerteQuery.value(2).toInt()] = endwerteQuery.value(0).toDouble();
    }

    detailwerte.clear();
    if (j) {
        QSqlQuery detailwerteQuery;
        detailwerteQuery.prepare("SELECT rel_leistung, int_disziplinen_felderid, tfx_wertungen.int_wertungenid, int_versuch FROM tfx_jury_results INNER JOIN tfx_disziplinen_felder USING (int_disziplinen_felderid) INNER JOIN tfx_wertungen ON tfx_jury_results.int_wertungenid = tfx_wertungen.int_wertungenid INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_disziplinenid=? AND var_riege=? AND int_kp=? AND bol_enabled='true'");
        detailwerteQuery.bindValue(0,_global::checkHWK());
        detailwerteQuery.bindValue(1,geraet);
        detailwerteQuery.bindValue(2,riege);
        detailwerteQuery.bindValue(3,kp);
        detailwerteQuery.exec();
        while (detailwerteQuery.next()) {
            detailwerte[detailwerteQuery.value(2).toInt()][detailwerteQuery.value(3).toInt()][detailwerteQuery.value(1).toInt()] = detailwerteQuery.value(0).toDouble();
        }
    }
    this->reset();
}

QList<int> QRiegenTableModel::getExtraColumnIDs() {
    return extraColumns;
}

int QRiegenTableModel::getCurrentID(const QModelIndex &index) {
    return starter.at(floor(index.row()/versuche)).at(4).toInt();
}

int QRiegenTableModel::getNextID(const QModelIndex &index) {
    if (floor((index.row()+versuche)/versuche) >= starter.size()) return -1;
    return starter.at(floor((index.row()+versuche)/versuche)).at(4).toInt();
}

int QRiegenTableModel::getLastID(const QModelIndex &index) {
    if (index.row()-versuche < 0) return -1;
    return starter.at(floor((index.row()-versuche)/versuche)).at(4).toInt();
}
