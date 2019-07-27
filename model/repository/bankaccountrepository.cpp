#include "bankaccountrepository.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"
#include "model/entity/bankaccount.h"
#include "model/entitymanager.h"
#include <QMetaProperty>
#include <QSqlQuery>

BankAccountRepository::BankAccountRepository(EntityManager *em)
    : AbstractRepository<BankAccount>(em)
{}

QList<BankAccount *> BankAccountRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());
    QList<BankAccount *> output;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM tfx_konten ORDER BY var_name DESC");
    query.exec();

    while (query.next()) {
        BankAccount *account = new BankAccount();
        const QMetaObject *metaObj = account->metaObject();
        const DBTable *mappingObj = account->mapping();

        for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
            QMetaProperty property = metaObj->property(i);
            DBColumn *column = mappingObj->columnByProperty(property.name());

            account->setProperty(property.name(), query.value(column->name()));
        }

        output.append(account);
    }

    return output;
}

void BankAccountRepository::persist(BankAccount *account)
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());
    QSqlQuery query(db);

    if (account->id() == 0) {
        query.prepare("INSERT INTO tfx_konten "
                      "(var_name,var_kontonummer,var_blz,var_bank,var_inhabe) VALUES (?,?,?,?,?)");
    } else {
        query.prepare("UPDATE tfx_konten SET var_name=?, var_kontonummer=?, var_blz=?, var_bank=?, "
                      "var_inhabe=? WHERE int_kontenid=?");
        query.bindValue(5, account->id());
    }
    query.bindValue(0, account->name());
    query.bindValue(1, account->number());
    query.bindValue(2, account->routingCode());
    query.bindValue(3, account->institution());
    query.bindValue(4, account->accountHolder());
    query.exec();

    if (account->id() == 0) {
        account->setId(query.lastInsertId().toInt());
    }
}

void BankAccountRepository::remove(BankAccount *account) {}
