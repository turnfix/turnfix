#include <QSqlQuery>
#include "model/objects/event.h"
#include "header/dlg_select_rg.h"
#include "../../global/header/_global.h"

Select_Rg_Dialog::Select_Rg_Dialog(Event *event, QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    this->event = event;

    connect(but_select, SIGNAL(clicked()), this, SLOT(select1()));
    initData();
}

void Select_Rg_Dialog::initData() {
    QSqlQuery query2;
    query2.prepare("SELECT DISTINCT(var_riege) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? ORDER BY var_riege");
    query2.bindValue(0, this->event->getMainEventId());
    query2.bindValue(1, this->event->getRound());
    query2.exec();
    while (query2.next()) {
        QListWidgetItem *itm = new QListWidgetItem(query2.value(0).toString());
        lst_rg->addItem(itm);
    }
}

void Select_Rg_Dialog::select1() {
    rg.clear();
    QList<QListWidgetItem*> selected = lst_rg->selectedItems();
    for (int i=0;i<selected.size();i++) {
        rg.append(selected.at(i)->text());
    }
    done(1);
}

QStringList Select_Rg_Dialog::getRg() {
    return rg;
}
