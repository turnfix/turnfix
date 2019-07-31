#ifndef BANKACCOUNTDIALOG_H
#define BANKACCOUNTDIALOG_H

#include <QDialog>

namespace Ui {
class BankAccountDialog;
}

class BankAccount;
class EntityManager;

class BankAccountDialog : public QDialog
{
    Q_OBJECT

public:
    BankAccountDialog(BankAccount *account, EntityManager *em, QWidget *parent = nullptr);
    ~BankAccountDialog() override;

    BankAccount *account() const;

private slots:
    void save();

private:
    Ui::BankAccountDialog *ui;
    EntityManager *m_em;
    BankAccount *m_account;
};
#endif
