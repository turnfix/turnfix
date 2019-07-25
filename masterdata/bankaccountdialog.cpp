#include "bankaccountdialog.h"
#include "model/entity/bankaccount.h"
#include "model/entitymanager.h"
#include "model/repository/bankaccountrepository.h"
#include "ui_bankaccountdialog.h"

BankAccountDialog::BankAccountDialog(BankAccount *account, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BankAccountDialog)
    , m_em(em)
    , m_account(account)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));

    if (m_account == nullptr) {
        m_account = new BankAccount();
    }

    ui->txt_name->setText(m_account->name());
    ui->txt_number->setText(m_account->number());
    ui->txt_blz->setText(m_account->routingCode());
    ui->txt_bank->setText(m_account->institution());
    ui->txt_owner->setText(m_account->accountHolder());
}

BankAccountDialog::~BankAccountDialog()
{
    delete ui;
}

BankAccount *BankAccountDialog::account() const
{
    return m_account;
}

void BankAccountDialog::save()
{
    m_account->setName(ui->txt_name->text());
    m_account->setNumber(ui->txt_number->text());
    m_account->setRoutingCode(ui->txt_blz->text());
    m_account->setInstitution(ui->txt_bank->text());
    m_account->setAccountHolder(ui->txt_owner->text());

    m_em->bankAccountRepository()->persist(m_account);
    done(1);
}
