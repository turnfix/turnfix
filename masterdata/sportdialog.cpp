#include "sportdialog.h"
#include "model/entity/sport.h"
#include "model/entitymanager.h"
#include "model/repository/sportrepository.h"
#include "ui_sportdialog.h"
#include <QSqlQuery>

SportDialog::SportDialog(Sport *sport, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SportDialog)
    , m_em(em)
    , m_sport(sport)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    connect(ui->but_save, &QPushButton::clicked, this, &SportDialog::save);

    if (m_sport == nullptr) {
        m_sport = new Sport();
    }

    ui->txt_name->setText(m_sport->name());
}

SportDialog::~SportDialog()
{
    delete ui;
}

Sport *SportDialog::sport()
{
    return m_sport;
}

void SportDialog::save()
{
    m_sport->setName(ui->txt_name->text());

    m_em->sportRepository()->persist(m_sport);
    done(1);
}
