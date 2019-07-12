#include <QSqlQuery>
#include <QVariant>
#include "competition.h"

QMap<QString, Competition*> Competition::competitionsByNumber =  QMap<QString, Competition*>();

Competition* Competition::getByNumber(Event *event, QString number)
{
    if (competitionsByNumber.contains(number)) {
        return competitionsByNumber.value(number);
    }

    QSqlQuery query;
    query.prepare("SELECT var_nummer, int_typ FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? AND var_nummer=? LIMIT 1");
    query.bindValue(0, event->mainEventId());
    query.bindValue(1, number);
    query.exec();
    query.next();

    Competition *competition = new Competition(event, query.value(0).toString(), query.value(1).toInt());

    competitionsByNumber.insert(number, competition);

    return competition;
}

Competition::Competition(Event *event, QString number, int type, QObject *parent) : QObject (parent)
{
    this->event = event;
    this->number = number;
    this->type = type;
}

Event* Competition::getEvent()
{
    return this->event;
}

QString Competition::getNumber()
{
    return this->number;
}

int Competition::getType()
{
    return this->type;
}
