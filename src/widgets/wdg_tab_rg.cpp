#include <QSqlQuery>
#include <QInputDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include "header/wdg_tab_rg.h"
#include "../global/header/_delegates.h"
#include "../global/header/_global.h"
#include "../models/header/mdl_einteilen.h"
#include "others/header/wdg_riegen.h"

Tab_RG::Tab_RG(QWidget* parent) : QWidget(parent) {
    setupUi(this);
    re_model = new QEinteilTableModel();
    re_model2 = new QEinteilTableModel();
    rg_model = new QStandardItemModel();
    rg_model->setColumnCount(4);
    lst_all->setModel(rg_model);
    connect(but_add, SIGNAL(clicked()), this, SLOT(addRiege()));
    connect(lst_all->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(fetchRgData()));
    connect(but_add_2, SIGNAL(clicked()), this, SLOT(sendData()));
    connect(but_remove, SIGNAL(clicked()), this, SLOT(getData()));
    connect(txt_nummer, SIGNAL(editingFinished()), this, SLOT(updateRiege()));
    connect(but_del, SIGNAL(clicked()), this, SLOT(removeRiege()));
}


void Tab_RG::fillRETable2() {
    re_model2->setRiege("");
    re_table2->setModel(re_model2);
    QHeaderView::ResizeMode resizeModeRE2[] = {QHeaderView::ResizeToContents, QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::Stretch, QHeaderView::ResizeToContents};
    for (int i=0;i<6;i++) {
        re_table2->horizontalHeader()->setResizeMode(i, resizeModeRE2[i]);
    }
    rg_model->removeRows(0,rg_model->rowCount());
    QSqlQuery query;
    query.prepare("SELECT var_riege, COUNT(DISTINCT int_teilnehmerid)+(SELECT COUNT(*) FROM tfx_gruppen_x_teilnehmer INNER JOIN tfx_wertungen AS w USING (int_gruppenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_Veranstaltungenid=? AND int_runde=? AND var_riege=tfx_wertungen.var_riege) as count, (SELECT COUNT(*) FROM tfx_mannschaften INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND tfx_mannschaften.var_riege=tfx_wertungen.var_riege),(SELECT COUNT(int_gruppenid) FROM tfx_wertungen AS w WHERE int_veranstaltungenid=? AND int_runde=? AND var_riege=tfx_wertungen.var_riege), (SELECT var_name FROM tfx_riegen_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) WHERE int_veranstaltungenid=tfx_wettkaempfe.int_veranstaltungenid AND int_runde=tfx_wertungen.int_runde AND var_riege=tfx_wertungen.var_riege AND bol_erstes_geraet='true' LIMIT 1) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND tfx_wertungen.int_runde=? AND var_riege != '' GROUP BY var_riege, int_runde, int_veranstaltungenid");

    query.bindValue(0,_global::checkHWK());
    query.bindValue(1, _global::getRunde());
    query.bindValue(2,_global::getWkNr());
    query.bindValue(3,_global::checkHWK());
    query.bindValue(4, _global::getRunde());
    query.bindValue(5,_global::checkHWK());
    query.bindValue(6, _global::getRunde());
    query.exec();
    while (query.next()) {
        tbl_list->setEnabled(true);
        txt_nummer->setEnabled(true);
        but_remove->setEnabled(true);
        but_add_2->setEnabled(true);
        rg_model->insertRow(rg_model->rowCount());
        rg_model->setItem(query.at(),0,new QStandardItem(query.value(0).toString()));
        rg_model->item(query.at(),0)->setEditable(false);
        rg_model->setItem(query.at(),1,new QStandardItem(query.value(1).toString()));
        rg_model->item(query.at(),1)->setEditable(false);
        rg_model->setItem(query.at(),2,new QStandardItem(query.value(2).toString()));
        rg_model->item(query.at(),2)->setEditable(false);
        rg_model->setItem(query.at(),3,new QStandardItem(query.value(3).toString()));
        rg_model->item(query.at(),3)->setEditable(false);
        rg_model->setItem(query.at(),4,new QStandardItem(query.value(4).toString()));
    }
    QHeaderView::ResizeMode resizeModeRE[] = {QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Fixed};
    QStringList heads;
    heads << "Riege" << "Teiln." << "Manns." << "Gruppen" << "1. Gerät";
    for (int i=0;i<5;i++) {
        lst_all->horizontalHeader()->setResizeMode(i, resizeModeRE[i]);
        rg_model->setHeaderData(i, Qt::Horizontal, heads[i]);
    }
    lst_all->horizontalHeader()->resizeSection(1, 45);
    lst_all->horizontalHeader()->resizeSection(2, 45);
    lst_all->horizontalHeader()->resizeSection(3, 45);
    lst_all->horizontalHeader()->resizeSection(4, 90);
    lst_all->setItemDelegateForColumn(4,new cmbDelegate);
    lst_all->selectRow(0);
}

