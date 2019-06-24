#include "header/wdg_riegen.h"
#include "ui_wdg_riegen.h"
#include "model/objects/event.h"
#include "model/viewmodels/assignmenttablemodel.h"
#include "../../global/header/_global.h"

#include <QDebug>
#include <QSqlError>

Riegen_Widget::Riegen_Widget(Event *event, QString riege, QWidget *parent) : QWidget(parent), ui(new Ui::Riegen_Widget) {
    ui->setupUi(this);

    this->event = event;
    this->re_model = new AssignmentTableModel(this->event);
    this->re_model->setRiege(riege);

    ui->txt_nummer->setText(riege);
    ui->tbl_list->setDropIndicatorShown(true);
    ui->tbl_list->setModel(re_model);

    QHeaderView::ResizeMode resizeModeRE2[] = {QHeaderView::ResizeToContents, QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::Stretch, QHeaderView::ResizeToContents};
    for (int i=0;i<6;i++) {
        ui->tbl_list->horizontalHeader()->setSectionResizeMode(i, resizeModeRE2[i]);
    }

    refreshDisziplinen();

    connect(ui->but_add, SIGNAL(clicked()), this, SLOT(request()));
    connect(ui->but_remove, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->txt_nummer, SIGNAL(editingFinished()), this, SLOT(updateRiege()));
    connect(ui->cmb_geraet, SIGNAL(currentIndexChanged(int)), this, SLOT(saveFirst()));
}

Riegen_Widget::~Riegen_Widget() {
    delete ui;
}

void Riegen_Widget::request() {
    emit(requestSelected(this));
}

void Riegen_Widget::remove() {
    emit(removeSelected(this,ui->tbl_list));
}

void Riegen_Widget::updateRiege() {
    re_model->updateRiege(ui->txt_nummer->text());
    if (ui->txt_nummer->text() == "") {
        emit(removeMe(this));
    }
}

void Riegen_Widget::refreshDisziplinen() {
    disconnect(ui->cmb_geraet, nullptr, nullptr, nullptr);
    QString selected = ui->cmb_geraet->currentText();
    ui->cmb_geraet->clear();
    ui->cmb_geraet->addItem("",-1);
    QSqlQuery query;
    query.prepare("SELECT var_kurz2, bol_erstes_geraet, int_riegen_x_disziplinenid FROM tfx_riegen_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) WHERE int_veranstaltungenid=? AND int_runde=? AND var_riege=? ORDER BY int_disziplinenid");
    query.bindValue(0, this->event->getMainEventId());
    query.bindValue(1, this->event->getRound());
    query.bindValue(2, ui->txt_nummer->text());
    query.exec();
    int sel=-1;
    while (query.next()) {
        if (query.value(1).toBool()) sel = query.at();
        ui->cmb_geraet->addItem(query.value(0).toString(),query.value(2).toInt());
    }
    ui->cmb_geraet->setCurrentIndex(sel);
    connect(ui->cmb_geraet, SIGNAL(currentIndexChanged(int)), this, SLOT(saveFirst()));
}

void Riegen_Widget::saveFirst() {
    QSqlQuery query2;
    query2.prepare("UPDATE tfx_riegen_x_disziplinen SET bol_erstes_geraet='true' WHERE int_riegen_x_disziplinenid = ?");
    query2.bindValue(0,ui->cmb_geraet->itemData(ui->cmb_geraet->currentIndex()));
    query2.exec();
    query2.prepare("UPDATE tfx_riegen_x_disziplinen SET bol_erstes_geraet='false' WHERE int_veranstaltungenid=? AND int_runde=? AND var_riege=? AND int_riegen_x_disziplinenid != ?");
    query2.bindValue(0, this->event->getMainEventId());
    query2.bindValue(1, this->event->getRound());
    query2.bindValue(2, ui->txt_nummer->text());
    query2.bindValue(3,ui->cmb_geraet->itemData(ui->cmb_geraet->currentIndex()));
    query2.exec();
}
