#include <QSqlQuery>
#include "header/dlg_db_gau.h"
#include "header/dlg_db_verband.h"
#include "../../global/header/_global.h"

Db_Gau_Dialog::Db_Gau_Dialog(int im_vid, QWidget* parent) : QDialog(parent) {
    vid = im_vid;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    readVerbaende();
    cmb_verband->setCurrentIndex(0);
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    QObject::connect(but_addl, SIGNAL(clicked()), this, SLOT(addVerband()));
    if (vid != 0) {
        QSqlQuery query;
        query.prepare("SELECT int_verbaendeid, var_name, var_kuerzel FROM tfx_gaue WHERE int_gaueid=?");
        query.bindValue(0,vid);
        query.exec();
        query.next();
        cmb_verband->setCurrentIndex(cmb_verband->findData(query.value(0).toInt()));
        txt_name->setText(query.value(1).toString());
        txt_kuerzel->setText(query.value(2).toString());
    }
}

void Db_Gau_Dialog::readVerbaende() {
    QString currtext = cmb_verband->currentText();
    cmb_verband->clear();
    QSqlQuery query("SELECT int_verbaendeid, var_name FROM tfx_verbaende ORDER BY var_name");
    while (query.next()) {
        cmb_verband->addItem(query.value(1).toString(),query.value(0).toInt());
    }
    cmb_verband->setCurrentIndex(cmb_verband->findText(currtext));
}

void Db_Gau_Dialog::addVerband() {
    Db_Verband_Dialog *pe = new Db_Verband_Dialog(0,this);
    if(pe->exec() == 1) { readVerbaende(); }
}

void Db_Gau_Dialog::save() {
    QSqlQuery query6;
    if (vid == 0) {
        query6.prepare("INSERT INTO tfx_gaue (int_verbaendeid,var_name,var_kuerzel) VALUES (?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_gaue SET int_verbaendeid=?,var_name=?,var_kuerzel=? WHERE int_gaueid=?");
        query6.bindValue(3,vid);
    }
    query6.bindValue(0,cmb_verband->itemData(cmb_verband->currentIndex()));
    query6.bindValue(1,txt_name->text());
    query6.bindValue(2,txt_kuerzel->text());
    query6.exec();
    done(1);
}
