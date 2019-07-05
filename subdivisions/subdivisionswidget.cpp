#include "subdivisionswidget.h"
#include "assignmenttablemodel.h"
#include "model/objects/event.h"
#include "model/settings/session.h"
#include "src/global/header/_delegates.h"
#include "src/global/header/_global.h"
#include "ui_subdivisionswidget.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QStandardItemModel>

SubdivisionsWidget::SubdivisionsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubdivisionsWidget)
{
    ui->setupUi(this);

    this->event = Session::getInstance()->getEvent();
    this->re_model = new AssignmentTableModel(this->event);
    this->re_model2 = new AssignmentTableModel(this->event);
    this->rg_model = new QStandardItemModel();
    this->rg_model->setColumnCount(4);

    ui->lst_all->setModel(rg_model);

    connect(ui->but_add, SIGNAL(clicked()), this, SLOT(addRiege()));
    connect(ui->lst_all->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            this,
            SLOT(fetchRgData()));
    connect(ui->but_add_2, SIGNAL(clicked()), this, SLOT(sendData()));
    connect(ui->but_remove, SIGNAL(clicked()), this, SLOT(getData()));
    connect(ui->txt_nummer, SIGNAL(editingFinished()), this, SLOT(updateRiege()));
    connect(ui->but_del, SIGNAL(clicked()), this, SLOT(removeRiege()));
}

SubdivisionsWidget::~SubdivisionsWidget()
{
    delete ui;
}

void SubdivisionsWidget::fillRETable2()
{
    re_model2->setRiege("");
    ui->re_table2->setModel(re_model2);
    QHeaderView::ResizeMode resizeModeRE2[] = {QHeaderView::ResizeToContents,
                                               QHeaderView::Stretch,
                                               QHeaderView::ResizeToContents,
                                               QHeaderView::ResizeToContents,
                                               QHeaderView::Stretch,
                                               QHeaderView::ResizeToContents};
    for (int i = 0; i < 6; i++) {
        ui->re_table2->horizontalHeader()->setSectionResizeMode(i, resizeModeRE2[i]);
    }
    rg_model->removeRows(0, rg_model->rowCount());
    QSqlQuery query;
    query.prepare(
        "SELECT var_riege, COUNT(DISTINCT int_teilnehmerid)+(SELECT COUNT(*) FROM "
        "tfx_gruppen_x_teilnehmer INNER JOIN tfx_wertungen AS w USING (int_gruppenid) INNER JOIN "
        "tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_Veranstaltungenid=? AND int_runde=? "
        "AND var_riege=tfx_wertungen.var_riege) as count, (SELECT COUNT(*) FROM tfx_mannschaften "
        "INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND "
        "tfx_mannschaften.var_riege=tfx_wertungen.var_riege),(SELECT COUNT(int_gruppenid) FROM "
        "tfx_wertungen AS w WHERE int_veranstaltungenid=? AND int_runde=? AND "
        "var_riege=tfx_wertungen.var_riege), (SELECT var_name FROM tfx_riegen_x_disziplinen INNER "
        "JOIN tfx_disziplinen USING (int_disziplinenid) WHERE "
        "int_veranstaltungenid=tfx_wettkaempfe.int_veranstaltungenid AND "
        "int_runde=tfx_wertungen.int_runde AND var_riege=tfx_wertungen.var_riege AND "
        "bol_erstes_geraet='true' LIMIT 1) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING "
        "(int_wettkaempfeid) WHERE int_veranstaltungenid=? AND tfx_wertungen.int_runde=? AND "
        "var_riege != '' GROUP BY var_riege, int_runde, int_veranstaltungenid");

    query.bindValue(0, this->event->getMainEventId());
    query.bindValue(1, this->event->getRound());
    query.bindValue(2, this->event->getId());
    query.bindValue(3, this->event->getMainEventId());
    query.bindValue(4, this->event->getRound());
    query.bindValue(5, this->event->getMainEventId());
    query.bindValue(6, this->event->getRound());
    query.exec();
    while (query.next()) {
        ui->tbl_list->setEnabled(true);
        ui->txt_nummer->setEnabled(true);
        ui->but_remove->setEnabled(true);
        ui->but_add_2->setEnabled(true);
        rg_model->insertRow(rg_model->rowCount());
        rg_model->setItem(query.at(), 0, new QStandardItem(query.value(0).toString()));
        rg_model->item(query.at(), 0)->setEditable(false);
        rg_model->setItem(query.at(), 1, new QStandardItem(query.value(1).toString()));
        rg_model->item(query.at(), 1)->setEditable(false);
        rg_model->setItem(query.at(), 2, new QStandardItem(query.value(2).toString()));
        rg_model->item(query.at(), 2)->setEditable(false);
        rg_model->setItem(query.at(), 3, new QStandardItem(query.value(3).toString()));
        rg_model->item(query.at(), 3)->setEditable(false);
        rg_model->setItem(query.at(), 4, new QStandardItem(query.value(4).toString()));
    }
    QHeaderView::ResizeMode resizeModeRE[] = {QHeaderView::Stretch,
                                              QHeaderView::Fixed,
                                              QHeaderView::Fixed,
                                              QHeaderView::Fixed,
                                              QHeaderView::Fixed};
    QStringList heads;
    heads << "Riege"
          << "Teiln."
          << "Manns."
          << "Gruppen"
          << "1. Gerät";
    for (int i = 0; i < 5; i++) {
        ui->lst_all->horizontalHeader()->setSectionResizeMode(i, resizeModeRE[i]);
        rg_model->setHeaderData(i, Qt::Horizontal, heads[i]);
    }
    ui->lst_all->horizontalHeader()->resizeSection(1, 45);
    ui->lst_all->horizontalHeader()->resizeSection(2, 45);
    ui->lst_all->horizontalHeader()->resizeSection(3, 45);
    ui->lst_all->horizontalHeader()->resizeSection(4, 90);
    ui->lst_all->setItemDelegateForColumn(4, new CmbDelegate(this->event));
    ui->lst_all->selectRow(0);
}

