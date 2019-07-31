#include "penaltydialog.h"
#include "model/entity/penalty.h"
#include "model/entitymanager.h"
#include "model/repository/penaltyrepository.h"
#include "ui_penaltydialog.h"

PenaltyDialog::PenaltyDialog(Penalty *penalty, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PenaltyDialog)
    , m_em(em)
    , m_penalty(penalty)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));

    if (m_penalty == nullptr) {
        m_penalty = new Penalty();
    }

    ui->txt_name->setText(m_penalty->name());
    ui->dsb_abzug->setValue(m_penalty->deduction());
}

PenaltyDialog::~PenaltyDialog()
{
    delete ui;
}

void PenaltyDialog::save()
{
    m_penalty->setName(ui->txt_name->text());
    m_penalty->setDeduction(ui->dsb_abzug->value());

    m_em->penaltyRepository()->persist(m_penalty);
    done(1);
}
