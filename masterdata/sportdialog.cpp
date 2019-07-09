#include "sportdialog.h"
#include "src/global/header/_global.h"
#include "ui_sportdialog.h"
#include <QSqlQuery>

SportDialog::SportDialog(int tid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SportDialog)
{
    tnid = tid;

    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    connect(ui->but_save, &QPushButton::clicked, this, &SportDialog::save);

    if (tid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name FROM tfx_sport WHERE int_sportid=?");
        query.bindValue(0,tid);
        query.exec();
        query.next();
        ui->txt_name->setText(query.value(0).toString());
    }
}

SportDialog::~SportDialog()
{
    delete ui;
}

void SportDialog::save()
{
    QSqlQuery query6;
    if (tnid == 0) {
        query6.prepare("INSERT INTO tfx_sport (var_name) VALUES (?)");
    } else {
        query6.prepare("UPDATE tfx_sport SET var_name=? WHERE int_sportid=?");
        query6.bindValue(1,tnid);
    }
    query6.bindValue(0, ui->txt_name->text());
    query6.exec();
    done(1);
}
