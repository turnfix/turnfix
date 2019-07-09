#include "persondialog.h"
#include "src/global/header/_global.h"
#include "ui_persondialog.h"
#include <QSqlQuery>

PersonDialog::PersonDialog(int tid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PersonDialog)
{
    tnid = tid;
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));

    if (tid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_nachname || ', ' || var_vorname, var_adresse, var_plz, var_ort, var_telefon, var_fax, var_email FROM tfx_personen WHERE int_personenid=?");
        query.bindValue(0,tid);
        query.exec();
        query.next();
        ui->txt_name->setText(query.value(0).toString());
        ui->txt_adress->setText(query.value(1).toString());
        ui->txt_plz->setText(query.value(2).toString());
        ui->txt_ort->setText(query.value(3).toString());
        ui->txt_phone->setText(query.value(4).toString());
        ui->txt_fax->setText(query.value(5).toString());
        ui->txt_mail->setText(query.value(6).toString());
    }
}

PersonDialog::~PersonDialog()
{
    delete ui;
}

void PersonDialog::save()
{
    QSqlQuery query6;
    if (tnid == 0) {
        query6.prepare("INSERT INTO tfx_personen (var_vorname,var_nachname,var_adresse,var_plz,var_ort,var_telefon,var_fax,var_email) VALUES (?,?,?,?,?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_personen SET var_vorname=?, var_nachname=?, var_adresse=?, var_plz=?, var_ort=?, var_telefon=?, var_fax=?, var_email=? WHERE int_personenid=?");
        query6.bindValue(8,tnid);
    }
    query6.bindValue(0, _global::nameSplit(ui->txt_name->text()).at(0));
    query6.bindValue(1, _global::nameSplit(ui->txt_name->text()).at(1));
    query6.bindValue(2, ui->txt_adress->text());
    query6.bindValue(3, ui->txt_plz->text());
    query6.bindValue(4, ui->txt_ort->text());
    query6.bindValue(5, ui->txt_phone->text());
    query6.bindValue(6, ui->txt_fax->text());
    query6.bindValue(7, ui->txt_mail->text());
    query6.exec();
    done(1);
}
