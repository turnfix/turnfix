#include "statusdialog.h"
#include "model/entity/status.h"
#include "model/entitymanager.h"
#include "model/repository/statusrepository.h"
#include "ui_statusdialog.h"
#include <QColorDialog>

StatusDialog::StatusDialog(Status *status, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StatusDialog)
    , m_em(em)
    , m_status(status)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_color, SIGNAL(clicked()), this, SLOT(colorChooser()));

    if (m_status == nullptr) {
        m_status = new Status();
    }

    m_color = m_status->color();

    ui->txt_name->setText(m_status->name());
    ui->lbl_color->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                     .arg(m_color.red())
                                     .arg(m_color.green())
                                     .arg(m_color.blue()));
    ui->chk_karte->setChecked(m_status->scorecard());
    ui->chk_bogen->setChecked(m_status->scoresheet());
}

StatusDialog::~StatusDialog()
{
    delete ui;
}

void StatusDialog::colorChooser()
{
    QColorDialog colc(m_color, this);
    colc.exec();
    m_color = colc.selectedColor();
    ui->lbl_color->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                     .arg(m_color.red())
                                     .arg(m_color.green())
                                     .arg(m_color.blue()));
}

void StatusDialog::save()
{
    m_status->setName(ui->txt_name->text());
    m_status->setColor(m_color);
    m_status->setScorecard(ui->chk_karte->isChecked());
    m_status->setScoresheet(ui->chk_bogen->isChecked());

    m_em->statusRepository()->persist(m_status);
    done(1);
}
