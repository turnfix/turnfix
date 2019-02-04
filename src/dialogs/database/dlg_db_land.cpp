#include <QSqlQuery>
#include "header/dlg_db_land.h"
#include "../../global/header/_global.h"

Db_Land_Dialog::Db_Land_Dialog(int im_lid, QWidget* parent) : QDialog(parent) {
    lid = im_lid;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    if (lid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, var_kuerzel FROM tfx_laender WHERE int_laenderid=?");
        query.bindValue(0,lid);
        query.exec();
        query.next();
        txt_land->setText(query.value(0).toString());
        txt_kuerzel->setText(query.value(1).toString());
    }
}

void Db_Land_Dialog::save() {
    QSqlQuery query6;
    if (lid == 0) {
        query6.prepare("INSERT INTO tfx_laender (var_name,var_kuerzel) VALUES (?,?)");
    } else {
        query6.prepare("UPDATE tfx_laender SET var_name=?,var_kuerzel=? WHERE int_laenderid=?");
        query6.bindValue(2,lid);
    }
    query6.bindValue(0,txt_land->text());
    query6.bindValue(1,txt_kuerzel->text());
    query6.exec();
    done(1);
}
