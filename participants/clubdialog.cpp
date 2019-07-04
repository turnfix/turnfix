#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include "model/objects/event.h"
#include "clubdialog.h"
#include "ui_clubdialog.h"
#include "src/global/header/_global.h"

ClubDialog::ClubDialog(Event *event, QWidget *parent) : QDialog(parent), ui(new Ui::ClubDialog) {
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    this->event = event;
    this->model = new QStandardItemModel();
    this->model->setColumnCount(5);
    this->model2 = new QSqlQueryModel();
    ui->tbl_tn->setModel(model);
    ui->tbl_list->setModel(model2);
    QHeaderView::ResizeMode resizeMode[] = {QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::Fixed};
    int resize[] = {200,40,23,23,0};
    QString headers[5] = {"Name","Geb.","AK","SN","id"};
    for (int i=0;i<5;i++) {
        model->setHeaderData(i, Qt::Horizontal, headers[i]);
        ui->tbl_tn->horizontalHeader()->setSectionResizeMode(i, resizeMode[i]);
        ui->tbl_tn->horizontalHeader()->resizeSection(i, resize[i]);
    }

    QSqlQuery query2;
    query2.prepare("SELECT int_wettkaempfeid, var_nummer, var_name, int_typ FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? AND int_typ=0 ORDER BY var_nummer ASC");
    query2.bindValue( 0, this->event->getId());
    query2.exec();
    while (query2.next()) {
        ui->cmb_wk->addItem(query2.value(1).toString() + " " + query2.value(2).toString(),query2.value(0).toInt());
    }
    QSqlQuery query;
    query.prepare("SELECT int_vereineid, var_name FROM tfx_vereine ORDER BY var_name");
    query.exec();
    while (query.next()) {
        ui->cmb_club->addItem(query.value(1).toString(),query.value(0).toInt());
    }
    updateTable();
    connect(ui->cmb_club,SIGNAL(currentIndexChanged(int)),this,SLOT(updateTable()));
    connect(ui->chk_vp,SIGNAL(stateChanged(int)),this,SLOT(updateTable()));
    connect(ui->but_add, SIGNAL(clicked()), this, SLOT(addTn()));
    connect(ui->but_del, SIGNAL(clicked()), this, SLOT(removeTn()));
    connect(ui->but_add, SIGNAL(clicked()), this, SLOT(updateTable()));
    connect(ui->but_del, SIGNAL(clicked()), this, SLOT(updateTable()));
    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
}

ClubDialog::~ClubDialog() {
    delete ui;
}

void ClubDialog::fillTable() {
    for (int i=0;i<lst_int_ids.size();i++) {
        QSqlQuery query;
        query.prepare("SELECT tfx_teilnehmer.var_nachname || ', ' || tfx_teilnehmer.var_vorname, "+_global::date("dat_geburtstag",4)+", tfx_teilnehmer.int_teilnehmerid FROM tfx_teilnehmer WHERE tfx_teilnehmer.int_teilnehmerid=? LIMIT 1");
        query.bindValue( 0, lst_int_ids.at(i) );
        query.exec();
        query.next();
        for (int j=0;j<2;j++) {
            model->setItem(i,j,new QStandardItem(query.value(j).toString()));
        }
        QStandardItem *itak = new QStandardItem();
        QStandardItem *itks = new QStandardItem();
        itak->setCheckable(true);
        model->setItem(i,2,itak);
        itks->setCheckable(true);
        model->setItem(i,3,itks);
        model->setItem(i,4,new QStandardItem(query.value(2).toString()));
    }
}

void ClubDialog::updateTable() {
    QSqlQuery query3;
    QString query;
    query = "SELECT var_nachname || ', ' || var_vorname, "+_global::date("dat_geburtstag",2)+", tfx_vereine.var_name, int_teilnehmerid FROM tfx_teilnehmer INNER JOIN tfx_vereine USING (int_vereineid) WHERE int_vereineid=? ";
    if (ui->chk_vp->isChecked() ) {
        query += "AND int_teilnehmerid NOT IN (SELECT int_teilnehmerid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_teilnehmerid>0) ";
        if (lst_int_ids.size()>0) query += "AND int_teilnehmerid NOT IN (" + _global::intListToString(lst_int_ids) + ") ";
    }
    query += "ORDER BY tfx_vereine.var_name, var_nachname, var_vorname";
    query3.prepare(query);
    query3.bindValue(0,ui->cmb_club->itemData(ui->cmb_club->currentIndex()));
    if (ui->chk_vp->isChecked()) {
        query3.bindValue(1, this->event->getMainEventId());
    }
    query3.exec();
    model2->setQuery(query3);
    QHeaderView::ResizeMode resizeMode2[] = {QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed};
    int resize2[] = {200,40,150,0};
    QString headers2[4] = {"Name","Geb.","Verein","id"};
    for (int i=0;i<4;i++) {
        model2->setHeaderData(i, Qt::Horizontal, headers2[i]);
        ui->tbl_list->horizontalHeader()->setSectionResizeMode(i, resizeMode2[i]);
        ui->tbl_list->horizontalHeader()->resizeSection(i, resize2[i]);
    }
}

void ClubDialog::addTn() {
    QModelIndexList indexes = ui->tbl_list->selectionModel()->selectedRows();
    for (int i=0;i<indexes.count();i++) {
        if (lst_int_ids.indexOf(QVariant(model2->data(model2->index(indexes.at(i).row(),3))).toInt()) == -1) {
            lst_int_ids.append(QVariant(model2->data(model2->index(indexes.at(i).row(),3))).toInt());

        }
    }
    fillTable();
}

void ClubDialog::removeTn() {
    QModelIndexList indexes = ui->tbl_tn->selectionModel()->selectedRows();
    std::sort(indexes.begin(), indexes.end());
    for (int i=indexes.count()-1;i>=0;i--) {
        model->removeRow(indexes.at(i).row());
        lst_int_ids.removeAt(indexes.at(i).row());
    }
}

void ClubDialog::save() {
    for (int i=0;i<lst_int_ids.size();i++) {
        QSqlQuery query20;
        query20.prepare("SELECT MAX(int_startnummer) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid INNER JOIN tfx_vereine USING (int_vereineid) WHERE int_veranstaltungenid=? AND int_runde=?");
        query20.bindValue(0, this->event->getId());
        query20.bindValue(1, this->event->getRound());
        query20.exec();
        query20.next();
        QSqlQuery query7;
        query7.prepare("INSERT INTO tfx_wertungen (int_teilnehmerid,int_wettkaempfeid,bol_ak,int_runde,int_startnummer,bol_startet_nicht,int_statusid) VALUES(?,?,?,?,?,?,?)");
        query7.bindValue( 0, lst_int_ids.at(i) );
        query7.bindValue( 1, ui->cmb_wk->itemData(ui->cmb_wk->currentIndex()) );
        if (model->item(i,2)->checkState() == Qt::Checked) {
            query7.bindValue(2,true);
        } else {
            query7.bindValue(2,false);
        }
        query7.bindValue( 3, this->event->getRound() );
        query7.bindValue( 4, (query20.value(0).toInt()+1) );
        if (model->item(i,3)->checkState() == Qt::Checked) {
            query7.bindValue(5,true);
        } else {
            query7.bindValue(5,false);
        }
        query7.bindValue(6,1);
        query7.exec();
    }
    done(1);
}

