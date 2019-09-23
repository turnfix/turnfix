#include "dbchecker.h"
#include "dbcolumn.h"
#include "dbconstraint.h"
#include "dbtable.h"
#include "entity/abstractconnection.h"
#include "entity/athlete.h"
#include "entity/bankaccount.h"
#include "entity/club.h"
#include "entity/competition.h"
#include "entity/competitiondiscipline.h"
#include "entity/country.h"
#include "entity/discipline.h"
#include "entity/disciplinefield.h"
#include "entity/disciplinegroup.h"
#include "entity/disciplinegroupitem.h"
#include "entity/disciplineposition.h"
#include "entity/division.h"
#include "entity/event.h"
#include "entity/formula.h"
#include "entity/group.h"
#include "entity/groupmember.h"
#include "entity/juryscore.h"
#include "entity/layout.h"
#include "entity/layoutfield.h"
#include "entity/penalty.h"
#include "entity/person.h"
#include "entity/region.h"
#include "entity/score.h"
#include "entity/scoredetails.h"
#include "entity/scorediscipline.h"
#include "entity/sport.h"
#include "entity/squaddiscipline.h"
#include "entity/startingorder.h"
#include "entity/state.h"
#include "entity/status.h"
#include "entity/team.h"
#include "entity/teammember.h"
#include "entity/teampenalty.h"
#include "entity/venue.h"

DBChecker::DBChecker(AbstractConnection *connection, QObject *parent)
    : QThread(parent), connection(connection)
{}

void DBChecker::run()
{
    bool connectionEstablished = connection->connect("checker");
    if (!connectionEstablished) {
        return;
    }

    QList<const DBTable *> tables = tableData();

    emit tableCount(tables.size());

    for (int i=0;i<tables.size();i++)
    {
        const DBTable *table = tables.at(i);
        emit currentTable(
            tr("Überprüfe %1... %2 von %3").arg(table->name()).arg(i + 1).arg(tables.size()));
        // emit columnFKCount(table->columnFKCount());
        connect(table, SIGNAL(columnsChecked(int)), this, SLOT(columnUpdate(int)));
        table->check("checker");
        emit tablesChecked(i+1);
    }

    emit currentTable("Datenbanküberprüfung/-aktualisierung erfolgreich abgeschlossen.");

    connection->close("checker");
}

void DBChecker::columnUpdate(int count)
{
    emit columnsChecked(count);
}

QList<const DBTable *> DBChecker::tableData()
{
    QList<const DBTable *> tables;

    tables.append(Division::mapping());
    tables.append(Sport::mapping());
    tables.append(Formula::mapping());
    tables.append(Discipline::mapping());
    tables.append(DisciplineField::mapping());

    tables.append(Country::mapping());
    tables.append(State::mapping());
    tables.append(Region::mapping());
    tables.append(Person::mapping());
    tables.append(Club::mapping());

    tables.append(Group::mapping());
    tables.append(Athlete::mapping());
    tables.append(BankAccount::mapping());
    tables.append(Status::mapping());
    tables.append(Venue::mapping());

    tables.append(Event::mapping());
    tables.append(Competition::mapping());
    tables.append(CompetitionDiscipline::mapping());
    tables.append(DisciplinePosition::mapping());
    tables.append(Team::mapping());

    tables.append(Penalty::mapping());
    tables.append(TeamPenalty::mapping());
    tables.append(TeamMember::mapping());
    tables.append(GroupMember::mapping());
    tables.append(SquadDiscipline::mapping());

    tables.append(Score::mapping());
    tables.append(ScoreDetails::mapping());
    tables.append(ScoreDiscipline::mapping());
    tables.append(JuryScore::mapping());
    tables.append(Layout::mapping());

    tables.append(LayoutField::mapping());
    tables.append(DisciplineGroup::mapping());
    tables.append(DisciplineGroupItem::mapping());
    tables.append(StartingOrder::mapping());

    return tables;
}
