#include <QSqlQuery>
#include "header/dlg_db_verband.h"
#include "header/dlg_db_land.h"
#include "../../global/header/_global.h"

Db_Verband_Dialog::Db_Verband_Dialog(int im_vid, QWidget* parent) : QDialog(parent) {
    vid = im_vid;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    readCountries();
    cmb_land->setCurrentIndex(0);
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    QObject::connect(but_addl, SIGNAL(clicked()), this, SLOT(addCountry()));
    if (vid != 0) {
        QSqlQuery query;
        query.prepare("SELECT int_laenderid, var_name, var_kuerzel FROM tfx_verbaende WHERE int_verbaendeid=?");
        query.bindValue(0,vid);
        query.exec();
        query.next();
        cmb_land->setCurrentIndex(cmb_land->findData(query.value(0).toInt()));
        txt_name->setText(query.value(1).toString());
        txt_kuerzel->setText(query.value(2).toString());
    }
}

void Db_Verband_Dialog::readCountries() {
    QString currtext = cmb_land->currentText();
    cmb_land->clear();
    QSqlQuery query("SELECT int_laenderid, var_name FROM tfx_laender ORDER BY var_name");
    while (query.next()) {
        cmb_land->addItem(query.value(1).toString(),query.value(0).toInt());
    }
    cmb_land->setCurrentIndex(cmb_land->findText(currtext));
}

void Db_Verband_Dialog::addCountry() {
    Db_Land_Dialog *pe = new Db_Land_Dialog(0,this);
    if(pe->exec() == 1) { readCountries(); }
}

void Db_Verband_Dialog::save() {
    QSqlQuery query6;
    if (vid == 0) {
        query6.prepare("INSERT INTO tfx_verbaende (int_laenderid,var_name,var_kuerzel) VALUES (?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_verbaende SET int_laenderid=?,var_name=?,var_kuerzel=? WHERE int_verbaendeid=?");
        query6.bindValue(3,vid);
    }
    query6.bindValue(0,cmb_land->itemData(cmb_land->currentIndex()));
    query6.bindValue(1,txt_name->text());
    query6.bindValue(2,txt_kuerzel->text());
    query6.exec();
    done(1);
}
