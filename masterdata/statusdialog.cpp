#include "statusdialog.h"
#include "src/global/header/_global.h"
#include "ui_statusdialog.h"
#include <QColorDialog>
#include <QSqlQuery>

StatusDialog::StatusDialog(int tid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StatusDialog)
{
    tnid = tid;
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_color, SIGNAL(clicked()), this, SLOT(colorChooser()));

    if (tid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, ary_colorcode bol_bogen, bol_karte FROM tfx_status WHERE int_statusid=?");
        query.bindValue(0,tid);
        query.exec();
        query.next();
        ui->txt_name->setText(query.value(0).toString());
        QList<int> color = _global::splitColorArray(query.value(1).toString());
        ui->lbl_color->setStyleSheet("background-color: rgb(" + QString().setNum(color.at(0)) + ", "
                                     + QString().setNum(color.at(1)) + ", "
                                     + QString().setNum(color.at(2)) + ");");
        ui->chk_karte->setChecked(query.value(2).toBool());
        ui->chk_bogen->setChecked(query.value(3).toBool());
    }
}

StatusDialog::~StatusDialog()
{
    delete ui;
}

void StatusDialog::colorChooser()
{
    QColorDialog colc(col,this);
    colc.exec();
    col = colc.selectedColor();
    ui->lbl_color->setStyleSheet("background-color: rgb(" + QString().setNum(col.red()) + ", "
                                 + QString().setNum(col.green()) + ", "
                                 + QString().setNum(col.blue()) + ");");
}

void StatusDialog::save()
{
    QSqlQuery query6;
    if (tnid == 0) {
        query6.prepare("INSERT INTO tfx_status (var_name,ary_colorcode,bol_bogen,bol_karte) VALUES (?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_status SET var_name=?, ary_colorcode=?, bol_bogen=?, bol_karte=? WHERE int_statusid=?");
        query6.bindValue(4,tnid);
    }
    query6.bindValue(0, ui->txt_name->text());
    query6.bindValue(1,"{" + QString().setNum(col.red()) + "," + QString().setNum(col.green()) + "," + QString().setNum(col.blue()) + "}");
    query6.bindValue(2, ui->chk_karte->isChecked());
    query6.bindValue(3, ui->chk_bogen->isChecked());
    query6.exec();
    done(1);
}
