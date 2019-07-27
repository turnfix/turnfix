#include "club.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Club::initializeMapping()
{
    DBTable *club = new DBTable("tfx_vereine");
    club->addColumn("id", "int_vereineid", ColumnType::Integer, 0, false, "", "", true);
    club->addColumn("contactPersonId", "int_personenid", ColumnType::Integer);
    club->addColumn("name", "var_name", ColumnType::Varchar, 150);
    club->addColumn("posCity", "int_start_ort", ColumnType::SmallInt, 0, true, "0");
    club->addColumn("website", "var_website", ColumnType::Varchar, 200);
    club->addColumn("regionId", "int_gaueid", ColumnType::Integer, 0, false, "1");
    club->addContraint("fky_gaueid", "tfx_gaue", "int_gaueid", "int_gaueid", "RESTRICT", "RESTRICT");
    club->addContraint("fky_personenid",
                       "tfx_personen",
                       "int_personenid",
                       "int_personenid",
                       "RESTRICT",
                       "RESTRICT");

    return club;
}

const DBTable *Club::m_mapping = Club::initializeMapping();

const DBTable *Club::mapping()
{
    return m_mapping;
}

Person *Club::contactPerson() const
{
    return m_contactPerson;
}

void Club::setContactPerson(Person *contactPerson)
{
    m_contactPerson = contactPerson;
    if (contactPerson == nullptr) {
        m_contactPersonId = 0;
        return;
    }

    m_contactPersonId = m_contactPerson->id();
}

int Club::id() const
{
    return m_id;
}

void Club::setId(int id)
{
    m_id = id;
}

QString Club::name() const
{
    return m_name;
}

void Club::setName(const QString &name)
{
    m_name = name;
}

int Club::regionId() const
{
    return m_regionId;
}

void Club::setRegionId(int regionId)
{
    m_regionId = regionId;
}

int Club::contactPersonId() const
{
    return m_contactPersonId;
}

void Club::setContactPersonId(int contactPersonId)
{
    m_contactPersonId = contactPersonId;
}

int Club::posCity() const
{
    return m_posCity;
}

void Club::setPosCity(int posCity)
{
    m_posCity = posCity;
}

QString Club::website() const
{
    return m_website;
}

void Club::setWebsite(const QString &website)
{
    m_website = website;
}
