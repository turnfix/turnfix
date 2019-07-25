#ifndef BANKACCOUNTREPOSITORY_H
#define BANKACCOUNTREPOSITORY_H

#include "abstractrepository.h"

class BankAccount;

class BankAccountRepository : public AbstractRepository<BankAccount>
{
public:
    explicit BankAccountRepository(EntityManager *em);

    QList<BankAccount *> loadAll();
    void persist(BankAccount *account);
    void remove(BankAccount *account);
};

#endif // BANKACCOUNTREPOSITORY_H
