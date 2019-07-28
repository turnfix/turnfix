#include "eventdialog.h"
#include "masterdata/bankaccountdialog.h"
#include "masterdata/persondialog.h"
#include "masterdata/venuedialog.h"
#include "model/entity/bankaccount.h"
#include "model/entity/event.h"
#include "model/entity/person.h"
#include "model/entity/venue.h"
#include "model/entitymanager.h"
#include "model/repository/eventrepository.h"
#include "model/view/bankaccountmodel.h"
#include "model/view/eventmodel.h"
#include "model/view/personmodel.h"
#include "model/view/venuemodel.h"
#include "ui_eventdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QToolBar>

EventDialog::EventDialog(Event *event, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EventDialog)
    , m_event(event)
    , m_em(em)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    auto *tb = new QToolBar();
    auto *ag = new QActionGroup(this);
    tb->setAllowedAreas(Qt::LeftToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setOrientation(Qt::Vertical);
    tb->addAction(ui->act_event);
    ag->addAction(ui->act_event);
    tb->addAction(ui->act_aus);
    ag->addAction(ui->act_aus);
    tb->addAction(ui->act_round);
    ag->addAction(ui->act_round);
    ui->act_event->setChecked(true);
    ui->sidebar->layout()->addWidget(tb);

    connect(ui->act_event, &QAction::triggered, [this]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->act_aus, &QAction::triggered, [this]() { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->act_round, &QAction::triggered, [this]() { ui->stackedWidget->setCurrentIndex(2); });

    ui->dae_from->setDate(QDate::currentDate());
    ui->dae_to->setDate(QDate::currentDate());
    ui->dae_ms->setDate(QDate::currentDate());

    auto *eventModel = new EventModel(em, this);
    ui->cmb_mainround->setModel(eventModel);

    m_venueModel = new VenueModel(em, this);
    m_venueModel->fetchVenues();
    ui->cmb_locations->setModel(m_venueModel);
    ui->cmb_locations->setModelColumn(3);

    m_accountModel = new BankAccountModel(em, this);
    m_accountModel->fetchAccounts();
    ui->cmb_account->setModel(m_accountModel);
    ui->cmb_account->setModelColumn(0);

    m_personModel = new PersonModel(em, this);
    m_personModel->fetchPersons();
    ui->cmb_eventContact->setModel(m_personModel);
    ui->cmb_eventContact->setModelColumn(8);
    ui->cmb_registrationContact->setModel(m_personModel);
    ui->cmb_registrationContact->setModelColumn(8);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_addp, SIGNAL(clicked()), this, SLOT(addEventContact()));
    connect(ui->but_addp2, SIGNAL(clicked()), this, SLOT(addRegistrationContact()));
    connect(ui->but_addort, SIGNAL(clicked()), this, SLOT(addLocation()));
    connect(ui->but_addacc, SIGNAL(clicked()), this, SLOT(addAccount()));

    ui->txt_event->setText(m_event->name());
    ui->dae_from->setDate(m_event->startDate());
    ui->dae_to->setDate(m_event->endDate());
    ui->dae_ms->setDate(m_event->registrationDeadline());
    ui->cmb_locations->setCurrentIndex(ui->cmb_locations->findData(m_event->venueId()));
    ui->txt_orga->setText(m_event->organizer());
    ui->cmb_eventContact->setCurrentIndex(ui->cmb_eventContact->findData(m_event->eventContactId()));
    ui->cmb_registrationContact->setCurrentIndex(ui->cmb_registrationContact->findData(m_event->registrationContactId()));
    ui->txt_web->setText(m_event->website());
    ui->sbx_edv->setValue(m_event->itTeamCount());
    ui->sbx_ref->setValue(m_event->judgesCount());
    ui->sbx_volu->setValue(m_event->volunteersCount());
    ui->dsb_money->setValue(m_event->registrationFee());
    ui->chk_nostart->setChecked(m_event->chargeNoShow());
    ui->chk_rereg->setChecked(m_event->changesAllowed());
    ui->chk_postreg->setChecked(m_event->lateRegistrationAllowed());
    ui->dsb_postmoney->setValue(m_event->lateFee());
    ui->txt_zweck->setText(m_event->reference());
    ui->txt_regtext->setText(m_event->registrationNotes());
    ui->txt_startproof->setText(m_event->licenseRequirements());
    ui->txt_requirements->setText(m_event->entryRequirements());
    ui->txt_certificate->setText(m_event->awards());
    ui->txt_ref->setText(m_event->judges());
    ui->txt_misc->setText(m_event->notes());
    ui->cmb_account->setCurrentIndex(ui->cmb_account->findData(m_event->bankAccountId()));
    ui->sbx_round->setValue(m_event->round());
    ui->cmb_mainround->setCurrentIndex(ui->cmb_mainround->findData(m_event->mainEventId()));
    ui->gbx_round->setChecked(m_event->multiRoundEvent());
}

