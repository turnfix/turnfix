#include <QSqlQuery>
#include "header/dlg_select_wk.h"
#include "../../global/header/_global.h"

Select_Wk_Dialog::Select_Wk_Dialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QObject::connect(but_select, SIGNAL(clicked()), this, SLOT(select1()));
    initData();
}

void Select_Wk_Dialog::initData() {
    QSqlQuery query2;
    query2.prepare("SELECT var_nummer || ' ' || var_name, var_nummer FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY var_nummer");
    query2.bindValue(0,_global::checkHWK());
    query2.exec();
    while (query2.next()) {
        cmb_dis->addItem(query2.value(0).toString(),query2.value(1).toString());
    }
}

void Select_Wk_Dialog::select1() {
    wk = QVariant(cmb_dis->itemData(cmb_dis->currentIndex())).toString();
    done(1);
}

QString Select_Wk_Dialog::getWk() {
    return wk;
}
