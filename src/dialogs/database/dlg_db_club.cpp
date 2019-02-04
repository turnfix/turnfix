#include <QSqlQuery>
#include "header/dlg_db_club.h"
#include "header/dlg_db_person.h"
#include "header/dlg_db_gau.h"
#include "../../global/header/_global.h"

Db_Club_Dialog::Db_Club_Dialog(int tid, QWidget* parent) : QDialog(parent) {
    vnid = tid;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    readPersons();
    readGaue();
    cmb_gau->setCurrentIndex(0);
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    QObject::connect(sli_ort, SIGNAL(valueChanged(int)), this, SLOT(updateOrt()));
    QObject::connect(txt_name, SIGNAL(textChanged(QString)), this, SLOT(updateSlider()));
    QObject::connect(but_addp, SIGNAL(clicked()), this, SLOT(addPerson()));
    QObject::connect(but_addg, SIGNAL(clicked()), this, SLOT(addGau()));
    QObject::connect(cmb_gau, SIGNAL(currentIndexChanged(int)), this, SLOT(updateGauInfo()));
    if (tid != 0) {
        QSqlQuery query;
        query.prepare("SELECT int_personenid, var_name, int_start_ort, var_website, int_gaueid FROM tfx_vereine WHERE int_vereineid=?");
        query.bindValue(0,tid);
        query.exec();
        query.next();
        cmb_ansprech->setCurrentIndex(cmb_ansprech->findData(query.value(0).toInt()));
        txt_name->setText(query.value(1).toString());
        txt_web->setText(query.value(3).toString());
        sli_ort->setValue(query.value(2).toInt());
        cmb_gau->setCurrentIndex(cmb_gau->findData(query.value(4).toInt()));
    }
}

void Db_Club_Dialog::readPersons() {
    QString currtext = cmb_ansprech->currentText();
    cmb_ansprech->clear();
    cmb_ansprech->addItem("");
    QSqlQuery query("SELECT int_personenid, var_nachname || ', ' || var_vorname FROM tfx_personen ORDER BY var_nachname, var_vorname");
    while (query.next()) {
        cmb_ansprech->addItem(query.value(1).toString(),query.value(0).toInt());
    }
    cmb_ansprech->setCurrentIndex(cmb_ansprech->findText(currtext));
}

void Db_Club_Dialog::addPerson() {
    Db_Pe_Dialog *pe = new Db_Pe_Dialog(0,this);
    if(pe->exec() == 1) { readPersons(); }
}

void Db_Club_Dialog::readGaue() {
    QString currtext = cmb_gau->currentText();
    cmb_gau->clear();
    QSqlQuery query("SELECT int_gaueid, var_name FROM tfx_gaue ORDER BY var_name");
    while (query.next()) {
        cmb_gau->addItem(query.value(1).toString(),query.value(0).toInt());
    }
    cmb_gau->setCurrentIndex(cmb_gau->findText(currtext));
}

void Db_Club_Dialog::addGau() {
    Db_Gau_Dialog *pe = new Db_Gau_Dialog(0,this);
    if(pe->exec() == 1) { readGaue(); }
}

void Db_Club_Dialog::save() {
    QSqlQuery query6;
    if (vnid == 0) {
        query6.prepare("INSERT INTO tfx_vereine (int_personenid,var_name,int_start_ort,var_website,int_gaueid) VALUES (?,?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_vereine SET int_personenid=?,var_name=?,int_start_ort=?,var_website=?,int_gaueid=? WHERE int_vereineid=?");
        query6.bindValue(5,vnid);
    }
    query6.bindValue(0,cmb_ansprech->itemData(cmb_ansprech->currentIndex()));
    query6.bindValue(1,txt_name->text());
    query6.bindValue(2,sli_ort->value());
    query6.bindValue(3,txt_web->text());
    query6.bindValue(4,cmb_gau->itemData(cmb_gau->currentIndex()));
    query6.exec();
    done(1);
}

void Db_Club_Dialog::updateSlider() {
    sli_ort->setMaximum(txt_name->text().length()-1);
    updateOrt();
}

void Db_Club_Dialog::updateOrt() {
    txt_pre_ort->setText(txt_name->text().right(txt_name->text().length()-sli_ort->value()));
}

void Db_Club_Dialog::updateGauInfo() {
    if (cmb_gau->itemData(cmb_gau->currentIndex()).toInt() > 0) {
        QSqlQuery query;
        query.prepare("SELECT tfx_verbaende.var_name, tfx_laender.var_name FROM tfx_gaue INNER JOIN tfx_verbaende ON tfx_gaue.int_verbaendeid = tfx_verbaende.int_verbaendeid INNER JOIN tfx_laender ON tfx_verbaende.int_laenderid = tfx_laender.int_laenderid WHERE int_gaueid=?");
        query.bindValue(0,cmb_gau->itemData(cmb_gau->currentIndex()));
        query.exec();
        query.next();
        lbl_verband->setText(query.value(0).toString());
        lbl_land->setText(query.value(1).toString());
    }
}
