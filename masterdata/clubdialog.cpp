#include "clubdialog.h"
#include "model/entity/club.h"
#include "model/entity/person.h"
#include "model/entitymanager.h"
#include "model/repository/clubrepository.h"
#include "model/view/personmodel.h"
#include "model/view/regionmodel.h"
#include "persondialog.h"
#include "regiondialog.h"
#include "ui_clubdialog.h"
#include <QMessageBox>

ClubDialog::ClubDialog(Club *club, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ClubDialog)
    , m_em(em)
    , m_club(club)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    m_personModel = new PersonModel(em, this);
    m_personModel->fetchPersons();
    ui->cmb_ansprech->setModel(m_personModel);
    ui->cmb_ansprech->setModelColumn(8);

    m_regionModel = new RegionModel(em, this);
    m_regionModel->fetchRegions();
    ui->cmb_gau->setModel(m_regionModel);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->sli_ort, SIGNAL(valueChanged(int)), this, SLOT(updateCity()));
    connect(ui->txt_name, SIGNAL(textChanged(QString)), this, SLOT(updateSlider()));
    connect(ui->but_addp, SIGNAL(clicked()), this, SLOT(addPerson()));
    connect(ui->but_addg, SIGNAL(clicked()), this, SLOT(addRegion()));
    connect(ui->cmb_gau, SIGNAL(currentIndexChanged(int)), this, SLOT(updateStateAndCountry()));

    if (m_club == nullptr) {
        m_club = new Club();
    }

    ui->cmb_ansprech->setCurrentIndex(
        ui->cmb_ansprech->findData(m_club->contactPersonId(), TF::IdRole));
    ui->txt_name->setText(m_club->name());
    ui->txt_web->setText(m_club->website());
    ui->sli_ort->setValue(m_club->posCity());
    ui->cmb_gau->setCurrentIndex(ui->cmb_gau->findData(m_club->regionId(), TF::IdRole));
    updateStateAndCountry();
}

ClubDialog::~ClubDialog()
{
    delete ui;
}

void ClubDialog::addPerson()
{
    auto personDialog = new PersonDialog(nullptr, m_em, this);
    if (personDialog->exec() == 1) {
        m_personModel->fetchPersons();
        ui->cmb_ansprech->setCurrentIndex(ui->cmb_ansprech->findData(personDialog->person()->id()));
    }
}

void ClubDialog::addRegion()
{
    auto pe = new RegionDialog(nullptr, m_em, this);
    if (pe->exec() == 1) {
        m_regionModel->fetchRegions();
    }
}

void ClubDialog::save()
{
    if (ui->cmb_gau->currentIndex() == -1) {
        QMessageBox::warning(this, tr("Fehlender Wert"), tr("Bitte einen Gau/Turnkreis wählen."));
        return;
    }

    m_club->setContactPerson(qvariant_cast<Person *>(ui->cmb_ansprech->currentData()));
    m_club->setName(ui->txt_name->text());
    m_club->setPosCity(ui->sli_ort->value());
    m_club->setWebsite(ui->txt_web->text());
    m_club->setRegion(qvariant_cast<Region *>(ui->cmb_gau->currentData()));

    m_em->clubRepository()->persist(m_club);
    done(1);
}

void ClubDialog::updateSlider()
{
    ui->sli_ort->setMaximum(ui->txt_name->text().length() - 1);
    updateCity();
}

void ClubDialog::updateCity()
{
    ui->txt_pre_ort->setText(
        ui->txt_name->text().right(ui->txt_name->text().length() - ui->sli_ort->value()));
}

void ClubDialog::updateStateAndCountry()
{
    QVariant data = ui->cmb_gau->currentData();
    if (data.isValid()) {
        auto region = qvariant_cast<Region *>(data);
        ui->lbl_verband->setText(region->state()->name());
        ui->lbl_land->setText(region->state()->country()->name());
    }
}

Club *ClubDialog::club()
{
    return m_club;
}
