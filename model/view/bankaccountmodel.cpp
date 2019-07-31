#include "bankaccountmodel.h"
#include "model/entity/bankaccount.h"
#include "model/entitymanager.h"
#include "model/repository/bankaccountrepository.h"

BankAccountModel::BankAccountModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{}

int BankAccountModel::rowCount(const QModelIndex &) const
{
    return m_accounts.size();
}

int BankAccountModel::columnCount(const QModelIndex &) const
{
    return 5;
}

QVariant BankAccountModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Nummer");
        case 2:
            return tr("Bankleitzahl");
        case 3:
            return tr("Bank");
        case 4:
            return tr("Inhaber");
        }
    }
    return QVariant();
}

QVariant BankAccountModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    BankAccount *bankAccount = m_accounts.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return bankAccount->name();
        case 1:
            return bankAccount->number();
        case 2:
            return bankAccount->routingCode();
        case 3:
            return bankAccount->institution();
        case 4:
            return bankAccount->accountHolder();
        }
    } else if (role == Qt::UserRole) {
        return QVariant::fromValue(bankAccount);
    }
    return QVariant();
}

void BankAccountModel::fetchAccounts()
{
    beginResetModel();
    m_accounts = m_em->bankAccountRepository()->loadAll();
    endResetModel();
}
