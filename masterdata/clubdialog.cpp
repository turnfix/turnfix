#include "clubdialog.h"
#include "persondialog.h"
#include "regiondialog.h"
#include "src/global/header/_global.h"
#include "ui_clubdialog.h"
#include <QSqlQuery>

ClubDialog::ClubDialog(int tid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ClubDialog)
{
    vnid = tid;
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    readPersons();
    readGaue();
    ui->cmb_gau->setCurrentIndex(0);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->sli_ort, SIGNAL(valueChanged(int)), this, SLOT(updateOrt()));
    connect(ui->txt_name, SIGNAL(textChanged(QString)), this, SLOT(updateSlider()));
    connect(ui->but_addp, SIGNAL(clicked()), this, SLOT(addPerson()));
    connect(ui->but_addg, SIGNAL(clicked()), this, SLOT(addGau()));
    connect(ui->cmb_gau, SIGNAL(currentIndexChanged(int)), this, SLOT(updateGauInfo()));

    if (tid != 0) {
        QSqlQuery query;
        query.prepare("SELECT int_personenid, var_name, int_start_ort, var_website, int_gaueid FROM tfx_vereine WHERE int_vereineid=?");
        query.bindValue(0,tid);
        query.exec();
        query.next();
        ui->cmb_ansprech->setCurrentIndex(ui->cmb_ansprech->findData(query.value(0).toInt()));
        ui->txt_name->setText(query.value(1).toString());
        ui->txt_web->setText(query.value(3).toString());
        ui->sli_ort->setValue(query.value(2).toInt());
        ui->cmb_gau->setCurrentIndex(ui->cmb_gau->findData(query.value(4).toInt()));
    }
}

ClubDialog::~ClubDialog()
{
    delete ui;
}

void ClubDialog::readPersons()
{
    QString currtext = ui->cmb_ansprech->currentText();
    ui->cmb_ansprech->clear();
    ui->cmb_ansprech->addItem("");
    QSqlQuery query("SELECT int_personenid, var_nachname || ', ' || var_vorname FROM tfx_personen ORDER BY var_nachname, var_vorname");
    while (query.next()) {
        ui->cmb_ansprech->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    ui->cmb_ansprech->setCurrentIndex(ui->cmb_ansprech->findText(currtext));
}

void ClubDialog::addPerson()
{
    PersonDialog *pe = new PersonDialog(0, this);
    if(pe->exec() == 1) { readPersons(); }
}

void ClubDialog::readGaue()
{
    QString currtext = ui->cmb_gau->currentText();
    ui->cmb_gau->clear();
    QSqlQuery query("SELECT int_gaueid, var_name FROM tfx_gaue ORDER BY var_name");
    while (query.next()) {
        ui->cmb_gau->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    ui->cmb_gau->setCurrentIndex(ui->cmb_gau->findText(currtext));
}

void ClubDialog::addGau()
{
    RegionDialog *pe = new RegionDialog(0, this);
    if(pe->exec() == 1) { readGaue(); }
}

void ClubDialog::save()
{
    QSqlQuery query6;
    if (vnid == 0) {
        query6.prepare("INSERT INTO tfx_vereine (int_personenid,var_name,int_start_ort,var_website,int_gaueid) VALUES (?,?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_vereine SET int_personenid=?,var_name=?,int_start_ort=?,var_website=?,int_gaueid=? WHERE int_vereineid=?");
        query6.bindValue(5,vnid);
    }
    query6.bindValue(0, ui->cmb_ansprech->itemData(ui->cmb_ansprech->currentIndex()));
    query6.bindValue(1, ui->txt_name->text());
    query6.bindValue(2, ui->sli_ort->value());
    query6.bindValue(3, ui->txt_web->text());
    query6.bindValue(4, ui->cmb_gau->itemData(ui->cmb_gau->currentIndex()));
    query6.exec();
    done(1);
}

void ClubDialog::updateSlider()
{
    ui->sli_ort->setMaximum(ui->txt_name->text().length() - 1);
    updateOrt();
}

void ClubDialog::updateOrt()
{
    ui->txt_pre_ort->setText(
        ui->txt_name->text().right(ui->txt_name->text().length() - ui->sli_ort->value()));
}

void ClubDialog::updateGauInfo()
{
    if (ui->cmb_gau->itemData(ui->cmb_gau->currentIndex()).toInt() > 0) {
        QSqlQuery query;
        query.prepare("SELECT tfx_verbaende.var_name, tfx_laender.var_name FROM tfx_gaue INNER JOIN tfx_verbaende ON tfx_gaue.int_verbaendeid = tfx_verbaende.int_verbaendeid INNER JOIN tfx_laender ON tfx_verbaende.int_laenderid = tfx_laender.int_laenderid WHERE int_gaueid=?");
        query.bindValue(0, ui->cmb_gau->itemData(ui->cmb_gau->currentIndex()));
        query.exec();
        query.next();
        ui->lbl_verband->setText(query.value(0).toString());
        ui->lbl_land->setText(query.value(1).toString());
    }
}
