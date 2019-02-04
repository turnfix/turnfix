#ifndef MDL_KARTE_H
#define MDL_KARTE_H

#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QStringList>

class QKartenTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    QKartenTableModel(QObject *parent = 0)
        : QAbstractTableModel(parent) {}
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                  int role = Qt::EditRole);
    void setTableData(int wertid, QList< QList<int> > disids);
private:
    QList< QList<int> > disidsv;
    int wertid;
    void calcColumns(QList< QList<int> > disids);
    QMap< int, QMap <int, QMap < int, double > > > werte;
    QMap< int, QStringList > disinfos;
};
#endif
