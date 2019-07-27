#include "eventrepository.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"
#include "model/entity/bankaccount.h"
#include "model/entity/person.h"
#include "model/entity/venue.h"
#include "model/entitymanager.h"
#include "model/querybuilder.h"
#include <QMetaProperty>
#include <QSqlQuery>

#include <QDebug>
#include <QSqlError>

bool EventRepository::persist(Event *event)
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());
    QSqlQuery query(db);

    if (event->id() == 0) {
        query.prepare(
            "INSERT INTO tfx_veranstaltungen "
            "(int_wettkampforteid, int_meldung_an, int_ansprechpartner, "
            "int_kontenid, int_hauptwettkampf, var_name, int_runde, dat_von, dat_bis, "
            "dat_meldeschluss, bol_rundenwettkampf, var_veranstalter, int_edv, int_helfer, "
            "int_kampfrichter, var_meldung_website, var_verwendungszweck, rel_meldegeld, "
            "rel_nachmeldung, bol_faellig_nichtantritt, bol_ummeldung_moeglich, "
            "bol_nachmeldung_moeglich, txt_meldung_an, txt_startberechtigung,"
            "txt_teilnahmebedingungen, txt_siegerauszeichnung ,txt_kampfrichter, txt_hinweise) "
            "VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    } else {
        query.prepare(
            "UPDATE tfx_veranstaltungen SET "
            "int_wettkampforteid=?,int_meldung_an=?,int_ansprechpartner=?,int_kontenid=?,int_"
            "hauptwettkampf=?,var_name=?,int_runde=?,dat_von=?,dat_bis=?,dat_meldeschluss=?,bol_"
            "rundenwettkampf=?,var_veranstalter=?,int_edv=?,int_helfer=?,int_kampfrichter=?,var_"
            "meldung_website=?,var_verwendungszweck=?,rel_meldegeld=?,rel_nachmeldung=?,bol_"
            "faellig_nichtantritt=?,bol_ummeldung_moeglich=?,bol_nachmeldung_moeglich=?,txt_"
            "meldung_an=?,txt_startberechtigung=?,txt_teilnahmebedingungen=?,txt_"
            "siegerauszeichnung=?,txt_kampfrichter=?,txt_hinweise=? WHERE int_veranstaltungenid=?");
        query.bindValue(28, event->id());
    }

    query.bindValue(0, event->venue()->id());
    if (event->registrationContact() != nullptr) {
        query.bindValue(1, event->registrationContact()->id());
    } else {
        query.bindValue(1, QVariant(QVariant::Int));
    }
    if (event->eventContact() != nullptr) {
        query.bindValue(2, event->eventContact()->id());
    } else {
        query.bindValue(2, QVariant(QVariant::Int));
    }
    if (event->bankAccount() != nullptr) {
        query.bindValue(3, event->bankAccount()->id());
    } else {
        query.bindValue(3, QVariant(QVariant::Int));
    }
    if (event->mainEvent() != event) {
        query.bindValue(4, event->mainEvent()->id());
    } else {
        query.bindValue(4, QVariant(QVariant::Int));
    }
    query.bindValue(5, event->name());
    query.bindValue(6, event->round());
    query.bindValue(7, event->startDate());
    query.bindValue(8, event->endDate());
    query.bindValue(9, event->registrationDeadline());
    query.bindValue(10, event->multiRoundEvent());
    query.bindValue(11, event->organizer());
    query.bindValue(12, event->itTeamCount());
    query.bindValue(13, event->volunteersCount());
    query.bindValue(14, event->judgesCount());
    query.bindValue(15, event->website());
    query.bindValue(16, event->reference());
    query.bindValue(17, event->registrationFee());
    query.bindValue(18, event->lateFee());
    query.bindValue(19, event->chargeNoShow());
    query.bindValue(20, event->changesAllowed());
    query.bindValue(21, event->lateRegistrationAllowed());
    query.bindValue(22, event->registrationNotes());
    query.bindValue(23, event->licenseRequirements());
    query.bindValue(24, event->entryRequirements());
    query.bindValue(25, event->awards());
    query.bindValue(26, event->judges());
    query.bindValue(27, event->notes());

    bool result = query.exec();

    if (event->id() == 0) {
        event->setId(query.lastInsertId().toInt());
    }

    return result;
}

bool EventRepository::remove(Event *event) {}

EventRepository::EventRepository(EntityManager *em)
    : AbstractRepository<Event>(em)
{}

QList<Event *> EventRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<Event> qb;
    qb.select(Event::staticMetaObject, Event::mapping());
    qb.join(Venue::staticMetaObject, Venue::mapping(), "Event", "venue", "venueId");
    qb.orderBy("Event", "startDate", "DESC");

    QList<Event *> output = qb.query(db);

    return output;
}
