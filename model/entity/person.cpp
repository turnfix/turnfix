#include "person.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Person::initializeMapping()
{
    DBTable *person = new DBTable("tfx_personen");
    person->addColumn("id", "int_personenid", ColumnType::Integer, 0, false, "", "", true);
    person->addColumn("firstName", "var_vorname", ColumnType::Varchar, 150);
    person->addColumn("lastName", "var_nachname", ColumnType::Varchar, 150);
    person->addColumn("address", "var_adresse", ColumnType::Varchar, 200);
    person->addColumn("zip", "var_plz", ColumnType::Varchar, 5);
    person->addColumn("city", "var_ort", ColumnType::Varchar, 150);
    person->addColumn("phoneNumber", "var_telefon", ColumnType::Varchar, 25);
    person->addColumn("faxNumber", "var_fax", ColumnType::Varchar, 25);
    person->addColumn("email", "var_email", ColumnType::Varchar, 200);

    return person;
}

const DBTable *Person::m_mapping = Person::initializeMapping();

const DBTable *Person::mapping()
{
    return m_mapping;
}

int Person::id() const
{
    return m_id;
}

void Person::setId(int id)
{
    m_id = id;
}

QString Person::firstName() const
{
    return m_firstName;
}

void Person::setFirstName(const QString &firstName)
{
    m_firstName = firstName;
}

QString Person::lastName() const
{
    return m_lastName;
}

void Person::setLastName(const QString &lastName)
{
    m_lastName = lastName;
}

QString Person::address() const
{
    return m_address;
}

void Person::setAddress(const QString &address)
{
    m_address = address;
}

QString Person::fullName() const
{
    return QString("%1 %2").arg(m_firstName, m_lastName);
}

QString Person::zip() const
{
    return m_zip;
}

void Person::setZip(const QString &zip)
{
    m_zip = zip;
}

QString Person::city() const
{
    return m_city;
}

void Person::setCity(const QString &city)
{
    m_city = city;
}

QString Person::phoneNumber() const
{
    return m_phoneNumber;
}

void Person::setPhoneNumber(const QString &phoneNumber)
{
    m_phoneNumber = phoneNumber;
}

QString Person::faxNumber() const
{
    return m_faxNumber;
}

void Person::setFaxNumber(const QString &faxNumber)
{
    m_faxNumber = faxNumber;
}

QString Person::email() const
{
    return m_email;
}

void Person::setEmail(const QString &email)
{
    m_email = email;
}
