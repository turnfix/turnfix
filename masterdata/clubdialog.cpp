#include "clubdialog.h"
#include "model/entity/club.h"
#include "model/entity/person.h"
#include "model/entitymanager.h"
#include "model/repository/clubrepository.h"
#include "model/view/personmodel.h"
#include "persondialog.h"
#include "regiondialog.h"
#include "ui_clubdialog.h"
#include <QSqlQuery>

ClubDialog::ClubDialog(Club *club, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ClubDialog)
    , m_em(em)
    , m_club(club)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    readGaue();
    ui->cmb_gau->setCurrentIndex(0);

    m_personModel = new PersonModel(em, this);
    m_personModel->fetchPersons();
    ui->cmb_ansprech->setModel(m_personModel);
    ui->cmb_ansprech->setModelColumn(8);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->sli_ort, SIGNAL(valueChanged(int)), this, SLOT(updateOrt()));
    connect(ui->txt_name, SIGNAL(textChanged(QString)), this, SLOT(updateSlider()));
    connect(ui->but_addp, SIGNAL(clicked()), this, SLOT(addPerson()));
    connect(ui->but_addg, SIGNAL(clicked()), this, SLOT(addGau()));
    connect(ui->cmb_gau, SIGNAL(currentIndexChanged(int)), this, SLOT(updateGauInfo()));

    if (m_club == nullptr) {
        m_club = new Club();
    }

    ui->cmb_ansprech->setCurrentIndex(ui->cmb_ansprech->findData(m_club->contactPersonId()));
    ui->txt_name->setText(m_club->name());
    ui->txt_web->setText(m_club->website());
    ui->sli_ort->setValue(m_club->posCity());
    ui->cmb_gau->setCurrentIndex(ui->cmb_gau->findData(m_club->regionId()));
}

ClubDialog::~ClubDialog()
{
    delete ui;
}

void ClubDialog::addPerson()
{
    PersonDialog *personDialog = new PersonDialog(nullptr, m_em, this);
    if (personDialog->exec() == 1) {
        m_personModel->fetchPersons();
        ui->cmb_ansprech->setCurrentIndex(ui->cmb_ansprech->findData(personDialog->person()->id()));
    }
}

void ClubDialog::readGaue()
{
    QString currtext = ui->cmb_gau->currentText();
    ui->cmb_gau->clear();
    QSqlQuery query("SELECT int_gaueid, var_name FROM tfx_gaue ORDER BY var_name");
    while (query.next()) {
        ui->cmb_gau->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    ui->cmb_gau->setCurrentIndex(ui->cmb_gau->findText(currtext));
}

void ClubDialog::addGau()
{
    RegionDialog *pe = new RegionDialog(0, this);
    if(pe->exec() == 1) { readGaue(); }
}

void ClubDialog::save()
{
    m_club->setContactPersonId(ui->cmb_ansprech->itemData(ui->cmb_ansprech->currentIndex()).toInt());
    m_club->setName(ui->txt_name->text());
    m_club->setPosCity(ui->sli_ort->value());
    m_club->setWebsite(ui->txt_web->text());
    m_club->setRegionId(ui->cmb_gau->itemData(ui->cmb_gau->currentIndex()).toInt());

    m_em->clubRepository()->persist(m_club);
    done(1);
}

void ClubDialog::updateSlider()
{
    ui->sli_ort->setMaximum(ui->txt_name->text().length() - 1);
    updateOrt();
}

void ClubDialog::updateOrt()
{
    ui->txt_pre_ort->setText(
        ui->txt_name->text().right(ui->txt_name->text().length() - ui->sli_ort->value()));
}

void ClubDialog::updateGauInfo()
{
    if (ui->cmb_gau->itemData(ui->cmb_gau->currentIndex()).toInt() > 0) {
        QSqlQuery query;
        query.prepare("SELECT tfx_verbaende.var_name, tfx_laender.var_name FROM tfx_gaue INNER JOIN tfx_verbaende ON tfx_gaue.int_verbaendeid = tfx_verbaende.int_verbaendeid INNER JOIN tfx_laender ON tfx_verbaende.int_laenderid = tfx_laender.int_laenderid WHERE int_gaueid=?");
        query.bindValue(0, ui->cmb_gau->itemData(ui->cmb_gau->currentIndex()));
        query.exec();
        query.next();
        ui->lbl_verband->setText(query.value(0).toString());
        ui->lbl_land->setText(query.value(1).toString());
    }
}

Club *ClubDialog::club()
{
    return m_club;
}
