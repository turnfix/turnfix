#ifndef BANKACCOUNTREPOSITORY_H
#define BANKACCOUNTREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/bankaccount.h"

class BankAccountRepository : public AbstractRepository<BankAccount>
{
public:
    explicit BankAccountRepository(EntityManager *em);

    QList<BankAccount *> loadAll();
};

#endif // BANKACCOUNTREPOSITORY_H
