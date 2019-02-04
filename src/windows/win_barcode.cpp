#include <QSqlQuery>
#include "header/win_barcode.h"
#include "../dialogs/participants/header/dlg_tn.h"
#include "../dialogs/participants/header/dlg_group.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QShortcut>
#include <QInputDialog>

Barcode_Tool::Barcode_Tool() {
    setupUi(this);
    showMaximized();
    QSqlQuery query;
    query.prepare("SELECT int_runde, int_hauptwettkampf FROM tfx_veranstaltungen WHERE int_veranstaltungenid=? LIMIT 1");
    query.bindValue(0,_global::getWkNr());
    query.exec();
    query.next();
    id = 0;
    _global::setWkVars(_global::getWkNr(), query.value(0).toInt(), query.value(1).toInt());
    connect(but_edit,SIGNAL(clicked()),this,SLOT(edit()));
    connect(but_sn,SIGNAL(clicked()),this,SLOT(nostart()));
    connect(but_check,SIGNAL(clicked()),this,SLOT(changeStatus()));
}

void Barcode_Tool::edit() {
    bool ok;
    int stnr = QInputDialog::getInteger(0, tr("Teilnehmer editieren"), tr("Scannen Sie die Startnummer oder geben sie sie ein!"),1,0,2147483647,1,&ok);
    if (ok) {
        QSqlQuery query;
        query.prepare("SELECT int_wertungenid, int_typ, int_gruppenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? AND int_startnummer=?");
        query.bindValue(0, _global::getWkNr());
        query.bindValue(1, _global::getRunde());
        query.bindValue(2, stnr);
        query.exec();
        if (query.size()>0) {
            query.next();
            if (query.value(1).toInt() == 2) {
                Group_Dialog *gr = new Group_Dialog(query.value(2).toInt(),this);
                gr->exec();
            } else {
                Tn_Dialog *tn = new Tn_Dialog(query.value(0).toInt(),this);
                tn->exec();
            }
        }
        edit();
    }
}

void Barcode_Tool::nostart() {
    bool ok;
    int stnr = QInputDialog::getInteger(0, tr("Teilnehmer startet nicht"), tr("Scannen Sie die Startnummer oder geben sie sie ein!"),1,0,2147483647,1,&ok);
    if (ok) {
        QSqlQuery query;
        query.prepare("UPDATE tfx_wertungen SET bol_startet_nicht=true WHERE int_wertungenid=(SELECT int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? AND int_startnummer=?)");
        query.bindValue(0, _global::getWkNr());
        query.bindValue(1, _global::getRunde());
        query.bindValue(2, stnr);
        query.exec();
        id++;
        if (query.numRowsAffected()>0) {
            txt_change->setText("Änderung: " + QString::number(id)+" wurde gespeichert.");
        } else {
            txt_change->setText("Änderung: " + QString::number(id)+" FEHLER Änderung wurde nicht gespeichert.");
        }
        nostart();
    }
}

void Barcode_Tool::changeStatus() {
    bool ok;
    int stnr = QInputDialog::getInteger(0, tr("Wettkampfkarte ist zurÃ¼ck"), tr("Scannen Sie die Startnummer oder geben sie sie ein!"),1,0,2147483647,1,&ok);
    if (ok) {
        QSqlQuery query;
        query.prepare("UPDATE tfx_wertungen SET int_statusid=? WHERE int_wertungenid=(SELECT int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? AND int_startnummer=?)");
        query.bindValue(0, 8);
        query.bindValue(1, _global::getWkNr());
        query.bindValue(2, _global::getRunde());
        query.bindValue(3, stnr);
        query.exec();
        id++;
        if (query.numRowsAffected()>0) {
            txt_change->setText("Änderung: " + QString::number(id)+" wurde gespeichert.");
        } else {
            txt_change->setText("Änderung: " + QString::number(id)+" FEHLER Änderung wurde nicht gespeichert.");
        }
        changeStatus();
    }
}
