#include "countrydialog.h"
#include "model/entity/country.h"
#include "model/entitymanager.h"
#include "model/repository/countryrepository.h"
#include "ui_countrydialog.h"

CountryDialog::CountryDialog(Country *country, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CountryDialog)
    , m_em(em)
    , m_country(country)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));

    if (m_country == nullptr) {
        m_country = new Country();
    }

    ui->txt_land->setText(m_country->name());
    ui->txt_kuerzel->setText(m_country->code());
}

CountryDialog::~CountryDialog()
{
    delete ui;
}

void CountryDialog::save()
{
    m_country->setName(ui->txt_land->text());
    m_country->setCode(ui->txt_kuerzel->text());

    m_em->countryRepository()->persist(m_country);
    done(1);
}

Country *CountryDialog::country()
{
    return m_country;
}
