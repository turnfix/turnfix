#include <QSqlQuery>
#include <QLineEdit>
#include "header/dlg_quali.h"
#include "../../global/header/_delegates.h"
#include "../../global/header/_global.h"
#include "../../models/header/mdl_quali.h"

Quali_Dialog::Quali_Dialog(int edit, QWidget* parent) : QDialog(parent) {
    editid=edit;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    model = new QQualiTableModel();
    tbl_quali->setModel(model);
    editorDelegate *ed = new editorDelegate;
    connect(ed, SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)), this, SLOT(finishEdit()));
    tbl_quali->setItemDelegateForColumn(1,ed);
    tbl_quali->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    tbl_quali->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    QObject::connect(but_save, SIGNAL(clicked()), this, SLOT(close()));
    initData();
}

void Quali_Dialog::initData() {
    QSqlQuery query2;
    query2.prepare("SELECT int_disziplinenid FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_veranstaltungenid=? AND int_wertungenid=? AND bol_bahnen AND (int_disziplinenid IN (SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_disziplinenid=tfx_disziplinen.int_disziplinenid AND int_wertungenid=tfx_wertungen.int_wertungenid) OR (SELECT COUNT(*) FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=tfx_wertungen.int_wertungenid)=0) ORDER BY int_disziplinenid");
    query2.bindValue(0,_global::getWkNr());
    query2.bindValue(1,editid);
    query2.exec();
    QList<int> disids;
    while (query2.next()) {
        disids.append(query2.value(0).toInt());
    }
    model->setTableData(editid,disids);
    QSqlQuery query;
    query.prepare("SELECT CASE WHEN tfx_gruppen.int_gruppenid IS NULL THEN var_nachname || ', ' || var_vorname ELSE tfx_gruppen.var_name END, tfx_vereine.var_name, yer_bis, yer_von, var_nummer, tfx_wettkaempfe.var_name FROM tfx_wertungen LEFT JOIN tfx_teilnehmer USING (int_teilnehmerid) LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid =  tfx_gruppen.int_vereineid INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE int_wertungenid=? AND int_veranstaltungenid=?");
    query.bindValue(0,editid);
    query.bindValue(1,_global::getWkNr());
    query.exec();
    query.next();
    lbl_name->setText(query.value(0).toString());
    lbl_club->setText(query.value(1).toString());
    QString to;
    switch (query.value(3).toInt()) {
           case 1  : to =  QString(" und älter"); break;
           case 2  : to =  QString(" und jünger"); break;
           default : to =  QString(" - " + query.value(3).toString()); break;
       }
    if ( query.value(2).toString() == query.value(3).toString() ) {
        to = QString("");
    }
    lbl_wk->setText("Nr. " + query.value(4).toString() + " - " +query.value(5).toString() + " - Jahrgang " + query.value(2).toString() + to);
}

void Quali_Dialog::finishEdit() {
    int row = tbl_quali->currentIndex().row();
    int col = tbl_quali->currentIndex().column();
    if (model->index(row+1,col).isValid()) {
        tbl_quali->setCurrentIndex(model->index(row+1,col));
    } else {
        tbl_quali->setCurrentIndex(model->index(0,col));
    }
}