EventDialog::~EventDialog()
{
    delete ui;
}

void EventDialog::addLocation()
{
    VenueDialog *venueDialog = new VenueDialog(nullptr, m_em, this);
    if (venueDialog->exec() == 1) {
        m_venueModel->fetchVenues();
        ui->cmb_locations->setCurrentIndex(
            ui->cmb_locations->findData(QVariant::fromValue(venueDialog->venue())));
    }
}

void EventDialog::addEventContact()
{
    PersonDialog *personDialog = new PersonDialog(nullptr, m_em, this);
    if (personDialog->exec() == 1) {
        m_personModel->fetchPersons();
        ui->cmb_eventContact->setCurrentIndex(
            ui->cmb_eventContact->findData(QVariant::fromValue(personDialog->person())));
    }
}

void EventDialog::addRegistrationContact()
{
    PersonDialog *personDialog = new PersonDialog(nullptr, m_em, this);
    if (personDialog->exec() == 1) {
        m_personModel->fetchPersons();
        ui->cmb_registrationContact->setCurrentIndex(
            ui->cmb_registrationContact->findData(QVariant::fromValue(personDialog->person())));
    }
}

void EventDialog::addAccount()
{
    BankAccountDialog *accountDialog = new BankAccountDialog(nullptr, m_em, this);
    if (accountDialog->exec() == 1) {
        m_accountModel->fetchAccounts();
        ui->cmb_account->setCurrentIndex(
            ui->cmb_account->findData(QVariant::fromValue(accountDialog->account())));
    }
}

void EventDialog::save()
{
    if (ui->cmb_locations->currentText().length() == 0) {
        QMessageBox msg(QMessageBox::Warning,
                        tr("Keine Wettkampfort gewählt!"),
                        tr("Bitte wählen sie einen Wettkampfort aus!"));
        msg.exec();
    } else {
        m_event->setVenue(qvariant_cast<Venue *>(ui->cmb_locations->currentData()));
        m_event->setEventContact(qvariant_cast<Person *>(ui->cmb_eventContact->currentData()));
        m_event->setRegistrationContact(qvariant_cast<Person *>(ui->cmb_registrationContact->currentData()));
        m_event->setBankAccount(qvariant_cast<BankAccount *>(ui->cmb_account->currentData()));
        m_event->setMainEvent(qvariant_cast<Event *>(ui->cmb_mainround->currentData()));
        m_event->setName(ui->txt_event->text());
        m_event->setRound(ui->sbx_round->value());
        m_event->setStartDate(ui->dae_from->date());
        m_event->setEndDate(ui->dae_to->date());
        m_event->setRegistrationDeadline(ui->dae_ms->date());
        m_event->setMultiRoundEvent(ui->gbx_round->isChecked());
        m_event->setOrganizer(ui->txt_orga->text());
        m_event->setItTeamCount(ui->sbx_edv->value());
        m_event->setVolunteersCount(ui->sbx_volu->value());
        m_event->setJudgesCount(ui->sbx_ref->value());
        m_event->setWebsite(ui->txt_web->text());
        m_event->setReference(ui->txt_zweck->text());
        m_event->setRegistrationFee(ui->dsb_money->value());
        m_event->setLateFee(ui->dsb_postmoney->value());
        m_event->setChargeNoShow(ui->chk_nostart->isChecked());
        m_event->setChangesAllowed(ui->chk_rereg->isChecked());
        m_event->setLateRegistrationAllowed(ui->chk_postreg->isChecked());
        m_event->setRegistrationNotes(ui->txt_regtext->toPlainText());
        m_event->setLicenseRequirements(ui->txt_startproof->toPlainText());
        m_event->setEntryRequirements(ui->txt_requirements->toPlainText());
        m_event->setAwards(ui->txt_certificate->toPlainText());
        m_event->setJudges(ui->txt_ref->toPlainText());
        m_event->setNotes(ui->txt_misc->toPlainText());

        m_em->eventRepository()->persist(m_event);
        done(1);
    }
}
