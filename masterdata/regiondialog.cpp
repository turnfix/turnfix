#include "regiondialog.h"
#include "src/global/header/_global.h"
#include "statedialog.h"
#include "ui_regiondialog.h"
#include <QSqlQuery>

RegionDialog::RegionDialog(int im_vid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegionDialog)
{
    vid = im_vid;
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);
    readVerbaende();

    ui->cmb_verband->setCurrentIndex(0);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_addl, SIGNAL(clicked()), this, SLOT(addVerband()));

    if (vid != 0) {
        QSqlQuery query;
        query.prepare("SELECT int_verbaendeid, var_name, var_kuerzel FROM tfx_gaue WHERE int_gaueid=?");
        query.bindValue(0,vid);
        query.exec();
        query.next();
        ui->cmb_verband->setCurrentIndex(ui->cmb_verband->findData(query.value(0).toInt()));
        ui->txt_name->setText(query.value(1).toString());
        ui->txt_kuerzel->setText(query.value(2).toString());
    }
}

RegionDialog::~RegionDialog()
{
    delete ui;
}

void RegionDialog::readVerbaende()
{
    QString currtext = ui->cmb_verband->currentText();
    ui->cmb_verband->clear();
    QSqlQuery query("SELECT int_verbaendeid, var_name FROM tfx_verbaende ORDER BY var_name");
    while (query.next()) {
        ui->cmb_verband->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    ui->cmb_verband->setCurrentIndex(ui->cmb_verband->findText(currtext));
}

void RegionDialog::addVerband()
{
    StateDialog *pe = new StateDialog(0, this);
    if(pe->exec() == 1) { readVerbaende(); }
}

void RegionDialog::save()
{
    QSqlQuery query6;
    if (vid == 0) {
        query6.prepare("INSERT INTO tfx_gaue (int_verbaendeid,var_name,var_kuerzel) VALUES (?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_gaue SET int_verbaendeid=?,var_name=?,var_kuerzel=? WHERE int_gaueid=?");
        query6.bindValue(3,vid);
    }
    query6.bindValue(0, ui->cmb_verband->itemData(ui->cmb_verband->currentIndex()));
    query6.bindValue(1, ui->txt_name->text());
    query6.bindValue(2, ui->txt_kuerzel->text());
    query6.exec();
    done(1);
}
