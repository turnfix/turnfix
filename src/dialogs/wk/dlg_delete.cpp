#include <QMessageBox>
#include <QSqlQuery>
#include "header/dlg_delete.h"
#include "ui_dlg_delete.h"

Delete_Dialog::Delete_Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Delete_Dialog) {
    ui->setupUi(this);
    QSqlQuery query("SELECT int_veranstaltungenid, tfx_veranstaltungen.var_name, dat_von, tfx_wettkampforte.var_name, tfx_wettkampforte.var_ort FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) ORDER BY dat_von DESC");
    while (query.next()) {
        ui->cmb_events->addItem(query.value(1).toString()+", "+query.value(3).toString()+", "+query.value(4).toString(),query.value(0).toInt());
    }

    connect(ui->bbx_done,SIGNAL(accepted()),this,SLOT(closeAndDelete()));
    connect(ui->bbx_done,SIGNAL(rejected()),this,SLOT(close()));
}

Delete_Dialog::~Delete_Dialog() {
    delete ui;
}

void Delete_Dialog::closeAndDelete() {
    QMessageBox msg(QMessageBox::Question, "Wirklich löschen?", "Soll diese Veranstaltung wirklich gelöscht werden? Alle Daten (auch eventuelle zugehörige Runden) werden unwiederruflich gelöscht.",QMessageBox::Yes | QMessageBox::No);
    int ret = msg.exec();
    if (ret == QMessageBox::Yes) {
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM tfx_veranstaltungen WHERE int_hauptwettkampf=? AND bol_rundenwettkampf='true'");
        deleteQuery.bindValue(0,ui->cmb_events->itemData(ui->cmb_events->currentIndex()));
        deleteQuery.exec();
        deleteQuery.prepare("DELETE FROM tfx_veranstaltungen WHERE int_veranstaltungenid=?");
        deleteQuery.bindValue(0,ui->cmb_events->itemData(ui->cmb_events->currentIndex()));
        deleteQuery.exec();
        done(1);
    }
}
