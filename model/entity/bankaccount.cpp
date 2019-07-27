#include "bankaccount.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *BankAccount::initializeMapping()
{
    DBTable *account = new DBTable("tfx_konten");
    account->addColumn("id", "int_kontenid", ColumnType::Integer, 0, false, "", "", true);
    account->addColumn("name", "var_name", ColumnType::Varchar, 150);
    account->addColumn("number", "var_kontonummer", ColumnType::Varchar, 10);
    account->addColumn("routingCode", "var_blz", ColumnType::Varchar, 8);
    account->addColumn("institution", "var_bank", ColumnType::Varchar, 150);
    account->addColumn("accountHolder", "var_inhabe", ColumnType::Varchar, 150);

    return account;
}

const DBTable *BankAccount::m_mapping = BankAccount::initializeMapping();

const DBTable *BankAccount::mapping()
{
    return m_mapping;
}

int BankAccount::id() const
{
    return m_id;
}

void BankAccount::setId(int id)
{
    m_id = id;
}

QString BankAccount::name() const
{
    return m_name;
}

void BankAccount::setName(const QString &name)
{
    m_name = name;
}

QString BankAccount::number() const
{
    return m_number;
}

void BankAccount::setNumber(const QString &number)
{
    m_number = number;
}

QString BankAccount::routingCode() const
{
    return m_routingCode;
}

void BankAccount::setRoutingCode(const QString &routingCode)
{
    m_routingCode = routingCode;
}

QString BankAccount::institution() const
{
    return m_institution;
}

void BankAccount::setInstitution(const QString &institution)
{
    m_institution = institution;
}

QString BankAccount::accountHolder() const
{
    return m_accountHolder;
}

void BankAccount::setAccountHolder(const QString &accountHolder)
{
    m_accountHolder = accountHolder;
}
