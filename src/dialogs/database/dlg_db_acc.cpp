#include <QSqlQuery>
#include "header/dlg_db_acc.h"
#include "../../global/header/_global.h"

Db_Acc_Dialog::Db_Acc_Dialog(int aid, QWidget* parent) : QDialog(parent) {
    acid = aid;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    if (acid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, var_kontonummer, var_blz, var_bank, var_inhabe FROM tfx_konten WHERE int_kontenid=?");
        query.bindValue(0,acid);
        query.exec();
        query.next();
        txt_name->setText(query.value(0).toString());
        txt_number->setText(query.value(1).toString());
        txt_blz->setText(query.value(2).toString());
        txt_bank->setText(query.value(3).toString());
        txt_owner->setText(query.value(4).toString());
    }
}

void Db_Acc_Dialog::save() {
    QSqlQuery query6;
    if (acid == 0) {
        query6.prepare("INSERT INTO tfx_konten (var_name,var_kontonummer,var_blz,var_bank,var_inhabe) VALUES (?,?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_konten SET var_name=?, var_kontonummer=?, var_blz=?, var_bank=?, var_inhabe=? WHERE int_kontenid=?");
        query6.bindValue(5,acid);
    }
    query6.bindValue(0,txt_name->text());
    query6.bindValue(1,txt_number->text());
    query6.bindValue(2,txt_blz->text().replace(" ",""));
    query6.bindValue(3,txt_bank->text());
    query6.bindValue(4,txt_owner->text());
    query6.exec();
    done(1);
}
