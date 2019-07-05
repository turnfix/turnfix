#include "statustablemodel.h"
#include "src/global/header/_global.h"
#include <QColor>
#include <QSqlRecord>

int StatusTableModel::rowCount(const QModelIndex &) const
{
    return _global::querySize(query);
}

int StatusTableModel::columnCount(const QModelIndex &) const
{
    if (_global::querySize(query) > 0) {
        QSqlQuery *qry = const_cast<QSqlQuery*>(&query);
        qry->seek(0);
        return qry->record().count();
    }
    return 0;
}

QVariant StatusTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= _global::querySize(query))
        return QVariant();
    if (role == Qt::DisplayRole) {
        QSqlQuery *qry = const_cast<QSqlQuery*>(&query);
        qry->seek(index.row());
        return qry->value(index.column());
    }
    if (role == Qt::BackgroundRole) {
        int s;
        if (single) {
            s=3 ;
        } else {
            s=0;
        }
        if (index.column() > s) {
            QSqlQuery *qry = const_cast<QSqlQuery*>(&query);
            qry->seek(index.row());
            QSqlQuery cq;
            cq.prepare("SELECT ary_colorcode FROM tfx_status WHERE var_name LIKE ? LIMIT 1");
            cq.bindValue(0,qry->value(index.column()).toString());
            cq.exec();
            if (_global::querySize(cq)>0) {
                cq.next();
                QList<int> color = _global::splitColorArray(cq.value(0).toString());
                return QColor(color.at(0),color.at(1),color.at(2));
            } else {
                return QColor(0,0,0);
            }
        }
    }
    return QVariant();
}

QVariant StatusTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        QSqlQuery *qry = const_cast<QSqlQuery*>(&query);
        qry->seek(0);
        return qry->record().fieldName(section);
    } else {
        return QString("Row %1").arg(section);
    }
}

void StatusTableModel::setQuery(const QSqlQuery &qry)
{
    beginResetModel();
    query = qry;
    endResetModel();
}

void StatusTableModel::setSingle(bool s)
{
    single = s;
}
