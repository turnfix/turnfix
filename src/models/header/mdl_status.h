#ifndef MDL_STATUS_H
#define MDL_STATUS_H

#include <QAbstractTableModel>
#include <QSqlQuery>

class QStatusTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    QStatusTableModel(const QSqlQuery &qry=QSqlQuery(), QObject *parent = 0)
        : QAbstractTableModel(parent), query(qry) {}
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    void setQuery(const QSqlQuery &query);
    void setSingle(bool s);
private:
    QSqlQuery query;
    bool single;
};
#endif
