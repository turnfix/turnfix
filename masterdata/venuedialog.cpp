#include "venuedialog.h"
#include "model/entity/venue.h"
#include "model/entitymanager.h"
#include "model/repository/venuerepository.h"
#include "ui_venuedialog.h"

VenueDialog::VenueDialog(Venue *venue, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VenueDialog)
    , m_em(em)
    , m_venue(venue)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_save, &QPushButton::clicked, this, &VenueDialog::save);

    if (m_venue == nullptr) {
        m_venue = new Venue();
    }

    ui->txt_name->setText(m_venue->name());
    ui->txt_adress->setText(m_venue->address());
    ui->txt_plz->setText(m_venue->zip());
    ui->txt_ort->setText(m_venue->city());
}

VenueDialog::~VenueDialog()
{
    delete ui;
}

Venue *VenueDialog::venue()
{
    return m_venue;
}

void VenueDialog::save()
{
    m_venue->setName(ui->txt_name->text());
    m_venue->setAddress(ui->txt_adress->text());
    m_venue->setZip(ui->txt_plz->text());
    m_venue->setCity(ui->txt_ort->text());

    m_em->venueRepository()->persist(m_venue);
    done(1);
}
