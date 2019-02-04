#include <QSqlQuery>
#include <QColorDialog>
#include "header/dlg_db_status.h"
#include "../../global/header/_global.h"

Db_St_Dialog::Db_St_Dialog(int tid, QWidget* parent) : QDialog(parent) {
    tnid = tid;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    QObject::connect(but_color, SIGNAL(clicked()), this, SLOT(colorChooser()));
    if (tid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, ary_colorcode bol_bogen, bol_karte FROM tfx_status WHERE int_statusid=?");
        query.bindValue(0,tid);
        query.exec();
        query.next();
        txt_name->setText(query.value(0).toString());
        QList<int> color = _global::splitColorArray(query.value(1).toString());
        lbl_color->setStyleSheet("background-color: rgb(" + QString().setNum(color.at(0)) + ", " + QString().setNum(color.at(1)) + ", " + QString().setNum(color.at(2)) + ");");
        chk_karte->setChecked(query.value(2).toBool());
        chk_bogen->setChecked(query.value(3).toBool());
    }
}

void Db_St_Dialog::colorChooser() {
    QColorDialog colc(col,this);
    colc.exec();
    col = colc.selectedColor();
    lbl_color->setStyleSheet("background-color: rgb(" + QString().setNum(col.red()) + ", " + QString().setNum(col.green()) + ", " + QString().setNum(col.blue()) + ");");
}

void Db_St_Dialog::save() {
    QSqlQuery query6;
    if (tnid == 0) {
        query6.prepare("INSERT INTO tfx_status (var_name,ary_colorcode,bol_bogen,bol_karte) VALUES (?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_status SET var_name=?, ary_colorcode=?, bol_bogen=?, bol_karte=? WHERE int_statusid=?");
        query6.bindValue(4,tnid);
    }
    query6.bindValue(0,txt_name->text());
    query6.bindValue(1,"{" + QString().setNum(col.red()) + "," + QString().setNum(col.green()) + "," + QString().setNum(col.blue()) + "}");
    query6.bindValue(2,chk_karte->isChecked());
    query6.bindValue(3,chk_bogen->isChecked());
    query6.exec();
    done(1);
}
