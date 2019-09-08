#include "participant.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"
#include <QDate>

DBTable *Participant::initializeMapping()
{
    DBTable *participant = new DBTable("tfx_teilnehmer");
    participant->addColumn("id", "int_teilnehmerid", ColumnType::Integer, 0, false, "", "", true);
    participant->addColumn("clubId", "int_vereineid", ColumnType::Integer, 0, false)
            ->addContraint("fky_clubId", "tfx_vereine", "int_vereineid", "RESTRICT", "RESTRICT");
    participant->addColumn("firstName", "var_vorname", ColumnType::Varchar, 150);
    participant->addColumn("lastName", "var_nachname", ColumnType::Varchar, 150);
    participant->addColumn("gender", "int_geschlecht", ColumnType::Integer, 0, true, "1");
    participant->addColumn("birthday", "dat_geburtstag", ColumnType::Date, 0, false);
    participant->addColumn("onlyYear", "bool_nur_jahr", ColumnType::Boolean, 0, false, "false");
    participant->addColumn("startpass", "int_startpassnummer", ColumnType::Integer, 0, false);

    return participant;
}

const DBTable *Participant::m_mapping = Participant::initializeMapping();

const DBTable *Participant::mapping()
{
    return m_mapping;
}

int Participant::id() const
{
    return m_id;
}

//club
Club *Participant::club()
{
    return m_club;
}

void Participant::setClub(Club *club)
{
    m_club = club;
    if (club == nullptr){
        m_clubId = 0;
        return;
    }
}

int Participant::clubId() const
{
    return m_clubId;
}

void Participant::setClubId(int clubId)
{
    m_clubId = clubId;
}

//fistname
QString Participant::firstName() const
{
    return m_firstName;
}

void Participant::setFirstName(const QString &fistName)
{
    m_firstName = fistName;
}

//lastname
QString Participant::lastName() const
{
    return m_lastName;
}

void Participant::setLastName(const QString &lastName)
{
    m_lastName = lastName;
}

//gender
int Participant::gender() const
{
   return m_gender;
}

void Participant::setGender(const int &gender)
{
    m_gender = gender;
}

//birthday
QDate Participant::birthday() const
{
    return m_birthday;
}

void Participant::setBirthday(const QDate &birthday)
{
    m_birthday = birthday;
}

//onlyYear
bool Participant::onlyYear() const
{
    return m_onlyYear;
}

void Participant::setOnlyYear(const bool &onlyYear)
{
    m_onlyYear = onlyYear;
}

//startpass
int Participant::startPass() const
{
    return m_startPass;
}

void Participant::setStartPass(const int &startPass)
{
    m_startPass = startPass;
}
