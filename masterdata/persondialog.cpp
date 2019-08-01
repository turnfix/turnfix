#include "persondialog.h"
#include "model/entity/person.h"
#include "model/entitymanager.h"
#include "model/repository/personrepository.h"
#include "ui_persondialog.h"

PersonDialog::PersonDialog(Person *person, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PersonDialog)
    , m_em(em)
    , m_person(person)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_save, &QPushButton::clicked, this, &PersonDialog::save);

    if (m_person == nullptr) {
        m_person = new Person();
    }

    ui->firstNameText->setText(m_person->firstName());
    ui->lastNameText->setText(m_person->lastName());
    ui->addressText->setText(m_person->address());
    ui->zipText->setText(m_person->zip());
    ui->cityText->setText(m_person->city());
    ui->phoneNumberText->setText(m_person->phoneNumber());
    ui->faxText->setText(m_person->faxNumber());
    ui->emailText->setText(m_person->email());
}

PersonDialog::~PersonDialog()
{
    delete ui;
}

void PersonDialog::save()
{
    m_person->setFirstName(ui->firstNameText->text());
    m_person->setLastName(ui->lastNameText->text());
    m_person->setAddress(ui->addressText->text());
    m_person->setZip(ui->zipText->text());
    m_person->setCity(ui->cityText->text());
    m_person->setPhoneNumber(ui->phoneNumberText->text());
    m_person->setFaxNumber(ui->faxText->text());
    m_person->setEmail(ui->emailText->text());

    m_em->personRepository()->persist(m_person);
    done(1);
}

Person *PersonDialog::person()
{
    return m_person;
}
