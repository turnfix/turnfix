#ifndef VENUEMODEL_H
#define VENUEMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Venue;

class VenueModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit VenueModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void fetchVenues();

private:
    QList<Venue *> m_venues;
    EntityManager *m_em;
};

#endif // VENUEMODEL_H
