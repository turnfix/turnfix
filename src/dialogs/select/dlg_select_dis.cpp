#include <QSqlQuery>
#include "header/dlg_select_dis.h"
#include "../../global/header/_global.h"

Select_Dis_Dialog::Select_Dis_Dialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QObject::connect(but_select, SIGNAL(clicked()), this, SLOT(select1()));
    initData();
}

void Select_Dis_Dialog::initData() {
    QSqlQuery query2;
    query2.prepare("SELECT DISTINCT int_disziplinenid, tfx_disziplinen.var_name, var_icon, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp FROM tfx_disziplinen INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? GROUP BY int_disziplinenid, tfx_wettkaempfe.bol_kp, tfx_wettkaempfe_x_disziplinen.bol_kp, tfx_disziplinen.var_name, var_icon ORDER BY tfx_disziplinen.var_name, kp");
    query2.bindValue(0,_global::checkHWK());
    query2.exec();
    while (query2.next()) {
        QString name = query2.value(1).toString();
        if (query2.value(3).toInt()==0) {
            QListWidgetItem *itm = new QListWidgetItem(name);
            itm->setIcon(QIcon(query2.value(2).toString()));
            itm->setData(Qt::UserRole,query2.value(0).toInt());
            lst_dis->addItem(itm);
        } else {
            QListWidgetItem *itm = new QListWidgetItem(name+" (Pflicht)");
            itm->setIcon(QIcon(query2.value(2).toString()));
            itm->setData(Qt::UserRole,query2.value(0).toInt());
            lst_dis->addItem(itm);
            QListWidgetItem *itm2 = new QListWidgetItem(name+" (K�r)");
            itm2->setIcon(QIcon(query2.value(2).toString()));
            itm2->setData(Qt::UserRole,query2.value(0).toInt());
            lst_dis->addItem(itm2);
        }
    }
    lst_dis->selectAll();
}

void Select_Dis_Dialog::select1() {
    dis.clear();
    QList<QListWidgetItem*> selected = lst_dis->selectedItems();
    for (int i=0;i<selected.size();i++) {
        QList<int> lst;
        lst.append(selected.at(i)->data(Qt::UserRole).toInt());
        if (selected.at(i)->text().right(5) == "(K�r)") {
            lst.append(1);
        } else {
            lst.append(0);
        }
        dis.append(lst);
    }
    done(1);
}

QList< QList<int> > Select_Dis_Dialog::getDis() {
    return dis;
}
