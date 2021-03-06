#ifndef MDL_EINTEILEN_H
#define MDL_EINTEILEN_H

#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QStringList>
#include <QMimeData>

class QEinteilTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    QEinteilTableModel(QObject *parent = 0) : QAbstractTableModel(parent) { riege=""; }
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QStringList takeRow(int row);
    void insertRow(QStringList data);
    void setTableData();
public slots:
    void setRiege(QString riege);
    void updateRiege(QString riege);
private:
    QList<QStringList> tabledata;
    QString riege;
};
#endif
