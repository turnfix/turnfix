#include "personrepository.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"
#include "model/entity/person.h"
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

void PersonRepository::persist(Person *person)
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());
    QSqlQuery query(db);
    if (person->id() == 0) {
        query.prepare("INSERT INTO tfx_personen "
                      "(var_vorname,var_nachname,var_adresse,var_plz,var_ort,var_telefon,var_fax,"
                      "var_email) VALUES (?,?,?,?,?,?,?,?)");
    } else {
        query.prepare(
            "UPDATE tfx_personen SET var_vorname=?, var_nachname=?, var_adresse=?, var_plz=?, "
            "var_ort=?, var_telefon=?, var_fax=?, var_email=? WHERE int_personenid=?");
        query.bindValue(8, person->id());
    }
    query.bindValue(0, person->firstName());
    query.bindValue(1, person->lastName());
    query.bindValue(2, person->address());
    query.bindValue(3, person->zip());
    query.bindValue(4, person->city());
    query.bindValue(5, person->phoneNumber());
    query.bindValue(6, person->faxNumber());
    query.bindValue(7, person->email());
    query.exec();

    if (person->id() == 0) {
        person->setId(query.lastInsertId().toInt());
    }
}

void PersonRepository::remove(Person *person) {}
