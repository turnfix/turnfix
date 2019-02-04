#include <QSqlQuery>
#include "header/dlg_db_loc.h"
#include "../../global/header/_global.h"

Db_Loc_Dialog::Db_Loc_Dialog(int loc, QWidget* parent) : QDialog(parent) {
    lid = loc;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    if (lid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, var_adresse, var_plz, var_ort FROM tfx_wettkampforte WHERE int_wettkampforteid=?");
        query.bindValue(0,lid);
        query.exec();
        query.next();
        txt_name->setText(query.value(0).toString());
        txt_adress->setText(query.value(1).toString());
        txt_plz->setText(query.value(2).toString());
        txt_ort->setText(query.value(3).toString());
    }
}

void Db_Loc_Dialog::save() {
    QSqlQuery query6;
    if (lid == 0) {
        query6.prepare("INSERT INTO tfx_wettkampforte (var_name,var_adresse,var_plz,var_ort) VALUES (?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_wettkampforte SET var_name=?, var_adresse=?, var_plz=?, var_ort=? WHERE int_wettkampforteid=?");
        query6.bindValue(4,lid);
    }
    query6.bindValue(0,txt_name->text());
    query6.bindValue(1,txt_adress->text());
    query6.bindValue(2,txt_plz->text());
    query6.bindValue(3,txt_ort->text());
    query6.exec();
    done(1);
}
