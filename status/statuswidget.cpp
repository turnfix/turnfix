#include "statuswidget.h"
#include "model/objects/event.h"
#include "model/settings/session.h"
#include "src/global/header/_global.h"
#include "statustablemodel.h"
#include "ui_statuswidget.h"
#include <QSortFilterProxyModel>
#include <QSqlQuery>

StatusWidget::StatusWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StatusWidget)
{
    ui->setupUi(this);

    this->event = Session::getInstance()->getEvent();

    mdl_status1 = new StatusTableModel();
    mdl_sort_status1 = new QSortFilterProxyModel();
    mdl_sort_status1->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mdl_sort_status1->setSourceModel(mdl_status1);
    mdl_status2 = new StatusTableModel();
    mdl_sort_status2 = new QSortFilterProxyModel();
    mdl_sort_status2->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mdl_sort_status2->setSourceModel(mdl_status2);

    connect(ui->but_upstatus, SIGNAL(clicked()), this, SLOT(updateStatus()));
}

StatusWidget::~StatusWidget()
{
    delete ui;
}

void StatusWidget::updateStatus()
{
    QSqlQuery query2;
    query2.prepare("SELECT int_startnummer AS \"StNr.\", CASE WHEN tfx_gruppen.int_gruppenid IS NULL THEN var_vorname || ' ' || var_nachname ELSE tfx_gruppen.var_name END AS \"Name\", tfx_vereine.var_name AS \"Verein\", var_nummer AS \"Wettkampf\", tfx_status.var_name AS \"Status\" FROM tfx_wertungen INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid INNER JOIN tfx_status ON tfx_status.int_statusid = tfx_wertungen.int_statusid WHERE int_veranstaltungenid=? AND tfx_wertungen.int_runde=? ORDER BY int_startnummer");
    query2.bindValue(0, this->event->getMainEventId());
    query2.bindValue(1, this->event->getRound());
    query2.exec();
    mdl_status1->setSingle(true);
    mdl_status1->setQuery(query2);
    ui->tbl_status1->setModel(mdl_sort_status1);
    QSqlQuery columns;
    columns.prepare("SELECT int_disziplinenid, tfx_disziplinen.var_name FROM tfx_riegen_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) WHERE int_veranstaltungenid=? AND int_runde=? GROUP BY int_disziplinenid, tfx_disziplinen.var_name, int_sportid ORDER BY int_sportid, tfx_disziplinen.var_name");
    columns.bindValue(0, this->event->getMainEventId());
    columns.bindValue(1, this->event->getRound());
    columns.exec();
    QString querystring =  "SELECT DISTINCT(var_riege) AS \"Riege\"";
    while (columns.next()) {
        querystring += ", (SELECT tfx_status.var_name FROM tfx_riegen_x_disziplinen INNER JOIN tfx_status USING (int_statusid) WHERE tfx_riegen_x_disziplinen.var_riege = tfx_wertungen.var_riege AND int_disziplinenid='" + columns.value(0).toString() + "' AND int_veranstaltungenid='" + QString::number(this->event->getMainEventId()) + "' AND int_runde='" + QString::number(this->event->getRound()) + "' LIMIT 1) AS \"" + columns.value(1).toString() + "\"";
    }
    querystring += " FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? ORDER BY var_riege";
    QSqlQuery query3;
    query3.prepare(querystring);
    query3.bindValue(0, this->event->getMainEventId());
    query3.bindValue(1, this->event->getRound());
    query3.exec();
    mdl_status2->setSingle(false);
    mdl_status2->setQuery(query3);
    ui->tbl_status2->setModel(mdl_sort_status2);
    ui->tbl_status2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tbl_status2->horizontalHeader()->resizeSection(0, 40);
    int size = _global::querySize(columns);
    for (int i=0; i<size;i++) {
        ui->tbl_status2->horizontalHeader()->setSectionResizeMode(i + 1, QHeaderView::Fixed);
        ui->tbl_status2->horizontalHeader()->resizeSection(i + 1, 125);
    }
    QHeaderView::ResizeMode resizeModeST2[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Fixed};
    int resizeST2[] = {40, 250,300,350,200};
    for (int i=0;i<5;i++) {
        ui->tbl_status1->horizontalHeader()->setSectionResizeMode(i, resizeModeST2[i]);
        ui->tbl_status1->horizontalHeader()->resizeSection(i, resizeST2[i]);
    }
}
