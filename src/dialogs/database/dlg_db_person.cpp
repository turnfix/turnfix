#include <QSqlQuery>
#include "header/dlg_db_person.h"
#include "../../global/header/_global.h"

Db_Pe_Dialog::Db_Pe_Dialog(int tid, QWidget* parent) : QDialog(parent) {
    tnid = tid;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    if (tid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_nachname || ', ' || var_vorname, var_adresse, var_plz, var_ort, var_telefon, var_fax, var_email FROM tfx_personen WHERE int_personenid=?");
        query.bindValue(0,tid);
        query.exec();
        query.next();
        txt_name->setText(query.value(0).toString());
        txt_adress->setText(query.value(1).toString());
        txt_plz->setText(query.value(2).toString());
        txt_ort->setText(query.value(3).toString());
        txt_phone->setText(query.value(4).toString());
        txt_fax->setText(query.value(5).toString());
        txt_mail->setText(query.value(6).toString());
    }
}

void Db_Pe_Dialog::save() {
    QSqlQuery query6;
    if (tnid == 0) {
        query6.prepare("INSERT INTO tfx_personen (var_vorname,var_nachname,var_adresse,var_plz,var_ort,var_telefon,var_fax,var_email) VALUES (?,?,?,?,?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_personen SET var_vorname=?, var_nachname=?, var_adresse=?, var_plz=?, var_ort=?, var_telefon=?, var_fax=?, var_email=? WHERE int_personenid=?");
        query6.bindValue(8,tnid);
    }
    query6.bindValue(0,_global::nameSplit(txt_name->text()).at(0));
    query6.bindValue(1,_global::nameSplit(txt_name->text()).at(1));
    query6.bindValue(2,txt_adress->text());
    query6.bindValue(3,txt_plz->text());
    query6.bindValue(4,txt_ort->text());
    query6.bindValue(5,txt_phone->text());
    query6.bindValue(6,txt_fax->text());
    query6.bindValue(7,txt_mail->text());
    query6.exec();
    done(1);
}