void Tab_RG::sendData() {
    QModelIndexList list = re_table2->selectionModel()->selectedRows();
    for (int i=list.size()-1;i>=0;i--) {
        re_model->insertRow(re_model2->takeRow(list.at(i).row()));
    }
    _global::updateRgDis();
    re_model->setTableData();
    re_model2->setTableData();
    setRiegenData();
}

void Tab_RG::getData() {
    QModelIndexList list = tbl_list->selectionModel()->selectedRows();
    for (int i=list.size()-1;i>=0;i--) {
        re_model2->insertRow(re_model->takeRow(list.at(i).row()));
    }
    _global::updateRgDis();
    re_model->setTableData();
    re_model2->setTableData();
    setRiegenData();
}

void Tab_RG::addRiege() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Namen festlegen"), tr("Bitte einen Namen für die Riege Eingeben"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        tbl_list->setEnabled(true);
        txt_nummer->setEnabled(true);
        but_remove->setEnabled(true);
        but_add_2->setEnabled(true);
        rg_model->insertRow(rg_model->rowCount());
        rg_model->setItem(rg_model->rowCount()-1,0,new QStandardItem(text));
        rg_model->item(rg_model->rowCount()-1,0)->setEditable(false);
        rg_model->setItem(rg_model->rowCount()-1,1,new QStandardItem("0"));
        rg_model->item(rg_model->rowCount()-1,1)->setEditable(false);
        rg_model->setItem(rg_model->rowCount()-1,2,new QStandardItem("0"));
        rg_model->item(rg_model->rowCount()-1,2)->setEditable(false);
        rg_model->setItem(rg_model->rowCount()-1,3,new QStandardItem(""));
        lst_all->selectRow(rg_model->rowCount()-1);
    } else {
        QMessageBox::information(this,"Ungültiger Name","Sie haben keinen Namen eingegeben!");
    }
}

void Tab_RG::fetchRgData() {
    int row = lst_all->selectionModel()->currentIndex().row();
    if (row < 0) return;
    QString riege = rg_model->item(row,0)->text();
    re_model->setRiege(riege);
    tbl_list->setModel(re_model);
    txt_nummer->setText(riege);
    QHeaderView::ResizeMode resizeModeRE2[] = {QHeaderView::ResizeToContents, QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::Stretch, QHeaderView::ResizeToContents};
    for (int i=0;i<6;i++) {
        tbl_list->horizontalHeader()->setResizeMode(i, resizeModeRE2[i]);
    }
}

void Tab_RG::updateRiege() {
    if (txt_nummer->text() == "") {
        removeRiege();
        return;
    }
    re_model->updateRiege(txt_nummer->text());
    rg_model->item(lst_all->selectionModel()->currentIndex().row(),0)->setText(txt_nummer->text());
}

void Tab_RG::setRiegenData() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(DISTINCT int_teilnehmerid) as count, (SELECT COUNT(*) FROM tfx_mannschaften INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND tfx_mannschaften.var_riege=tfx_wertungen.var_riege) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND tfx_wertungen.int_runde=? AND var_riege=? GROUP BY var_riege");
    query.bindValue(0, _global::getWkNr());
    query.bindValue(1, _global::checkHWK());
    query.bindValue(2, _global::getRunde());
    query.bindValue(3, rg_model->item(lst_all->selectionModel()->currentIndex().row(),0)->text());
    query.exec();
    query.next();
    rg_model->item(lst_all->selectionModel()->currentIndex().row(),1)->setText(query.value(0).toString());
    rg_model->item(lst_all->selectionModel()->currentIndex().row(),2)->setText(query.value(1).toString());
}

void Tab_RG::removeRiege() {
    re_model->updateRiege("");
    rg_model->removeRow(lst_all->selectionModel()->currentIndex().row());
    re_model2->setTableData();
    if (rg_model->item(0,0) == NULL) {
        re_model->setRiege("#####");
        txt_nummer->setText("");
        tbl_list->setEnabled(false);
        txt_nummer->setEnabled(false);
        but_remove->setEnabled(false);
        but_add_2->setEnabled(false);
        return;
    }
    lst_all->selectRow(0);
}
