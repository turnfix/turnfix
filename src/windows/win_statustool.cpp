#include <QSqlQuery>
#include "header/win_statustool.h"
#include "../widgets/header/wdg_wkstatus.h"
#include "../global/header/_global.h"

Status_Tool::Status_Tool() {
    setupUi(this);
    showMaximized();
    QSqlQuery query;
    query.prepare("SELECT int_runde, int_hauptwettkampf FROM tfx_veranstaltungen WHERE int_veranstaltungenid=? LIMIT 1");
    query.bindValue(0,_global::getWkNr());
    query.exec();
    query.next();
    _global::setWkVars(_global::getWkNr(), query.value(0).toInt(), query.value(1).toInt());
    QSqlQuery query2;
    query2.prepare("SELECT int_wettkaempfeid, var_nummer FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY var_nummer");
    query2.bindValue(0, _global::checkHWK());
    query2.exec();
    while (query2.next()) {
        cmb_wk->addItem("WkNr. " + query2.value(1).toString() + _global::wkBez(query2.value(1).toString()),query2.value(1).toString());
    }
    connect(but_add, SIGNAL(clicked()), this, SLOT(addWk()));
    connect(but_addall, SIGNAL(clicked()), this, SLOT(addAll()));
    connect(rab_tn, SIGNAL(clicked()), this, SLOT(fillStatus()));
    connect(rab_riege, SIGNAL(clicked()), this, SLOT(fillStatus()));
    connect(cmb_status1, SIGNAL(currentIndexChanged(QString)), lbl_status1, SLOT(setText(QString)));
    connect(cmb_status2, SIGNAL(currentIndexChanged(QString)), lbl_status2, SLOT(setText(QString)));
    connect(cmb_status3, SIGNAL(currentIndexChanged(QString)), lbl_status3, SLOT(setText(QString)));
    connect(cmb_status4, SIGNAL(currentIndexChanged(QString)), lbl_status4, SLOT(setText(QString)));
    fillStatus();
}

void Status_Tool::addWk() {
    Status_Widget *wdg = new Status_Widget();
    wdg->setWkVars(_global::getWkNr(),_global::getRunde());
    wdg->setStatus1(cmb_status1->currentText());
    wdg->setStatus2(cmb_status2->currentText());
    wdg->setStatus3(cmb_status3->currentText());
    wdg->setStatus4(cmb_status4->currentText());
    if (rab_tn->isChecked()) {
        wdg->setMode(true);
    } else {
        wdg->setMode(false);
    }
    wdg->setWkNr(cmb_wk->itemData(cmb_wk->currentIndex()).toString());
    connect(cmb_status1,SIGNAL(currentIndexChanged(QString)),wdg,SLOT(setStatus1(QString)));
    connect(cmb_status2,SIGNAL(currentIndexChanged(QString)),wdg,SLOT(setStatus2(QString)));
    connect(cmb_status3,SIGNAL(currentIndexChanged(QString)),wdg,SLOT(setStatus3(QString)));
    connect(cmb_status4,SIGNAL(currentIndexChanged(QString)),wdg,SLOT(setStatus4(QString)));
    connect(rab_tn, SIGNAL(clicked(bool)), wdg, SLOT(setMode(bool)));
    connect(rab_riege, SIGNAL(clicked(bool)), wdg, SLOT(setMode2(bool)));
    sca_layout->insertWidget(sca_layout->count()-1,wdg);
}

void Status_Tool::addAll() {
    for (int i=0;i<cmb_wk->count();i++) {
    Status_Widget *wdg = new Status_Widget();
    wdg->setWkVars(_global::getWkNr(),_global::getRunde());
    wdg->setStatus1(cmb_status1->currentText());
    wdg->setStatus2(cmb_status2->currentText());
    wdg->setStatus3(cmb_status3->currentText());
    wdg->setStatus4(cmb_status4->currentText());
    if (rab_tn->isChecked()) {
        wdg->setMode(true);
    } else {
        wdg->setMode(false);
    }
    wdg->setWkNr(cmb_wk->itemData(i).toString());
    connect(cmb_status1,SIGNAL(currentIndexChanged(QString)),wdg,SLOT(setStatus1(QString)));
    connect(cmb_status2,SIGNAL(currentIndexChanged(QString)),wdg,SLOT(setStatus2(QString)));
    connect(cmb_status3,SIGNAL(currentIndexChanged(QString)),wdg,SLOT(setStatus3(QString)));
    connect(cmb_status4,SIGNAL(currentIndexChanged(QString)),wdg,SLOT(setStatus4(QString)));
    connect(rab_tn, SIGNAL(clicked(bool)), wdg, SLOT(setMode(bool)));
    connect(rab_riege, SIGNAL(clicked(bool)), wdg, SLOT(setMode2(bool)));
    sca_layout->insertWidget(sca_layout->count()-1,wdg);
}
}

void Status_Tool::fillStatus() {
    cmb_status1->clear();
    cmb_status2->clear();
    cmb_status3->clear();
    cmb_status4->clear();
    QSqlQuery query;
    if (rab_tn->isChecked()) {
        query.prepare("SELECT int_statusid,var_name FROM tfx_status WHERE bol_karte=true ORDER BY int_statusid");
    } else {
        query.prepare("SELECT int_statusid,var_name FROM tfx_status WHERE bol_bogen=true ORDER BY int_statusid");
    }
    query.exec();
    while (query.next()) {
        cmb_status1->addItem(query.value(1).toString(),query.value(0).toInt());
        cmb_status2->addItem(query.value(1).toString(),query.value(0).toInt());
        cmb_status3->addItem(query.value(1).toString(),query.value(0).toInt());
        cmb_status4->addItem(query.value(1).toString(),query.value(0).toInt());
    }
}
