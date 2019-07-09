#include "scorecardtablemodel.h"
#include "libs/fparser/fparser.hh"
#include "src/global/header/_global.h"
#include <QColor>
#include <QKeyEvent>
#include <QSqlQuery>

int ScoreCardTableModel::rowCount(const QModelIndex &) const
{
    return disidsv.count();
}

int ScoreCardTableModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant ScoreCardTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= disidsv.count())
        return QVariant();
    int v = disidsv.at(index.row()).at(2);
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            QString name = disinfos.value(disidsv.at(index.row()).at(0)).at(3) + " (" + QString().setNum(v) + ". Versuch)";
            QSqlQuery checkKuer;
            checkKuer.prepare("SELECT tfx_wettkaempfe.int_wettkaempfeid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_veranstaltungen USING (int_veranstaltungenid) INNER JOIN tfx_wettkaempfe_x_disziplinen ON tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid INNER JOIN tfx_disziplinen USING (int_disziplinenid) WHERE int_wertungenid=? AND (tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true')");
            checkKuer.bindValue(0,wertid);
            checkKuer.exec();
            if (_global::querySize(checkKuer)>0) {
                if (disidsv.at(index.row()).at(1) == 0) {
                    name += " (Pflicht)";
                } else {
                    name += " (Kür)";
                }
            }
            return name;
        } else if (index.column() == 1) {
            double wert=0;
            wert = werte.value(disidsv.at(index.row()).at(0)).value(v).value(disidsv.at(index.row()).at(1));
            return _global::strLeistung(wert,disinfos.value(disidsv.at(index.row()).at(0)).at(0),disinfos.value(disidsv.at(index.row()).at(0)).at(1),disinfos.value(disidsv.at(index.row()).at(0)).at(2).toInt());
        }
    } else if (role == Qt::BackgroundColorRole && index.column() == 1) {
        QSqlQuery check;
        check.prepare("SELECT rel_max, var_formel FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_disziplinen ON tfx_disziplinen.int_disziplinenid = tfx_wettkaempfe_x_disziplinen.int_disziplinenid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE tfx_disziplinen.int_disziplinenid=? AND int_wertungenid=?");
        check.bindValue(0,disidsv.at(index.row()).at(0));
        check.bindValue(1,wertid);
        check.exec();
        check.next();
        FunctionParser fparser;
        fparser.Parse(check.value(1).toString().replace(",",".").toStdString(),"x");
        double Vars[] = {werte.value(disidsv.at(index.row()).at(0)).value(v).value(disidsv.at(index.row()).at(1))};
        if (fparser.Eval(Vars)>check.value(0).toDouble() && check.value(0).toDouble()>0) {
            return QColor(Qt::red);
        }
    }
    return QVariant();
}

QVariant ScoreCardTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Disziplin";
        case 1: return "Leistung";
        }
    }
    return QVariant();
}

