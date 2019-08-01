#ifndef VENUEREPOSITORY_H
#define VENUEREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/venue.h"

class VenueRepository : public AbstractRepository<Venue>
{
public:
    explicit VenueRepository(EntityManager *em);

    QList<Venue *> loadAll();
};

#endif // VENUEREPOSITORY_H
