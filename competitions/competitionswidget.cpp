#include "competitionswidget.h"
#include "competitiondialog.h"
#include "model/objects/event.h"
#include "model/settings/session.h"
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

    this->event = Session::getInstance()->getEvent();
    this->wk_model = new QSqlQueryModel();
    this->wk_sort_model = new QSortFilterProxyModel();
    //wk_sort_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->wk_sort_model->setSourceModel(wk_model);

    ui->competitionsTable->setModel(wk_sort_model);

    connect(ui->but_addWK, SIGNAL(clicked()), this, SLOT(addWK()));
    connect(ui->but_editWK, SIGNAL(clicked()), this, SLOT(editWK()));
    connect(ui->but_delWK, SIGNAL(clicked()), this, SLOT(delWK()));
    connect(ui->cmb_filterWK,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(updateWKFilterColumn(int)));
    connect(ui->txt_filterWK, SIGNAL(textChanged(QString)), this, SLOT(updateWKFilterText(QString)));
    connect(ui->competitionsTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editWK()));
}

CompetitionsWidget::~CompetitionsWidget()
{
    delete ui;
}

void CompetitionsWidget::fillWKTable()
{
    int idx = ui->cmb_filterWK->currentIndex();
    QSqlQuery query;
    query.prepare(
        "SELECT var_nummer, tfx_wettkaempfe.var_name, tfx_bereiche.var_name, CASE WHEN yer_bis = 3 "
        "THEN 'offen' ELSE CAST(yer_von AS text) END, CASE WHEN yer_bis=1 THEN 'und älter' WHEN "
        "yer_bis=2 THEN 'und jünger' WHEN yer_bis=3 THEN 'offen' ELSE CAST(yer_bis AS text) END , "
        "int_qualifikation, (SELECT COUNT(*) FROM tfx_wertungen WHERE "
        "int_wettkaempfeid=tfx_wettkaempfe.int_wettkaempfeid AND int_runde=?) AS starter, "
        "int_wettkaempfeid FROM tfx_wettkaempfe INNER JOIN tfx_bereiche USING (int_bereicheid) "
        "WHERE int_veranstaltungenid=? ORDER BY var_nummer ASC");
    query.bindValue(0, this->event->getRound());
    query.bindValue(1, this->event->getMainEventId());
    query.exec();
    wk_model->setQuery(query);
    ui->cmb_filterWK->clear();
    QHeaderView::ResizeMode resizeModeWK[] = {QHeaderView::ResizeToContents,
                                              QHeaderView::Stretch,
                                              QHeaderView::ResizeToContents,
                                              QHeaderView::ResizeToContents,
                                              QHeaderView::ResizeToContents,
                                              QHeaderView::ResizeToContents,
                                              QHeaderView::ResizeToContents};
    QString headersWK[7] = {"#", "Bezeichnung", "Bereich", "von Jahr", "bis Jahr", "Q", "Starter"};
    for (int i = 0; i < 7; i++) {
        ui->cmb_filterWK->addItem(headersWK[i]);
        wk_model->setHeaderData(i, Qt::Horizontal, headersWK[i]);
        ui->competitionsTable->horizontalHeader()->setSectionResizeMode(i, resizeModeWK[i]);
    }
    ui->competitionsTable->hideColumn(7);
    ui->cmb_filterWK->setCurrentIndex(idx);
    if (ui->cmb_filterWK->currentIndex() == -1)
        ui->cmb_filterWK->setCurrentIndex(0);
}

void CompetitionsWidget::addWK()
{
    CompetitionDialog *sw = new CompetitionDialog(this->event, 0, this);
    if (sw->exec() == 1)
        fillWKTable();
    _global::updateRgDis(this->event);
    ui->competitionsTable->setFocus();
}

void CompetitionsWidget::editWK()
{
    if (ui->competitionsTable->currentIndex().isValid()) {
        QModelIndex idx = ui->competitionsTable->currentIndex();
        CompetitionDialog *sw = new CompetitionDialog(
            this->event,
            QVariant(wk_sort_model->data(
                         wk_sort_model->index(ui->competitionsTable->currentIndex().row(), 7)))
                .toInt(),
            this);
        if (sw->exec() == 1)
            fillWKTable();
        _global::updateRgDis(this->event);
        ui->competitionsTable->setCurrentIndex(idx);
        ui->competitionsTable->setFocus();
    }
}

void CompetitionsWidget::delWK()
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
                                wk_sort_model->data(
                                    wk_sort_model->index(ui->competitionsTable->currentIndex().row(),
                                                         7)))
                                .toInt());
            query.exec();
            fillWKTable();
        }
        _global::updateRgDis(this->event);
        ui->competitionsTable->setFocus();
    }
}

void CompetitionsWidget::updateWKFilterColumn(int index)
{
    wk_sort_model->setFilterKeyColumn(index);
}

void CompetitionsWidget::updateWKFilterText(const QString &text)
{
    wk_sort_model->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive, QRegExp::Wildcard));
}
