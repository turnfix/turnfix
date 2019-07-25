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
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

    void fetchVenues();

private:
    QList<Venue *> m_venues;
    EntityManager *m_em;
};

#endif // VENUEMODEL_H
