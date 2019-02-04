#include <QSqlQuery>
#include <QTimer>
#include "header/wdg_wkstatus.h"
#include "../global/header/_global.h"

Status_Widget::Status_Widget(QWidget* parent) : QWidget(parent) {
    setupUi(this);
}

void Status_Widget::setWkVars(int wnr, int rnd) {
    wknr = wnr;
    runde = rnd;
}

void Status_Widget::setWkNr(QString swk) {
    lbl_wknr->setText(swk);
    swknr = swk;
    updateData();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    i = 300;
    timer->start(100);
}

void Status_Widget::setStatus1(QString status) {
    status1 = status;
    updateData();
}

void Status_Widget::setStatus2(QString status) {
    status2 = status;
    updateData();
}

void Status_Widget::setStatus3(QString status) {
    status3 = status;
    updateData();
}

void Status_Widget::setStatus4(QString status) {
    status4 = status;
    updateData();
}

void Status_Widget::setMode(bool tnm) {
    tn = tnm;
    updateData();
}

void Status_Widget::setMode2(bool rgm) {
    tn = !rgm;
    updateData();
}

void Status_Widget::updateData() {
    i=300;
    QSqlQuery query;
    if (tn) {
        query.prepare("SELECT COUNT(case when tfx_status.var_name=? then int_statusid end), COUNT(case when tfx_status.var_name=? then int_statusid end), COUNT(case when tfx_status.var_name=? then int_statusid end), COUNT(case when tfx_status.var_name=? then int_statusid end), COUNT(*) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_status USING (int_statusid) WHERE int_veranstaltungenid=? AND var_nummer=? AND int_runde=? AND bol_startet_nicht=false");
    } else {
        query.prepare("SELECT COUNT(case when tfx_status.var_name=? then int_statusid end), COUNT(case when tfx_status.var_name=? then int_statusid end), COUNT(case when tfx_status.var_name=? then int_statusid end), COUNT(case when tfx_status.var_name=? then int_statusid end), COUNT(*) FROM tfx_riegen_x_disziplinen INNER JOIN (SELECT var_riege FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND var_nummer=? AND int_runde=? AND bol_startet_nicht=false GROUP BY var_riege) AS riegen USING (var_riege) INNER JOIN tfx_status USING (int_statusid) WHERE int_veranstaltungenid=? AND int_runde=?");
        query.bindValue(7, _global::checkHWK());
        query.bindValue(8, runde);
    }
    query.bindValue(0, status1);
    query.bindValue(1, status2);
    query.bindValue(2, status3);
    query.bindValue(3, status4);
    query.bindValue(4, _global::checkHWK());
    query.bindValue(5, swknr);
    query.bindValue(6, runde);
    query.exec();
    query.next();
    prb_status1->setMaximum(query.value(4).toInt());
    prb_status1->setValue(query.value(0).toInt());
    prb_status2->setMaximum(query.value(4).toInt());
    prb_status2->setValue(query.value(1).toInt());
    prb_status3->setMaximum(query.value(4).toInt());
    prb_status3->setValue(query.value(2).toInt());
    prb_status4->setMaximum(query.value(4).toInt());
    prb_status4->setValue(query.value(3).toInt());
}

void Status_Widget::timerUpdate() {
    progress->setValue(i);
    if (i == 0) {
        updateData();
    }
    i--;
}
