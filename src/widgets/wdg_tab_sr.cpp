#include <QSqlQuery>
#include <QKeyEvent>
#include "model/settings/session.h"
#include "model/objects/event.h"
#include "header/wdg_tab_sr.h"
#include "ui_wdg_tab_sr.h"
#include "../global/header/_global.h"

#include <QDebug>
#include <QSqlError>

Tab_SR::Tab_SR(QWidget *parent) : QWidget(parent), ui(new Ui::Tab_SR) {
    ui->setupUi(this);
    ui->listWidget_2->installEventFilter(this);

    this->event = Session::getInstance()->getEvent();

    connect(ui->cmb_riege, SIGNAL(currentIndexChanged(QString)), this, SLOT(squadChange(QString)));
    connect(ui->cmb_dis, SIGNAL(currentIndexChanged(int)), this, SLOT(load()));
    connect(ui->but_reset, SIGNAL(clicked()), this, SLOT(reset()));
    connect(ui->listWidget, SIGNAL(itemDropped()), this, SLOT(itemRemoved()));
    connect(ui->listWidget_2, SIGNAL(itemDropped()), this, SLOT(itemDropped()));
    connect(ui->but_copy, SIGNAL(clicked()), this, SLOT(copy()));
}

Tab_SR::~Tab_SR() {
    delete ui;
}

void Tab_SR::init() {
    ui->cmb_riege->clear();
    QSqlQuery query2;
    query2.prepare("SELECT DISTINCT(var_riege) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? ORDER BY var_riege");
    query2.bindValue(0, this->event->getMainEventId());
    query2.bindValue(1, this->event->getRound());
    query2.exec();
    while (query2.next()) {
        ui->cmb_riege->addItem(query2.value(0).toString(),query2.value(0).toString());
    }
}

void Tab_SR::squadChange(QString squadno) {
    ui->cmb_dis->clear();
    QSqlQuery query;
    query.prepare("SELECT DISTINCT int_disziplinenid, tfx_disziplinen.var_name, var_icon, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp FROM tfx_disziplinen INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_wertungen USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND var_riege=? ORDER BY tfx_disziplinen.var_name, kp");
    query.bindValue(0, this->event->getMainEventId());
    query.bindValue(1 ,squadno);
    query.exec();
    while (query.next()) {
        QString name = query.value(1).toString();
        if (query.value(3).toInt() == 1) {
            name += " (Pflicht)";
        }
        ui->cmb_dis->addItem(QIcon(query.value(2).toString()),name,query.value(0).toInt());
        if (query.value(3).toInt() == 1) {
            ui->cmb_dis->addItem(QIcon(query.value(2).toString()),query.value(1).toString()+" (Kür)",query.value(0).toInt());
        }
    }
}

