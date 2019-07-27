#include "personrepository.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"
#include "model/entitymanager.h"
#include <QMetaProperty>
#include <QSqlQuery>

PersonRepository::PersonRepository(EntityManager *em)
    : AbstractRepository<Person>(em)
{}

QList<Person *> PersonRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());
    QList<Person *> output;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM tfx_personen ORDER BY var_nachname, var_vorname");
    query.exec();

    while (query.next()) {
        Person *person = new Person();
        const QMetaObject *metaObj = person->metaObject();
        const DBTable *mappingObj = person->mapping();

        for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
            QMetaProperty property = metaObj->property(i);
            DBColumn *column = mappingObj->columnByProperty(property.name());

            person->setProperty(property.name(), query.value(column->name()));
        }

        output.append(person);
    }

    return output;
}
