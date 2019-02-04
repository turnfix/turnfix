#include <QSqlQuery>
#include "header/dlg_db_penalty.h"
#include "../../global/header/_global.h"

Db_Penalty_Dialog::Db_Penalty_Dialog(int penid, QWidget* parent) : QDialog(parent) {
    pid = penid;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    if (pid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, rel_abzug FROM tfx_mannschaften_abzug WHERE int_mannschaften_abzugid=?");
        query.bindValue(0,pid);
        query.exec();
        query.next();
        txt_name->setText(query.value(0).toString());
        dsb_abzug->setValue(query.value(1).toDouble());
    }
}

void Db_Penalty_Dialog::save() {
    QSqlQuery query6;
    if (pid == 0) {
        query6.prepare("INSERT INTO tfx_mannschaften_abzug (var_name,rel_abzug) VALUES (?,?)");
    } else {
        query6.prepare("UPDATE tfx_mannschaften_abzug SET var_name=?, rel_abzug=? WHERE int_mannschaften_abzugid=?");
        query6.bindValue(2,pid);
    }
    query6.bindValue(0,txt_name->text());
    query6.bindValue(1,dsb_abzug->value());
    query6.exec();
    done(1);
}

