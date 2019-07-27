#include "venuerepository.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"
#include "model/entity/venue.h"
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

void VenueRepository::persist(Venue *venue)
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());
    QSqlQuery query(db);
    if (venue->id() == 0) {
        query.prepare("INSERT INTO tfx_wettkampforte (var_name,var_adresse,var_plz,var_ort) "
                      "VALUES (?,?,?,?)");
    } else {
        query.prepare("UPDATE tfx_wettkampforte SET var_name=?, var_adresse=?, var_plz=?, "
                      "var_ort=? WHERE int_wettkampforteid=?");
        query.bindValue(4, venue->id());
    }
    query.bindValue(0, venue->name());
    query.bindValue(1, venue->address());
    query.bindValue(2, venue->zip());
    query.bindValue(3, venue->city());
    query.exec();

    if (venue->id() == 0) {
        venue->setId(query.lastInsertId().toInt());
    }
}

void VenueRepository::remove(Venue *venue) {}
