#ifndef QUALIFICATIONSTANDARDSTABLEMODEL_H
#define QUALIFICATIONSTANDARDSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QSqlQuery>

class QualificationStandardsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                  int role = Qt::EditRole);
    void setTableData(int wertid, QList<int> disids);

private:
    QList<int> disids;
    int wertid;
};
#endif
