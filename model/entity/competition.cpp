#include "competition.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Competition::initializeMapping()
{
    DBTable *competition = new DBTable("tfx_wettkaempfe");
    competition->addColumn("id", "int_wettkaempfeid", ColumnType::Integer, 0, false, "", "", true);
    competition->addColumn("eventId", "int_veranstaltungenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_veranstaltungenid",
                       "tfx_veranstaltungen",
                       "int_veranstaltungenid",
                       "RESTRICT",
                       "CASCADE");
    competition->addColumn("divisionId", "int_bereicheid", ColumnType::Integer, 0, false)
        ->addContraint("fky_bereicheid", "tfx_bereiche", "int_bereicheid", "RESTRICT", "RESTRICT");
    competition->addColumn("type", "int_typ", ColumnType::SmallInt, 0, true, "0");
    competition->addColumn("number", "var_nummer", ColumnType::Varchar, 5);
    competition->addColumn("name", "var_name", ColumnType::Varchar, 150);
    competition->addColumn("minYear", "yer_von", ColumnType::SmallInt, 0, false);
    competition->addColumn("maxYear", "yer_bis", ColumnType::SmallInt);
    competition->addColumn("qualifiers", "int_qualifikation", ColumnType::SmallInt, 0, true, "0");
    competition->addColumn("scores", "int_wertungen", ColumnType::SmallInt);
    competition
        ->addColumn("dropScores", "bol_streichwertung", ColumnType::Boolean, 0, true, "'false'");
    competition
        ->addColumn("showAgeGroup", "bol_ak_anzeigen", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("apparatusChoices",
                           "bol_wahlwettkampf",
                           ColumnType::Boolean,
                           0,
                           true,
                           "'false'");
    competition->addColumn("round", "int_durchgang", ColumnType::SmallInt, 0, true, "1");
    competition->addColumn("group", "int_bahn", ColumnType::SmallInt, 0, true, "1");
    competition->addColumn("startTime", "tim_startzeit", ColumnType::TimeNoTimezone);
    competition->addColumn("warmUpTime", "tim_einturnen", ColumnType::TimeNoTimezone);
    competition->addColumn("showInfo", "bol_info_anzeigen", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("freeAndCompulsary", "bol_kp", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("sortAsc", "bol_sortasc", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("manualSort", "bol_mansort", ColumnType::Boolean, 0, true, "'false'");
    competition
        ->addColumn("extraApparatusPoints", "bol_gerpkt", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("numDropScores", "int_anz_streich", ColumnType::SmallInt, 0, true, "0");

    return competition;
}

const DBTable *Competition::m_mapping = Competition::initializeMapping();

const DBTable *Competition::mapping()
{
    return m_mapping;
}

int Competition::id() const
{
    return m_id;
}

void Competition::setId(int id)
{
    m_id = id;
}

int Competition::eventId() const
{
    return m_eventId;
}

void Competition::setEventId(int eventId)
{
    m_eventId = eventId;
}

int Competition::divisionId() const
{
    return m_divisionId;
}

void Competition::setDivisionId(int sectionId)
{
    m_divisionId = sectionId;
}

int Competition::type() const
{
    return m_type;
}

void Competition::setType(int type)
{
    m_type = type;
}

QString Competition::number() const
{
    return m_number;
}

void Competition::setNumber(const QString &number)
{
    m_number = number;
}

QString Competition::name() const
{
    return m_name;
}

void Competition::setName(const QString &name)
{
    m_name = name;
}

int Competition::minYear() const
{
    return m_minYear;
}

QString Competition::minYearText() const
{
    if (m_maxYear == 3) {
        return tr("offen");
    }
    return QString::number(m_minYear);
}

void Competition::setMinYear(int minYear)
{
    m_minYear = minYear;
}

int Competition::maxYear() const
{
    return m_maxYear;
}

QString Competition::maxYearText() const
{
    switch (m_maxYear) {
    case 1:
        return tr("und älter");
    case 2:
        return tr("und jünger");
    case 3:
        return tr("offen");
    default:
        return QString::number(m_maxYear);
    }
}

void Competition::setMaxYear(int maxYear)
{
    m_maxYear = maxYear;
}

int Competition::qualifiers() const
{
    return m_qualifiers;
}

void Competition::setQualifiers(int qualifiers)
{
    m_qualifiers = qualifiers;
}

int Competition::scores() const
{
    return m_scores;
}

void Competition::setScores(int scores)
{
    m_scores = scores;
}

bool Competition::dropScores() const
{
    return m_dropScores;
}

void Competition::setDropScores(bool dropScores)
{
    m_dropScores = dropScores;
}

bool Competition::showAgeGroup() const
{
    return m_showAgeGroup;
}

void Competition::setShowAgeGroup(bool showAgeGroup)
{
    m_showAgeGroup = showAgeGroup;
}

bool Competition::apparatusChoices() const
{
    return m_apparatusChoices;
}

void Competition::setApparatusChoices(bool apparatusChoices)
{
    m_apparatusChoices = apparatusChoices;
}

int Competition::round() const
{
    return m_round;
}

void Competition::setRound(int round)
{
    m_round = round;
}

int Competition::group() const
{
    return m_group;
}

void Competition::setGroup(int group)
{
    m_group = group;
}

QTime Competition::startTme() const
{
    return m_startTme;
}

void Competition::setStartTme(const QTime &startTme)
{
    m_startTme = startTme;
}

QTime Competition::warmUpTime() const
{
    return m_warmUpTime;
}

void Competition::setWarmUpTime(const QTime &warmUpTime)
{
    m_warmUpTime = warmUpTime;
}

bool Competition::showInfo() const
{
    return m_showInfo;
}

void Competition::setShowInfo(bool showInfo)
{
    m_showInfo = showInfo;
}

bool Competition::freeAndCompulsary() const
{
    return m_freeAndCompulsary;
}

void Competition::setFreeAndCompulsary(bool freeAndCompulsary)
{
    m_freeAndCompulsary = freeAndCompulsary;
}

bool Competition::sortAsc() const
{
    return m_sortAsc;
}

void Competition::setSortAsc(bool sortAsc)
{
    m_sortAsc = sortAsc;
}

bool Competition::manualSort() const
{
    return m_manualSort;
}

void Competition::setManualSort(bool manualSort)
{
    m_manualSort = manualSort;
}

bool Competition::extraApparatusPoints() const
{
    return m_extraApparatusPoints;
}

void Competition::setExtraApparatusPoints(bool extraApparatusPoints)
{
    m_extraApparatusPoints = extraApparatusPoints;
}

int Competition::numDropScores() const
{
    return m_numDropScores;
}

void Competition::setNumDropScores(int numDropScores)
{
    m_numDropScores = numDropScores;
}

Event *Competition::event() const
{
    return m_event;
}

void Competition::setEvent(Event *event)
{
    m_event = event;
}

Division *Competition::division() const
{
    return m_division;
}

void Competition::setDivision(Division *division)
{
    m_division = division;
    if (division == nullptr) {
        m_divisionId = 0;
        return;
    }

    m_divisionId = division->id();
}
