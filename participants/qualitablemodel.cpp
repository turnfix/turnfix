#include "qualitablemodel.h"
#include "model/entity/event.h"
#include "model/settings/session.h"
#include "src/global/header/_global.h"
#include <QKeyEvent>
#include <QSqlQuery>

int QualificationStandardsTableModel::rowCount(const QModelIndex &) const
{
    return disids.count();
}

int QualificationStandardsTableModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant QualificationStandardsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= disids.count())
        return QVariant();
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            QSqlQuery query;
            query.prepare("SELECT var_kurz2 FROM tfx_disziplinen WHERE int_disziplinenid=?");
            query.bindValue(0, disids.at(index.row()));
            query.exec();
            query.next();
            return query.value(0).toString();
        } else if (index.column() == 1) {
            QSqlQuery query2;
            query2.prepare("SELECT rel_leistung FROM tfx_quali_leistungen WHERE int_disziplinenid=? AND int_wertungenid=? LIMIT 1");
            query2.bindValue(0,disids.at(index.row()));
            query2.bindValue(1,wertid);
            query2.exec();
            QSqlQuery query3;
            query3.prepare("SELECT var_einheit, var_maske, int_berechnung FROM tfx_disziplinen WHERE int_disziplinenid=?");
            query3.bindValue(0,disids.at(index.row()));
            query3.exec();
            query3.next();
            if (_global::querySize(query2) == 0) {
                return _global::strLeistung(0.0,query3.value(0).toString(),query3.value(1).toString(),query3.value(2).toInt());
            } else {
                query2.next();
                return _global::strLeistung(query2.value(0).toDouble(),query3.value(0).toString(),query3.value(1).toString(),query3.value(2).toInt());
            }
        }
    }
    return QVariant();
}

QVariant QualificationStandardsTableModel::headerData(int section,
                                                      Qt::Orientation orientation,
                                                      int role) const
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

Qt::ItemFlags QualificationStandardsTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    if (index.column() > 0) {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }
    return Qt::ItemIsEnabled;
}

bool QualificationStandardsTableModel::setData(const QModelIndex &index,
                                               const QVariant &value,
                                               int role)
{
    Event *event = Session::getInstance()->getEvent();

    if (index.isValid() && role == Qt::EditRole) {
        QSqlQuery query;
        query.prepare("SELECT tfx_disziplinen.int_disziplinenid, tfx_wertungen.int_wertungenid, tfx_disziplinen.var_maske, tfx_disziplinen.int_versuche FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen ON tfx_disziplinen.int_disziplinenid = tfx_wettkaempfe_x_disziplinen.int_disziplinenid INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_veranstaltungenid=? AND tfx_wertungen.int_wertungenid=? AND tfx_disziplinen.int_disziplinenid=? ORDER BY int_sortierung");
        query.bindValue(0,event->id());
        query.bindValue(1,wertid);
        query.bindValue(2,disids.at(index.row()));
        query.exec();
        query.next();
        double leistung = _global::calcLeistung(value.toString());
        if (value != query.value(2).toString() && leistung > 0) {
            QSqlQuery query2;
            query2.prepare("SELECT int_quali_leistungenid FROM tfx_quali_leistungen WHERE int_wertungenid=? AND int_disziplinenid=? LIMIT 1");
            query2.bindValue(0,query.value(1).toString());
            query2.bindValue(1,query.value(0).toString());
            query2.exec();
            query2.next();
            QSqlQuery query3;
            if (_global::querySize(query2) == 1) {
                query3.prepare("UPDATE tfx_quali_leistungen SET int_wertungenid=?,int_disziplinenid=?,rel_leistung=? WHERE int_quali_leistungenid=?");
                query3.bindValue(3,query2.value(0).toString());
            } else {
                query3.prepare("INSERT INTO tfx_quali_leistungen (int_wertungenid,int_disziplinenid,rel_leistung) VALUES (?,?,?)");
            }
            query3.bindValue(0,query.value(1).toString());
            query3.bindValue(1,query.value(0).toString());
            query3.bindValue(2,leistung);
            query3.exec();
        } else {
            QSqlQuery query2;
            query2.prepare("DELETE FROM tfx_quali_leistungen WHERE int_wertungenid=? AND int_disziplinenid=?");
            query2.bindValue(0,query.value(1).toString());
            query2.bindValue(1,query.value(0).toString());
            query2.exec();
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void QualificationStandardsTableModel::setTableData(int we, QList<int> id)
{
    beginResetModel();
    disids = id;
    wertid = we;
    endResetModel();
}