Qt::ItemFlags ScoreCardTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    if (index.column() > 0) {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool ScoreCardTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        QSqlQuery query;
        query.prepare("SELECT tfx_disziplinen.int_disziplinenid, tfx_wertungen.int_wertungenid, tfx_disziplinen.var_maske, tfx_disziplinen.int_versuche FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen ON tfx_disziplinen.int_disziplinenid = tfx_wettkaempfe_x_disziplinen.int_disziplinenid INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_wertungenid=? AND tfx_disziplinen.int_disziplinenid=? ORDER BY int_sortierung");
        query.bindValue(0,wertid);
        query.bindValue(1,disidsv.at(index.row()).at(0));
        query.exec();
        query.next();
        int v = disidsv.at(index.row()).at(2);
        double leistung = _global::calcLeistung(value.toString());
        if (value != query.value(2).toString() && leistung > 0) {
            QSqlQuery query2;
            query2.prepare("SELECT int_wertungen_detailsid FROM tfx_wertungen_details WHERE int_wertungenid=? AND int_disziplinenid=? AND int_versuch=? AND int_kp=? LIMIT 1");
            query2.bindValue(0,query.value(1).toString());
            query2.bindValue(1,query.value(0).toString());
            query2.bindValue(2,v);
            query2.bindValue(3,disidsv.at(index.row()).at(1));
            query2.exec();
            QSqlQuery query3;
            if (_global::querySize(query2) == 1) {
            query2.next();
                query3.prepare("UPDATE tfx_wertungen_details SET int_wertungenid=?,int_disziplinenid=?,int_versuch=?,rel_leistung=?,int_kp=? WHERE int_wertungen_detailsid=?");
                query3.bindValue(5,query2.value(0).toString());
            } else {
                query3.prepare("INSERT INTO tfx_wertungen_details (int_wertungenid,int_disziplinenid,int_versuch,rel_leistung,int_kp) VALUES (?,?,?,?,?)");
            }
            query3.bindValue(0,query.value(1).toString());
            query3.bindValue(1,query.value(0).toString());
            query3.bindValue(2,v);
            query3.bindValue(3,leistung);
            query3.bindValue(4,disidsv.at(index.row()).at(1));
            query3.exec();
            werte[query.value(0).toInt()][v][disidsv.at(index.row()).at(1)] = leistung;
        } else {
            QSqlQuery query2;
            query2.prepare("DELETE FROM tfx_wertungen_details WHERE int_wertungenid=? AND int_disziplinenid=? AND int_versuch=? AND int_kp=?");
            query2.bindValue(0,query.value(1).toString());
            query2.bindValue(1,query.value(0).toString());
            query2.bindValue(2,v);
            query2.bindValue(3,disidsv.at(index.row()).at(1));
            query2.exec();
            query2.prepare("DELETE FROM tfx_jury_results WHERE int_wertungenid=? AND int_disziplinen_felderid IN (SELECT int_disziplinen_felderid FROM tfx_disziplinen_felder WHERE int_disziplinenid=?) AND int_versuch=? AND int_kp=?");
            query2.bindValue(0,query.value(1).toString());
            query2.bindValue(1,query.value(0).toString());
            query2.bindValue(2,v);
            query2.bindValue(3,disidsv.at(index.row()).at(1));
            query2.exec();
            werte[query.value(0).toInt()][v][disidsv.at(index.row()).at(1)] = 0;
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void ScoreCardTableModel::setTableData(int we, QList<QList<int>> id)
{
    beginResetModel();

    wertid = we;
    calcColumns(id);

    QSqlQuery disinfo;
    disinfo.prepare("SELECT var_einheit, var_maske, int_berechnung, var_kurz2, int_disziplinenid FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wertungen ON tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE int_wertungenid=?");
    disinfo.bindValue(0,wertid);
    disinfo.exec();
    while (disinfo.next()) {
        QStringList lst;
        lst << disinfo.value(0).toString() << disinfo.value(1).toString() << disinfo.value(2).toString() << disinfo.value(3).toString();
        disinfos[disinfo.value(4).toInt()] = lst;
    }

    werte.clear();
    QSqlQuery query2;
    query2.prepare("SELECT int_disziplinenid, int_versuch, int_kp, rel_leistung FROM tfx_wertungen_details WHERE int_wertungenid=?");
    query2.bindValue(0,wertid);
    query2.exec();
    while (query2.next()) {
        werte[query2.value(0).toInt()][query2.value(1).toInt()][query2.value(2).toInt()] = query2.value(3).toDouble();
    }

    endResetModel();
}

void ScoreCardTableModel::calcColumns(QList<QList<int>> disids)
{
    disidsv.clear();
    for(int i=0;i<disids.size();i++) {
        QSqlQuery query2;
        query2.prepare("SELECT int_versuche FROM tfx_disziplinen WHERE int_disziplinenid=?");
        query2.bindValue(0,disids.at(i).at(0));
        query2.exec();
        query2.next();
        for (int j=0;j<query2.value(0).toInt();j++) {
            disidsv.append(disids.at(i));
            disidsv.last().append(j+1);
        }
    }
}
