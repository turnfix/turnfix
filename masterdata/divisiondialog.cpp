#include "divisiondialog.h"
#include "model/entity/division.h"
#include "model/entitymanager.h"
#include "model/repository/divisionrepository.h"
#include "ui_divisiondialog.h"
#include <QSqlQuery>

DivisionDialog::DivisionDialog(Division *division, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DivisionDialog)
    , m_em(em)
    , m_division(division)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_save, &QPushButton::clicked, this, &DivisionDialog::save);

    if (m_division == nullptr) {
        m_division = new Division();
    }

    ui->txt_name->setText(m_division->name());
    ui->chk_m->setChecked(m_division->men());
    ui->chk_w->setChecked(m_division->women());
}

DivisionDialog::~DivisionDialog()
{
    delete ui;
}

Division *DivisionDialog::division()
{
    return m_division;
}

void DivisionDialog::save()
{
    m_division->setName(ui->txt_name->text());
    m_division->setMen(ui->chk_m->isChecked());
    m_division->setWomen(ui->chk_w->isChecked());

    m_em->divisionRepository()->persist(m_division);
    done(1);
}
