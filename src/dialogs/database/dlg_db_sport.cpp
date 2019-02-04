#include <QSqlQuery>
#include "header/dlg_db_sport.h"
#include "../../global/header/_global.h"

Db_Sp_Dialog::Db_Sp_Dialog(int tid, QWidget* parent) : QDialog(parent) {
    tnid = tid;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    if (tid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name FROM tfx_sport WHERE int_sportid=?");
        query.bindValue(0,tid);
        query.exec();
        query.next();
        txt_name->setText(query.value(0).toString());
    }
}

void Db_Sp_Dialog::save() {
    QSqlQuery query6;
    if (tnid == 0) {
        query6.prepare("INSERT INTO tfx_sport (var_name) VALUES (?)");
    } else {
        query6.prepare("UPDATE tfx_sport SET var_name=? WHERE int_sportid=?");
        query6.bindValue(1,tnid);
    }
    query6.bindValue(0,txt_name->text());
    query6.exec();
    done(1);
}
