#include "divisiondialog.h"
#include "src/global/header/_global.h"
#include "ui_divisiondialog.h"
#include <QSqlQuery>

DivisionDialog::DivisionDialog(int tid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DivisionDialog)
{
    tnid = tid;
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));

    if (tid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, bol_maennlich, bol_weiblich FROM tfx_bereiche WHERE int_bereicheid=?");
        query.bindValue(0,tid);
        query.exec();
        query.next();
        ui->txt_name->setText(query.value(0).toString());
        ui->chk_m->setChecked(query.value(1).toBool());
        ui->chk_w->setChecked(query.value(2).toBool());
    }
}

DivisionDialog::~DivisionDialog()
{
    delete ui;
}

void DivisionDialog::save()
{
    QSqlQuery query6;
    if (tnid == 0) {
        query6.prepare("INSERT INTO tfx_bereiche (var_name,bol_maennlich,bol_weiblich) VALUES (?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_bereiche SET var_name=?, bol_maennlich=?, bol_weiblich=? WHERE int_bereicheid=?");
        query6.bindValue(3,tnid);
    }
    query6.bindValue(0, ui->txt_name->text());
    query6.bindValue(1, ui->chk_m->isChecked());
    query6.bindValue(2, ui->chk_w->isChecked());
    query6.exec();
    done(1);
}