void Tab_SR::load() {
    ui->wdg_content->setEnabled(false);
    ui->but_reset->setEnabled(false);
    ui->but_copy->setEnabled(false);
    ui->listWidget->clear();
    ui->listWidget_2->clear();
    bool kuer=false;
    if (ui->cmb_dis->currentText().right(5) == "(Kür)") kuer = true;
    QSqlQuery query4;
    query4.prepare("SELECT CASE WHEN tfx_gruppen.int_gruppenid IS NULL THEN var_vorname || ' ' || var_nachname ELSE tfx_gruppen.var_name END, tfx_vereine.var_name, tfx_wertungen.int_wertungenid, int_pos FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid LEFT JOIN tfx_mannschaften ON tfx_mannschaften.int_mannschaftenid = tfx_wertungen.int_mannschaftenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid LEFT JOIN tfx_startreihenfolge ON tfx_startreihenfolge.int_wertungenid = tfx_wertungen.int_wertungenid AND tfx_startreihenfolge.int_disziplinenid=? AND tfx_startreihenfolge.int_kp=? INNER JOIN tfx_wettkaempfe_x_disziplinen ON tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid AND tfx_wettkaempfe_x_disziplinen.int_disziplinenid=? WHERE int_veranstaltungenid=? AND tfx_wertungen.var_riege=? AND int_runde=? AND bol_startet_nicht='false' AND ((SELECT COUNT(*) FROM tfx_wettkaempfe_x_disziplinen WHERE int_disziplinenid=? AND int_wettkaempfeid=tfx_wettkaempfe.int_wettkaempfeid)>0 AND (NOT EXISTS (SELECT int_wertungen_x_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=tfx_wertungen.int_wertungenid) OR EXISTS (SELECT int_wertungen_x_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE tfx_wertungen_x_disziplinen.int_wertungenid=tfx_wertungen.int_wertungenid AND tfx_wertungen_x_disziplinen.int_disziplinenid=?))) AND (CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 'true' ELSE 'false' END = 'true' OR ?='true') ORDER BY int_pos, tfx_wettkaempfe.var_nummer, tfx_mannschaften.int_nummer, tfx_mannschaften.int_mannschaftenid, tfx_wertungen.int_startnummer");
    query4.bindValue(0, ui->cmb_dis->itemData(ui->cmb_dis->currentIndex()));
    query4.bindValue(1, (int)kuer);
    query4.bindValue(2, ui->cmb_dis->itemData(ui->cmb_dis->currentIndex()));
    query4.bindValue(3, this->event->getMainEventId());
    query4.bindValue(4, ui->cmb_riege->currentText());
    query4.bindValue(5, this->event->getRound());
    query4.bindValue(6, ui->cmb_dis->itemData(ui->cmb_dis->currentIndex()));
    query4.bindValue(7, ui->cmb_dis->itemData(ui->cmb_dis->currentIndex()));
    query4.bindValue(8, !kuer);
    query4.exec();
    clubs.clear();
    if (_global::querySize(query4)>0) {
        while (query4.next()) {
            if (!clubs.contains(query4.value(1).toString())) clubs.append(query4.value(1).toString());
            QListWidgetItem *itm = new QListWidgetItem(query4.value(0).toString() + " - " + query4.value(1).toString());
            itm->setData(Qt::UserRole,query4.value(2).toInt());
            if (query4.value(3).toInt()>0) {
                ui->listWidget_2->insertItem(query4.value(3).toInt(),itm);
            } else {
                ui->listWidget->addItem(itm);
            }
        }
        ui->wdg_content->setEnabled(true);
        ui->but_reset->setEnabled(true);
        ui->but_copy->setEnabled(true);
    }
}

void Tab_SR::reset() {
    int kuer=0;
    if (ui->cmb_dis->currentText().right(5) == "(Kür)") kuer = 1;
    QSqlQuery query;
    query.prepare("DELETE FROM tfx_startreihenfolge WHERE int_disziplinenid=? AND int_kp=? AND (int_wertungenid IN ("+_global::intListToString(wertungenInList())+") OR int_wertungenid IN ("+_global::intListToString(wertungenInList2())+"))");
    query.bindValue(0,ui->cmb_dis->itemData(ui->cmb_dis->currentIndex()));
    query.bindValue(1,kuer);
    query.exec();
    load();
}

void Tab_SR::itemDropped() {
    int kuer=0;
    if (ui->cmb_dis->currentText().right(5) == "(Kür)") kuer = 1;
    QList<int> ids = wertungenInList2();
    QSqlQuery query2;
    query2.prepare("SELECT int_startreihenfolgeid FROM tfx_startreihenfolge WHERE int_disziplinenid=? AND int_kp=? AND int_wertungenid=?");
    query2.bindValue(0,ui->cmb_dis->itemData(ui->cmb_dis->currentIndex()));
    query2.bindValue(1,kuer);
    QSqlQuery query3;
    for (int i=0;i<ids.size();i++) {
        query2.bindValue(2,ids.at(i));
        query2.exec();
        if (_global::querySize(query2)==0) {
            query3.prepare("INSERT INTO tfx_startreihenfolge (int_wertungenid,int_disziplinenid,int_pos,int_kp) VALUES (?,?,?,?)");
            query3.bindValue(0,ids.at(i));
            query3.bindValue(1,ui->cmb_dis->itemData(ui->cmb_dis->currentIndex()));
            query3.bindValue(2,i+1);
            query3.bindValue(3,kuer);
        } else {
            query2.next();
            query3.prepare("UPDATE tfx_startreihenfolge SET int_pos=? WHERE int_startreihenfolgeid=?");
            query3.bindValue(0,i+1);
            query3.bindValue(1,query2.value(0).toInt());
        }
        query3.exec();
    }
}

