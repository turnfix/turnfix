#include "penaltydialog.h"
#include "src/global/header/_global.h"
#include "ui_penaltydialog.h"
#include <QSqlQuery>

PenaltyDialog::PenaltyDialog(int penid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PenaltyDialog)
{
    pid = penid;
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));

    if (pid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, rel_abzug FROM tfx_mannschaften_abzug WHERE int_mannschaften_abzugid=?");
        query.bindValue(0,pid);
        query.exec();
        query.next();
        ui->txt_name->setText(query.value(0).toString());
        ui->dsb_abzug->setValue(query.value(1).toDouble());
    }
}

PenaltyDialog::~PenaltyDialog()
{
    delete ui;
}

void PenaltyDialog::save()
{
    QSqlQuery query6;
    if (pid == 0) {
        query6.prepare("INSERT INTO tfx_mannschaften_abzug (var_name,rel_abzug) VALUES (?,?)");
    } else {
        query6.prepare("UPDATE tfx_mannschaften_abzug SET var_name=?, rel_abzug=? WHERE int_mannschaften_abzugid=?");
        query6.bindValue(2,pid);
    }
    query6.bindValue(0, ui->txt_name->text());
    query6.bindValue(1, ui->dsb_abzug->value());
    query6.exec();
    done(1);
}
