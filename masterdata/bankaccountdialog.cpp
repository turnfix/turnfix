#include "bankaccountdialog.h"
#include "src/global/header/_global.h"
#include "ui_bankaccountdialog.h"
#include <QSqlQuery>

BankAccountDialog::BankAccountDialog(int aid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BankAccountDialog)
{
    acid = aid;
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));

    if (acid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, var_kontonummer, var_blz, var_bank, var_inhabe FROM tfx_konten WHERE int_kontenid=?");
        query.bindValue(0,acid);
        query.exec();
        query.next();
        ui->txt_name->setText(query.value(0).toString());
        ui->txt_number->setText(query.value(1).toString());
        ui->txt_blz->setText(query.value(2).toString());
        ui->txt_bank->setText(query.value(3).toString());
        ui->txt_owner->setText(query.value(4).toString());
    }
}

BankAccountDialog::~BankAccountDialog()
{
    delete ui;
}

void BankAccountDialog::save()
{
    QSqlQuery query6;
    if (acid == 0) {
        query6.prepare("INSERT INTO tfx_konten (var_name,var_kontonummer,var_blz,var_bank,var_inhabe) VALUES (?,?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_konten SET var_name=?, var_kontonummer=?, var_blz=?, var_bank=?, var_inhabe=? WHERE int_kontenid=?");
        query6.bindValue(5,acid);
    }
    query6.bindValue(0, ui->txt_name->text());
    query6.bindValue(1, ui->txt_number->text());
    query6.bindValue(2, ui->txt_blz->text().replace(" ", ""));
    query6.bindValue(3, ui->txt_bank->text());
    query6.bindValue(4, ui->txt_owner->text());
    query6.exec();
    done(1);
}