void Tab_SR::itemRemoved() {
    int kuer=0;
    if (ui->cmb_dis->currentText().right(5) == "(Kür)") kuer = 1;
    QSqlQuery query;
    query.prepare("DELETE FROM tfx_startreihenfolge WHERE int_disziplinenid=? AND int_kp=? AND int_wertungenid IN ("+_global::intListToString(wertungenInList())+")");
    query.bindValue(0,ui->cmb_dis->itemData(ui->cmb_dis->currentIndex()));
    query.bindValue(1,kuer);
    query.exec();
}

QList<int> Tab_SR::wertungenInList() {
    QList<int> list;
    for (int i=0;i<ui->listWidget->count();i++) {
        list.append(ui->listWidget->item(i)->data(Qt::UserRole).toInt());
    }
    return list;
}

QList<int> Tab_SR::wertungenInList2() {
    QList<int> list;
    for (int i=0;i<ui->listWidget_2->count();i++) {
        list.append(ui->listWidget_2->item(i)->data(Qt::UserRole).toInt());
    }
    return list;
}


void Tab_SR::setRgDis(QString rg, QString dis) {
    ui->cmb_riege->setCurrentIndex(ui->cmb_riege->findText(rg));
    ui->cmb_dis->setCurrentIndex(ui->cmb_dis->findText(dis));
    ui->cmb_dis->setEnabled(false);
    ui->cmb_riege->setEnabled(false);
    ui->but_copy->setEnabled(false);
}

QStringList Tab_SR::getClubs() {
    return clubs;
}

void Tab_SR::copy() {
    QList<int> ids = wertungenInList2();
    for (int i=0;i<ui->cmb_dis->count();i++) {
        if (i == ui->cmb_dis->currentIndex()) continue;
        bool kuer=false;
        if (ui->cmb_dis->itemText(i).right(5) == "(Kür)") kuer = true;
        QSqlQuery query;
        query.prepare("DELETE FROM tfx_startreihenfolge WHERE int_disziplinenid=? AND int_kp=? AND (int_wertungenid IN ("+_global::intListToString(wertungenInList())+") OR int_wertungenid IN ("+_global::intListToString(wertungenInList2())+"))");
        query.bindValue(0,ui->cmb_dis->itemData(i));
        query.bindValue(1,(int)kuer);
        query.exec();
        QSqlQuery query3;
        for (int j=0;j<ids.size();j++) {
            query3.prepare("INSERT INTO tfx_startreihenfolge (int_wertungenid,int_disziplinenid,int_pos,int_kp) VALUES (?,?,?,?)");
            query3.bindValue(0,ids.at(j));
            query3.bindValue(1,ui->cmb_dis->itemData(i));
            query3.bindValue(2,j+1);
            query3.bindValue(3,(int)kuer);
            query3.exec();
        }
    }
}

bool Tab_SR::eventFilter(QObject *o, QEvent *e) {
    if (o == ui->listWidget_2 && e->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        if (keyEvent->key() == Qt::Key_Delete) {
            for (int i=ui->listWidget_2->selectedItems().size()-1;i>=0;i--) {
                QListWidgetItem *item = ui->listWidget_2->takeItem(ui->listWidget_2->row(ui->listWidget_2->selectedItems().at(i)));
                ui->listWidget->addItem(item);
            }
            return true;
        }
    }
    return QObject::eventFilter(o, e);
}
