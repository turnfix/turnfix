#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>
#include <QSqlQuery>
#include "header/dlg_select_club.h"
#include "../../global/header/_global.h"

Select_Club_Dialog::Select_Club_Dialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QObject::connect(but_select, SIGNAL(clicked()), this, SLOT(select1()));
    initData();
}

QStringList Select_Club_Dialog::returnVereine() {
    return vereine;
}

void Select_Club_Dialog::initData() {
    QSqlQuery query2;
    query2.prepare("SELECT tfx_vereine.int_vereineid, tfx_vereine.var_name FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid WHERE int_veranstaltungenid=? GROUP BY tfx_vereine.int_vereineid, tfx_vereine.var_name, tfx_vereine.int_start_ort, tfx_gruppen.int_gruppenid ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name");
    query2.bindValue(0,_global::checkHWK());
    query2.exec();
    while (query2.next()) {
        if (lst_clubs->findItems(query2.value(1).toString(),Qt::MatchExactly).count() == 0) {
        QListWidgetItem *item = new QListWidgetItem(query2.value(1).toString());
        item->setData(Qt::UserRole,query2.value(0).toInt());
        lst_clubs->addItem(item);
    }
    }
    lst_clubs->setCurrentRow(0);
}

void Select_Club_Dialog::select1() {
    for (int i=0; i<lst_clubs->selectedItems().size(); i++) {
        vereine << lst_clubs->selectedItems().at(i)->data(Qt::UserRole).toString();
    }
    done(1);
}
