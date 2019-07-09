#include "scorecarddialog.h"
#include "model/objects/event.h"
#include "scorecardtablemodel.h"
#include "src/global/header/_delegates.h"
#include "src/global/header/_global.h"
#include "ui_scorecarddialog.h"
#include <QSqlQuery>

ScoreCardDialog::ScoreCardDialog(Event *event, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ScoreCardDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);

    this->event = event;

    pe_model = new ScoreCardTableModel();
    ui->pe_table->setModel(pe_model);
    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(saveClose()));
}

ScoreCardDialog::~ScoreCardDialog()
{
    delete ui;
}

void ScoreCardDialog::init(int s, int we, QList< QList<int> > d) {
    stnr = s;
    disids = d;
    wertid = we;
    QSqlQuery query;
    query.prepare("SELECT 'StNr.: ' || tfx_wertungen.int_startnummer || '\n' ||var_vorname || ' ' || var_nachname || ' - ' || tfx_vereine.var_name FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_mannschaften ON tfx_mannschaften.int_mannschaftenid = tfx_wertungen.int_mannschaftenid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_mannschaften.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid OR tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid WHERE int_veranstaltungenid=? AND tfx_wertungen.int_startnummer=? AND int_runde=?");
    query.bindValue(0, this->event->getMainEventId());
    query.bindValue(1, stnr);
    query.bindValue(2, this->event->getRound());
    query.exec();
    query.next();
    ui->lbl_tn->setText(query.value(0).toString());
    QSqlQuery query3;
    query3.prepare("SELECT int_statusid, var_name, ary_colorcode FROM tfx_status WHERE bol_karte='true' ORDER BY int_statusid ASC");
    query3.exec();
    while(query3.next()) {
        ui->cmb_status->addItem(query3.value(1).toString(), query3.value(0).toInt());
        QList<int> color = _global::splitColorArray(query3.value(2).toString());
        ui->cmb_status->setItemData(ui->cmb_status->count() - 1,
                                    QColor(color.at(0), color.at(1), color.at(2)),
                                    Qt::BackgroundColorRole);
    }
    QSqlQuery query2;
    query2.prepare("SELECT int_statusid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_startnummer=? AND int_runde=?");
    query2.bindValue(0, this->event->getMainEventId());
    query2.bindValue(1, stnr);
    query2.bindValue(2, this->event->getRound());
    query2.exec();
    query2.next();
    ui->cmb_status->setCurrentIndex(ui->cmb_status->findData(query2.value(0).toInt()));
    connect(ui->cmb_status, SIGNAL(currentIndexChanged(int)), this, SLOT(statusChange1()));
    fillPETable();
}

void ScoreCardDialog::fillPETable() {
    ui->pe_table->clearSelection();
    pe_model->setTableData(wertid,disids);
    ui->pe_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->pe_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    EditorDelegate *ed = new EditorDelegate;
    connect(ed, SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)), this, SLOT(finishEdit()));
    ui->pe_table->setItemDelegateForColumn(1, ed);
    ui->pe_table->setCurrentIndex(pe_model->index(0, 1));
    ui->pe_table->edit(ui->pe_table->currentIndex());
}

void ScoreCardDialog::finishEdit() {
    int row = ui->pe_table->currentIndex().row();
    int col = ui->pe_table->currentIndex().column();
    if (pe_model->index(row+1,col).isValid()) {
        ui->pe_table->setCurrentIndex(pe_model->index(row + 1, col));
    } else {
        ui->pe_table->setCurrentIndex(pe_model->index(0, col));
    }
}

void ScoreCardDialog::statusChange1() {
    QSqlQuery query;
    query.prepare("UPDATE tfx_wertungen SET int_statusid=? WHERE int_wettkaempfeid IN (SELECT int_wettkaempfeid FROM tfx_wettkaempfe WHERE int_veranstaltungenid=?) AND int_startnummer=? AND int_runde=?");
    query.bindValue(0, ui->cmb_status->itemData(ui->cmb_status->currentIndex()).toInt());
    query.bindValue(1, this->event->getMainEventId());
    query.bindValue(2, stnr);
    query.bindValue(3, this->event->getRound());
    query.exec();
}

void ScoreCardDialog::saveClose() {
    finishEdit();
    close();
}
