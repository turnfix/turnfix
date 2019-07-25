#include "venue.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Venue::initializeMapping()
{
    DBTable *venue = new DBTable("tfx_wettkampforte");
    venue->addColumn("id", "int_wettkampforteid", ColumnType::Integer, 0, false, "", "", true);
    venue->addColumn("name", "var_name", ColumnType::Varchar, 150);
    venue->addColumn("address", "var_adresse", ColumnType::Varchar, 200);
    venue->addColumn("zip", "var_plz", ColumnType::Varchar, 5);
    venue->addColumn("city", "var_ort", ColumnType::Varchar, 150);

    return venue;
}

const DBTable *Venue::m_mapping = Venue::initializeMapping();

const DBTable *Venue::mapping()
{
    return m_mapping;
}

void Venue::setId(int id)
{
    m_id = id;
}

QString Venue::nameAndCity()
{
    return this->m_name + ", " + this->m_city;
}

QString Venue::name() const
{
    return m_name;
}

void Venue::setName(const QString &name)
{
    m_name = name;
}

QString Venue::address() const
{
    return m_address;
}

void Venue::setAddress(const QString &address)
{
    m_address = address;
}

QString Venue::zip() const
{
    return m_zip;
}

void Venue::setZip(const QString &zip)
{
    m_zip = zip;
}

QString Venue::city() const
{
    return m_city;
}

void Venue::setCity(const QString &city)
{
    m_city = city;
}

int Venue::id() const
{
    return m_id;
}
