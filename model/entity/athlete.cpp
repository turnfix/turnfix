#include "athlete.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Athlete::initializeMapping()
{
    DBTable *athlete = new DBTable("tfx_teilnehmer");
    athlete->addColumn("id", "int_teilnehmerid", ColumnType::Integer, 0, false, "", "", true);
    athlete->addColumn("clubId", "int_vereineid", ColumnType::Integer, 0, false)
        ->addContraint("fky_vereineid", "tfx_vereine", "int_vereineid", "RESTRICT", "RESTRICT");
    athlete->addColumn("firstName", "var_vorname", ColumnType::Varchar, 150);
    athlete->addColumn("lastName", "var_nachname", ColumnType::Varchar, 150);
    athlete->addColumn("gender", "int_geschlecht", ColumnType::SmallInt, 0, false);
    athlete->addColumn("dateOfBirth", "dat_geburtstag", ColumnType::Date);
    athlete->addColumn("yearOfBirthOnly", "bool_nur_jahr", ColumnType::Boolean, 0, true, "'true'");
    athlete->addColumn("license", "int_startpassnummer", ColumnType::Varchar, 50);

    return athlete;
}

const DBTable *Athlete::m_mapping = Athlete::initializeMapping();

const DBTable *Athlete::mapping()
{
    return m_mapping;
}

Club *Athlete::club() const
{
    return m_club;
}

void Athlete::setClub(Club *value)
{
    m_club = value;

    if (m_club == nullptr) {
        m_clubId = 0;
        return;
    }

    m_clubId = m_club->id();
}

int Athlete::id() const
{
    return m_id;
}

void Athlete::setId(int id)
{
    m_id = id;
}

int Athlete::clubId() const
{
    return m_clubId;
}

void Athlete::setClubId(int clubId)
{
    m_clubId = clubId;
}

QString Athlete::firstName() const
{
    return m_firstName;
}

void Athlete::setFirstName(const QString &firstName)
{
    m_firstName = firstName;
}

QString Athlete::lastName() const
{
    return m_lastName;
}

void Athlete::setLastName(const QString &lastName)
{
    m_lastName = lastName;
}

QString Athlete::fullName() const
{
    return QString("%1 %2").arg(m_firstName, m_lastName);
}

int Athlete::gender() const
{
    return m_gender;
}

void Athlete::setGender(const int &gender)
{
    m_gender = gender;
}

QDate Athlete::dateOfBirth() const
{
    return m_dateOfBirth;
}

void Athlete::setDateOfBirth(const QDate &dateOfBirth)
{
    m_dateOfBirth = dateOfBirth;
}

bool Athlete::yearOfBirthOnly() const
{
    return m_yearOfBirthOnly;
}

void Athlete::setYearOfBirthOnly(bool yearOfBirthOnly)
{
    m_yearOfBirthOnly = yearOfBirthOnly;
}

QString Athlete::license() const
{
    return m_license;
}

void Athlete::setLicense(const QString &license)
{
    m_license = license;
}
