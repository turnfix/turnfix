#include "athletedialog.h"
#include "model/entity/athlete.h"
#include "model/entity/club.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/athleterepository.h"
#include "model/repository/clubrepository.h"
#include "model/view/clubmodel.h"
#include "ui_athletedialog.h"
#include <QMessageBox>

#include <QDebug>

AthleteDialog::AthleteDialog(Athlete *athlete, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AthleteDialog)
    , m_em(em)
    , m_athlete(athlete)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    ui->cmb_sex->addItem(tr("weiblich"), Athlete::Gender::Female);
    ui->cmb_sex->addItem("männlich", Athlete::Gender::Male);

    m_clubModel = new ClubModel(m_em, this);
    m_clubModel->fetchClubs();
    ui->cmb_club->setModel(m_clubModel);

    connect(ui->but_save, &QPushButton::clicked, this, &AthleteDialog::save);
    connect(ui->chk_dat, &QCheckBox::stateChanged, this, &AthleteDialog::changeDat);

    if (m_athlete == nullptr) {
        m_athlete = new Athlete();
    }

    qDebug() << m_athlete->gender();

    ui->firstNameInput->setText(m_athlete->firstName());
    ui->lastnameInput->setText(m_athlete->lastName());
    ui->dae_year->setDate(m_athlete->dateOfBirth());
    ui->cmb_club->setCurrentIndex(ui->cmb_club->findData(m_athlete->clubId(), TF::IdRole));
    ui->cmb_sex->setCurrentIndex(m_athlete->gender());
    ui->txt_id->setText(m_athlete->license());
    ui->chk_dat->setChecked(!m_athlete->yearOfBirthOnly());
}

AthleteDialog::AthleteDialog(Athlete *athlete, Club *club, EntityManager *em, QWidget *parent)
    : AthleteDialog(athlete, em, parent)
{
    ui->cmb_club->setEditText(club->name());
}

AthleteDialog::~AthleteDialog()
{
    delete ui;
}

void AthleteDialog::changeDat()
{
    if (ui->chk_dat->isChecked()) {
        ui->dae_year->setDisplayFormat("dd.MM.yyyy");
        ui->dae_year->setCalendarPopup(true);
    } else {
        ui->dae_year->setDisplayFormat("yyyy");
        ui->dae_year->setCalendarPopup(false);
    }
}

void AthleteDialog::save()
{
    if (ui->cmb_club->currentIndex() == -1) {
        QMessageBox::warning(this, tr("Fehlender Wert"), tr("Bitte einen Verein wählen."));
        return;
    }

    m_athlete->setFirstName(ui->firstNameInput->text());
    m_athlete->setLastName(ui->lastnameInput->text());
    m_athlete->setDateOfBirth(ui->dae_year->date());
    m_athlete->setYearOfBirthOnly(!ui->chk_dat->isChecked());
    m_athlete->setClub(qvariant_cast<Club *>(ui->cmb_club->currentData()));
    m_athlete->setGender(ui->cmb_sex->currentData().toInt());
    m_athlete->setLicense(ui->txt_id->text());

    m_em->athleteRepository()->persist(m_athlete);
    done(1);
}
