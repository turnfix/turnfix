#include "bankaccountrepository.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"
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
        auto account = new BankAccount();
        const QMetaObject *metaObj = account->metaObject();
        const DBTable *mappingObj = BankAccount::mapping();

        for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
            QMetaProperty property = metaObj->property(i);
            DBColumn *column = mappingObj->columnByProperty(property.name());

            account->setProperty(property.name(), query.value(column->name()));
        }

        output.append(account);
    }

    return output;
}
