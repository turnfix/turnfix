#include "statedialog.h"
#include "countrydialog.h"
#include "countrymodel.h"
#include "model/entity/country.h"
#include "model/entity/state.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/staterepository.h"
#include "ui_statedialog.h"
#include <QMessageBox>

StateDialog::StateDialog(State *state, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StateDialog)
    , m_em(em)
    , m_state(state)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    m_countryModel = new CountryModel(m_em, this);
    m_countryModel->fetchCountries();
    ui->cmb_land->setModel(m_countryModel);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_addl, SIGNAL(clicked()), this, SLOT(addCountry()));

    if (m_state == nullptr) {
        m_state = new State();
    }

    ui->cmb_land->setCurrentIndex(ui->cmb_land->findData(m_state->countryId(), TF::IdRole));
    ui->txt_name->setText(m_state->name());
    ui->txt_kuerzel->setText(m_state->code());
}

StateDialog::~StateDialog()
{
    delete ui;
}

void StateDialog::addCountry()
{
    auto pe = new CountryDialog(nullptr, m_em, this);
    if (pe->exec() == 1) {
        m_countryModel->fetchCountries();
    }
}

void StateDialog::save()
{
    if (ui->cmb_land->currentIndex() == -1) {
        QMessageBox::warning(this, tr("Fehlender Wert"), tr("Bitte ein Land wählen."));
        return;
    }

    m_state->setCountry(qvariant_cast<Country *>(ui->cmb_land->currentData()));
    m_state->setName(ui->txt_name->text());
    m_state->setCode(ui->txt_kuerzel->text());

    m_em->stateRepository()->persist(m_state);
    done(1);
}