void SubdivisionsWidget::sendData()
{
    QModelIndexList list = ui->re_table2->selectionModel()->selectedRows();
    for (int i = list.size() - 1; i >= 0; i--) {
        re_model->insertRow(re_model2->takeRow(list.at(i).row()));
    }
    _global::updateRgDis(this->event);
    re_model->setTableData();
    re_model2->setTableData();
    setRiegenData();
}

void SubdivisionsWidget::getData()
{
    QModelIndexList list = ui->tbl_list->selectionModel()->selectedRows();
    for (int i = list.size() - 1; i >= 0; i--) {
        re_model2->insertRow(re_model->takeRow(list.at(i).row()));
    }
    _global::updateRgDis(this->event);
    re_model->setTableData();
    re_model2->setTableData();
    setRiegenData();
}

void SubdivisionsWidget::addRiege()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         tr("Namen festlegen"),
                                         tr("Bitte einen Namen für die Riege Eingeben"),
                                         QLineEdit::Normal,
                                         "",
                                         &ok);
    if (ok && !text.isEmpty()) {
        ui->tbl_list->setEnabled(true);
        ui->txt_nummer->setEnabled(true);
        ui->but_remove->setEnabled(true);
        ui->but_add_2->setEnabled(true);
        rg_model->insertRow(rg_model->rowCount());
        rg_model->setItem(rg_model->rowCount() - 1, 0, new QStandardItem(text));
        rg_model->item(rg_model->rowCount() - 1, 0)->setEditable(false);
        rg_model->setItem(rg_model->rowCount() - 1, 1, new QStandardItem("0"));
        rg_model->item(rg_model->rowCount() - 1, 1)->setEditable(false);
        rg_model->setItem(rg_model->rowCount() - 1, 2, new QStandardItem("0"));
        rg_model->item(rg_model->rowCount() - 1, 2)->setEditable(false);
        rg_model->setItem(rg_model->rowCount() - 1, 3, new QStandardItem(""));
        ui->lst_all->selectRow(rg_model->rowCount() - 1);
    } else {
        QMessageBox::information(this, "Ungültiger Name", "Sie haben keinen Namen eingegeben!");
    }
}

void SubdivisionsWidget::fetchRgData()
{
    int row = ui->lst_all->selectionModel()->currentIndex().row();
    if (row < 0)
        return;
    QString riege = rg_model->item(row, 0)->text();
    re_model->setRiege(riege);
    ui->tbl_list->setModel(re_model);
    ui->txt_nummer->setText(riege);
    QHeaderView::ResizeMode resizeModeRE2[] = {QHeaderView::ResizeToContents,
                                               QHeaderView::Stretch,
                                               QHeaderView::ResizeToContents,
                                               QHeaderView::ResizeToContents,
                                               QHeaderView::Stretch,
                                               QHeaderView::ResizeToContents};
    for (int i = 0; i < 6; i++) {
        ui->tbl_list->horizontalHeader()->setSectionResizeMode(i, resizeModeRE2[i]);
    }
}

void SubdivisionsWidget::updateRiege()
{
    if (ui->txt_nummer->text() == "") {
        removeRiege();
        return;
    }
    re_model->updateRiege(ui->txt_nummer->text());
    rg_model->item(ui->lst_all->selectionModel()->currentIndex().row(), 0)
        ->setText(ui->txt_nummer->text());
}

void SubdivisionsWidget::setRiegenData()
{
    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(DISTINCT int_teilnehmerid) as count, (SELECT COUNT(*) FROM tfx_mannschaften "
        "INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND "
        "tfx_mannschaften.var_riege=tfx_wertungen.var_riege) FROM tfx_wertungen INNER JOIN "
        "tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND "
        "tfx_wertungen.int_runde=? AND var_riege=? GROUP BY var_riege");
    query.bindValue(0, this->event->getId());
    query.bindValue(1, this->event->getMainEventId());
    query.bindValue(2, this->event->getRound());
    query.bindValue(3,
                    rg_model->item(ui->lst_all->selectionModel()->currentIndex().row(), 0)->text());
    query.exec();
    query.next();
    rg_model->item(ui->lst_all->selectionModel()->currentIndex().row(), 1)
        ->setText(query.value(0).toString());
    rg_model->item(ui->lst_all->selectionModel()->currentIndex().row(), 2)
        ->setText(query.value(1).toString());
}

void SubdivisionsWidget::removeRiege()
{
    re_model->updateRiege("");
    rg_model->removeRow(ui->lst_all->selectionModel()->currentIndex().row());
    re_model2->setTableData();
    if (rg_model->item(0, 0) == nullptr) {
        re_model->setRiege("#####");
        ui->txt_nummer->setText("");
        ui->tbl_list->setEnabled(false);
        ui->txt_nummer->setEnabled(false);
        ui->but_remove->setEnabled(false);
        ui->but_add_2->setEnabled(false);
        return;
    }
    ui->lst_all->selectRow(0);
}
