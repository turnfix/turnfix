#include <QSqlQuery>
#include "header/dlg_db_sec.h"
#include "../../global/header/_global.h"

Db_Sec_Dialog::Db_Sec_Dialog(int tid, QWidget* parent) : QDialog(parent) {
    tnid = tid;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    if (tid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, bol_maennlich, bol_weiblich FROM tfx_bereiche WHERE int_bereicheid=?");
        query.bindValue(0,tid);
        query.exec();
        query.next();
        txt_name->setText(query.value(0).toString());
        chk_m->setChecked(query.value(1).toBool());
        chk_w->setChecked(query.value(2).toBool());
    }
}

void Db_Sec_Dialog::save() {
    QSqlQuery query6;
    if (tnid == 0) {
        query6.prepare("INSERT INTO tfx_bereiche (var_name,bol_maennlich,bol_weiblich) VALUES (?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_bereiche SET var_name=?, bol_maennlich=?, bol_weiblich=? WHERE int_bereicheid=?");
        query6.bindValue(3,tnid);
    }
    query6.bindValue(0,txt_name->text());
    query6.bindValue(1,chk_m->isChecked());
    query6.bindValue(2,chk_w->isChecked());
    query6.exec();
    done(1);
}
