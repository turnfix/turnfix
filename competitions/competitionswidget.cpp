#include "competitionswidget.h"
#include "competitiondialog.h"
#include "model/entity/event.h"
#include "model/view/competitionmodel.h"
#include "src/global/header/_global.h"
#include "ui_competitionswidget.h"
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QSqlQueryModel>

CompetitionsWidget::CompetitionsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CompetitionsWidget)
{
    ui->setupUi(this);

    connect(ui->but_addWK, &QPushButton::clicked, this, &CompetitionsWidget::addCompetition);
    connect(ui->but_editWK, &QPushButton::clicked, this, &CompetitionsWidget::editCompetition);
    connect(ui->but_delWK, &QPushButton::clicked, this, &CompetitionsWidget::removeCompetition);
    connect(ui->cmb_filterWK,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &CompetitionsWidget::updateFilterColumn);
    connect(ui->txt_filterWK, &QLineEdit::textChanged, this, &CompetitionsWidget::updateFilterText);
    connect(ui->competitionsTable,
            &QTableView::doubleClicked,
            this,
            &CompetitionsWidget::editCompetition);
}

CompetitionsWidget::~CompetitionsWidget()
{
    delete ui;
}

void CompetitionsWidget::addCompetition()
{
    CompetitionDialog *sw = new CompetitionDialog(m_event, 0, this);
    if (sw->exec() == 1) {
        m_model->fetchCompetitions();
    }
    _global::updateRgDis(m_event);
    ui->competitionsTable->setFocus();
}

void CompetitionsWidget::editCompetition()
{
    if (ui->competitionsTable->currentIndex().isValid()) {
        QModelIndex idx = ui->competitionsTable->currentIndex();
        CompetitionDialog *sw = new CompetitionDialog(
            m_event,
            QVariant(m_sortModel->data(
                         m_sortModel->index(ui->competitionsTable->currentIndex().row(), 7)))
                .toInt(),
            this);
        if (sw->exec() == 1) {
            m_model->fetchCompetitions();
        }
        _global::updateRgDis(m_event);
        ui->competitionsTable->setCurrentIndex(idx);
        ui->competitionsTable->setFocus();
    }
}

void CompetitionsWidget::removeCompetition()
{
    if (ui->competitionsTable->currentIndex().isValid()) {
        QMessageBox msg(QMessageBox::Question,
                        "Wettkampf löschen",
                        "Wollen sie diesen Wettkampf wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            QSqlQuery query;
            query.prepare("DELETE FROM tfx_wettkaempfe WHERE int_wettkaempfeid=?");
            query.bindValue(0,
                            QVariant(
                                m_sortModel->data(
                                    m_sortModel->index(ui->competitionsTable->currentIndex().row(),
                                                       7)))
                                .toInt());
            query.exec();
            m_model->fetchCompetitions();
        }
        _global::updateRgDis(m_event);
        ui->competitionsTable->setFocus();
    }
}

void CompetitionsWidget::updateFilterColumn(int index)
{
    m_sortModel->setFilterKeyColumn(index);
}

void CompetitionsWidget::updateFilterText(const QString &text)
{
    m_sortModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive, QRegExp::Wildcard));
}

void CompetitionsWidget::setup(Event *event, EntityManager *em)
{
    m_em = em;
    m_event = event;
    m_model = new CompetitionModel(event, em);
    m_model->fetchCompetitions();
    m_sortModel = new QSortFilterProxyModel();
    m_sortModel->setSourceModel(m_model);

    ui->competitionsTable->setModel(m_sortModel);

    QList<QHeaderView::ResizeMode> resizeModes = {QHeaderView::ResizeToContents,
                                                  QHeaderView::Stretch,
                                                  QHeaderView::ResizeToContents,
                                                  QHeaderView::ResizeToContents,
                                                  QHeaderView::ResizeToContents,
                                                  QHeaderView::ResizeToContents,
                                                  QHeaderView::ResizeToContents};

    ui->cmb_filterWK->clear();

    for (int i = 0; i < 7; i++) {
        ui->cmb_filterWK->addItem(m_model->headerData(i, Qt::Horizontal).toString());
        ui->competitionsTable->horizontalHeader()->setSectionResizeMode(i, resizeModes.at(i));
    }
}
