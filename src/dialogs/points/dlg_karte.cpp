#include <QSqlQuery>
#include "header/dlg_karte.h"
#include "../../global/header/_delegates.h"
#include "../../global/header/_global.h"
#include "../../models/header/mdl_karte.h"

WK_Karte::WK_Karte(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);
    pe_model = new QKartenTableModel();
    pe_table->setModel(pe_model);
    connect(but_save, SIGNAL(clicked()), this, SLOT(saveClose()));
}

void WK_Karte::init(int s, int we, QList< QList<int> > d) {
    stnr = s;
    disids = d;
    wertid = we;
    QSqlQuery query;
    query.prepare("SELECT 'StNr.: ' || tfx_wertungen.int_startnummer || '\n' ||var_vorname || ' ' || var_nachname || ' - ' || tfx_vereine.var_name FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_mannschaften ON tfx_mannschaften.int_mannschaftenid = tfx_wertungen.int_mannschaftenid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_mannschaften.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid OR tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid WHERE int_veranstaltungenid=? AND tfx_wertungen.int_startnummer=? AND int_runde=?");
    query.bindValue(0, _global::checkHWK());
    query.bindValue(1, stnr);
    query.bindValue(2, _global::getRunde());
    query.exec();
    query.next();
    lbl_tn->setText(query.value(0).toString());
    QSqlQuery query3;
    query3.prepare("SELECT int_statusid, var_name, ary_colorcode FROM tfx_status WHERE bol_karte='true' ORDER BY int_statusid ASC");
    query3.exec();
    while(query3.next()) {
        cmb_status->addItem(query3.value(1).toString(),query3.value(0).toInt());
        QList<int> color = _global::splitColorArray(query3.value(2).toString());
        cmb_status->setItemData(cmb_status->count()-1,QColor(color.at(0),color.at(1),color.at(2)),Qt::BackgroundColorRole);
    }
    QSqlQuery query2;
    query2.prepare("SELECT int_statusid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_startnummer=? AND int_runde=?");
    query2.bindValue(0, _global::checkHWK());
    query2.bindValue(1, stnr);
    query2.bindValue(2, _global::getRunde());
    query2.exec();
    query2.next();
    cmb_status->setCurrentIndex(cmb_status->findData(query2.value(0).toInt()));
    connect(cmb_status, SIGNAL(currentIndexChanged(int)), this, SLOT(statusChange1()));
    fillPETable();
}

void WK_Karte::fillPETable() {
    pe_table->clearSelection();
    pe_model->setTableData(wertid,disids);
    pe_table->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    pe_table->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    editorDelegate *ed = new editorDelegate;
    connect(ed, SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)), this, SLOT(finishEdit()));
    pe_table->setItemDelegateForColumn(1,ed);
    pe_table->setCurrentIndex(pe_model->index(0,1));
    pe_table->edit(pe_table->currentIndex());
}

void WK_Karte::finishEdit() {
    int row = pe_table->currentIndex().row();
    int col = pe_table->currentIndex().column();
    if (pe_model->index(row+1,col).isValid()) {
        pe_table->setCurrentIndex(pe_model->index(row+1,col));
    } else {
        pe_table->setCurrentIndex(pe_model->index(0,col));
    }
}

void WK_Karte::statusChange1() {
    QSqlQuery query;
    query.prepare("UPDATE tfx_wertungen SET int_statusid=? WHERE int_wettkaempfeid IN (SELECT int_wettkaempfeid FROM tfx_wettkaempfe WHERE int_veranstaltungenid=?) AND int_startnummer=? AND int_runde=?");
    query.bindValue(0, cmb_status->itemData(cmb_status->currentIndex()).toInt());
    query.bindValue(1, _global::checkHWK());
    query.bindValue(2, stnr);
    query.bindValue(3, _global::getRunde());
    query.exec();
}

void WK_Karte::saveClose() {
    finishEdit();
    close();
}
