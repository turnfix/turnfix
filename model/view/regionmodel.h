#ifndef REGIONMODEL_H
#define REGIONMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class State;
class Region;

class RegionModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RegionModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchRegions();

private:
    QList<Region *> m_regions;
    EntityManager *m_em;
};

#endif // REGIONMODEL_H
