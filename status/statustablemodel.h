#ifndef STATUSTABLEMODEL_H
#define STATUSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QSqlQuery>

class StatusTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    StatusTableModel(const QSqlQuery &qry = QSqlQuery(), QObject *parent = nullptr)
        : QAbstractTableModel(parent)
        , query(qry)
    {}
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
