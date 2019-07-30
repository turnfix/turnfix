#include "venuerepository.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"
#include "model/entitymanager.h"
#include <QMetaProperty>
#include <QSqlQuery>

VenueRepository::VenueRepository(EntityManager *em)
    : AbstractRepository<Venue>(em)
{}

QList<Venue *> VenueRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());
    QList<Venue *> output;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM tfx_wettkampforte ORDER BY var_name DESC");
    query.exec();

    while (query.next()) {
        auto venue = new Venue();
        const QMetaObject *metaObj = venue->metaObject();
        const DBTable *mappingObj = Venue::mapping();

        for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
            QMetaProperty property = metaObj->property(i);
            DBColumn *column = mappingObj->columnByProperty(property.name());

            venue->setProperty(property.name(), query.value(column->name()));
        }

        output.append(venue);
    }

    return output;
}
