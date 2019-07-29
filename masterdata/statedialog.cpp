#include "statedialog.h"
#include "countrydialog.h"
#include "src/global/header/_global.h"
#include "ui_statedialog.h"
#include <QSqlQuery>

StateDialog::StateDialog(int im_vid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StateDialog)
{
    vid = im_vid;
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    readCountries();
    ui->cmb_land->setCurrentIndex(0);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_addl, SIGNAL(clicked()), this, SLOT(addCountry()));

    if (vid != 0) {
        QSqlQuery query;
        query.prepare("SELECT int_laenderid, var_name, var_kuerzel FROM tfx_verbaende WHERE int_verbaendeid=?");
        query.bindValue(0,vid);
        query.exec();
        query.next();
        ui->cmb_land->setCurrentIndex(ui->cmb_land->findData(query.value(0).toInt()));
        ui->txt_name->setText(query.value(1).toString());
        ui->txt_kuerzel->setText(query.value(2).toString());
    }
}

StateDialog::~StateDialog()
{
    delete ui;
}

void StateDialog::readCountries()
{
    QString currtext = ui->cmb_land->currentText();
    ui->cmb_land->clear();
    QSqlQuery query("SELECT int_laenderid, var_name FROM tfx_laender ORDER BY var_name");
    while (query.next()) {
        ui->cmb_land->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    ui->cmb_land->setCurrentIndex(ui->cmb_land->findText(currtext));
}

void StateDialog::addCountry()
{
    //    CountryDialog *pe = new CountryDialog(nullptr, m_em, this);
    //    if (pe->exec() == 1) {
    //        readCountries();
    //    }
}

void StateDialog::save()
{
    QSqlQuery query6;
    if (vid == 0) {
        query6.prepare("INSERT INTO tfx_verbaende (int_laenderid,var_name,var_kuerzel) VALUES (?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_verbaende SET int_laenderid=?,var_name=?,var_kuerzel=? WHERE int_verbaendeid=?");
        query6.bindValue(3,vid);
    }
    query6.bindValue(0, ui->cmb_land->itemData(ui->cmb_land->currentIndex()));
    query6.bindValue(1, ui->txt_name->text());
    query6.bindValue(2, ui->txt_kuerzel->text());
    query6.exec();
    done(1);
}
