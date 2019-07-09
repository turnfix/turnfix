#include "countrydialog.h"
#include "src/global/header/_global.h"
#include "ui_countrydialog.h"
#include <QSqlQuery>

CountryDialog::CountryDialog(int im_lid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CountryDialog)
{
    lid = im_lid;
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);
    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));

    if (lid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, var_kuerzel FROM tfx_laender WHERE int_laenderid=?");
        query.bindValue(0,lid);
        query.exec();
        query.next();
        ui->txt_land->setText(query.value(0).toString());
        ui->txt_kuerzel->setText(query.value(1).toString());
    }
}

CountryDialog::~CountryDialog()
{
    delete ui;
}

void CountryDialog::save()
{
    QSqlQuery query6;
    if (lid == 0) {
        query6.prepare("INSERT INTO tfx_laender (var_name,var_kuerzel) VALUES (?,?)");
    } else {
        query6.prepare("UPDATE tfx_laender SET var_name=?,var_kuerzel=? WHERE int_laenderid=?");
        query6.bindValue(2,lid);
    }
    query6.bindValue(0, ui->txt_land->text());
    query6.bindValue(1, ui->txt_kuerzel->text());
    query6.exec();
    done(1);
}
