#include "regiondialog.h"
#include "model/entity/region.h"
#include "model/entity/state.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/regionrepository.h"
#include "model/view/statemodel.h"
#include "statedialog.h"
#include "ui_regiondialog.h"
#include <QMessageBox>

RegionDialog::RegionDialog(Region *region, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegionDialog)
    , m_em(em)
    , m_region(region)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_addl, SIGNAL(clicked()), this, SLOT(addState()));

    if (m_region == nullptr) {
        m_region = new Region();
    }

    auto stateModel = new StateModel(m_em, this);
    stateModel->fetchStates();
    ui->cmb_verband->setModel(stateModel);

    ui->cmb_verband->setCurrentIndex(ui->cmb_verband->findData(m_region->stateId(), TF::IdRole));
    ui->txt_name->setText(m_region->name());
    ui->txt_kuerzel->setText(m_region->code());
}

RegionDialog::~RegionDialog()
{
    delete ui;
}

void RegionDialog::addState()
{
    auto pe = new StateDialog(nullptr, m_em, this);
    if (pe->exec() == 1) {
        m_stateModel->fetchStates();
    }
}

void RegionDialog::save()
{
    if (ui->cmb_verband->currentIndex() == -1) {
        QMessageBox::warning(this, tr("Fehlender Wert"), tr("Bitte einen Verband wählen."));
        return;
    }

    m_region->setState(qvariant_cast<State *>(ui->cmb_verband->currentData()));
    m_region->setName(ui->txt_name->text());
    m_region->setCode(ui->txt_kuerzel->text());

    m_em->regionRepository()->persist(m_region);
    done(1);
}
