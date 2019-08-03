#ifndef BANKACCOUNTMODEL_H
#define BANKACCOUNTMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class BankAccount;

class BankAccountModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BankAccountModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void fetchAccounts();

private:
    QList<BankAccount *> m_accounts;
    EntityManager *m_em;
};

#endif // BANKACCOUNTMODEL_H
