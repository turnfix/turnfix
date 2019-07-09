#include "venuedialog.h"
#include "src/global/header/_global.h"
#include "ui_venuedialog.h"
#include <QSqlQuery>

VenueDialog::VenueDialog(int loc, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VenueDialog)
{
    lid = loc;
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));

    if (lid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, var_adresse, var_plz, var_ort FROM tfx_wettkampforte WHERE int_wettkampforteid=?");
        query.bindValue(0,lid);
        query.exec();
        query.next();
        ui->txt_name->setText(query.value(0).toString());
        ui->txt_adress->setText(query.value(1).toString());
        ui->txt_plz->setText(query.value(2).toString());
        ui->txt_ort->setText(query.value(3).toString());
    }
}

VenueDialog::~VenueDialog()
{
    delete ui;
}

void VenueDialog::save()
{
    QSqlQuery query6;
    if (lid == 0) {
        query6.prepare("INSERT INTO tfx_wettkampforte (var_name,var_adresse,var_plz,var_ort) VALUES (?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_wettkampforte SET var_name=?, var_adresse=?, var_plz=?, var_ort=? WHERE int_wettkampforteid=?");
        query6.bindValue(4,lid);
    }
    query6.bindValue(0, ui->txt_name->text());
    query6.bindValue(1, ui->txt_adress->text());
    query6.bindValue(2, ui->txt_plz->text());
    query6.bindValue(3, ui->txt_ort->text());
    query6.exec();
    done(1);
}
