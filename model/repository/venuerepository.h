#ifndef VENUEREPOSITORY_H
#define VENUEREPOSITORY_H

#include "abstractrepository.h"

class Venue;

class VenueRepository : public AbstractRepository<Venue>
{
public:
    explicit VenueRepository(EntityManager *em);

    QList<Venue *> loadAll();
    void persist(Venue *venue) override;
    void remove(Venue *venue) override;
};

#endif // VENUEREPOSITORY_H
