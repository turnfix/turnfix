#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "header/wdg_tab_wk.h"
#include "../dialogs/wk/header/dlg_wk.h"
#include "../global/header/_global.h"

Tab_WK::Tab_WK(QWidget* parent) : QWidget(parent) {
    setupUi(this);
    wk_model = new QSqlQueryModel();
    wk_sort_model = new QSortFilterProxyModel();
    //wk_sort_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    wk_sort_model->setSourceModel(wk_model);
    wk_table->setModel(wk_sort_model);
    connect(but_addWK, SIGNAL(clicked()), this, SLOT(addWK()));
    connect(but_editWK, SIGNAL(clicked()), this, SLOT(editWK()));
    connect(but_delWK, SIGNAL(clicked()), this, SLOT(delWK()));
    connect(cmb_filterWK, SIGNAL(currentIndexChanged(int)), this, SLOT(updateWKFilterColumn(int)));
    connect(txt_filterWK, SIGNAL(textChanged(QString)), this, SLOT(updateWKFilterText(QString)));
    connect(wk_table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editWK()));
}

void Tab_WK::fillWKTable() {
    int idx = cmb_filterWK->currentIndex();
    QSqlQuery query;
    query.prepare("SELECT var_nummer, tfx_wettkaempfe.var_name, tfx_bereiche.var_name, CASE WHEN yer_bis = 3 THEN 'offen' ELSE CAST(yer_von AS text) END, CASE WHEN yer_bis=1 THEN 'und �lter' WHEN yer_bis=2 THEN 'und j�nger' WHEN yer_bis=3 THEN 'offen' ELSE CAST(yer_bis AS text) END , int_qualifikation, (SELECT COUNT(*) FROM tfx_wertungen WHERE int_wettkaempfeid=tfx_wettkaempfe.int_wettkaempfeid AND int_runde=?) AS starter, int_wettkaempfeid FROM tfx_wettkaempfe INNER JOIN tfx_bereiche USING (int_bereicheid) WHERE int_veranstaltungenid=? ORDER BY var_nummer ASC");
    query.bindValue( 0, _global::getRunde() );
    query.bindValue( 1,_global::checkHWK() );
    query.exec();
    wk_model->setQuery(query);
    cmb_filterWK->clear();
    QHeaderView::ResizeMode resizeModeWK[] = {QHeaderView::ResizeToContents,
                                              QHeaderView::Stretch,
                                              QHeaderView::ResizeToContents,
                                              QHeaderView::ResizeToContents,
                                              QHeaderView::ResizeToContents,
                                              QHeaderView::ResizeToContents,
                                              QHeaderView::ResizeToContents};
    QString headersWK[7] = {"#",
                            "Bezeichnung",
                            "Bereich",
                            "von Jahr",
                            "bis Jahr",
                            "Q",
                            "Starter"};
    for (int i=0;i<7;i++) {
        cmb_filterWK->addItem(headersWK[i]);
        wk_model->setHeaderData(i, Qt::Horizontal, headersWK[i]);
        wk_table->horizontalHeader()->setResizeMode(i, resizeModeWK[i]);
    }
    wk_table->hideColumn(7);
    cmb_filterWK->setCurrentIndex(idx);
    if (cmb_filterWK->currentIndex()==-1) cmb_filterWK->setCurrentIndex(0);
}

void Tab_WK::addWK() {
    Wk_Dialog *sw = new Wk_Dialog(0,this);
    if(sw->exec() == 1) fillWKTable();
    _global::updateRgDis();
    wk_table->setFocus();
}

void Tab_WK::editWK() {
    if (wk_table->currentIndex().isValid()) {
        QModelIndex idx = wk_table->currentIndex();
        Wk_Dialog *sw = new Wk_Dialog(QVariant(wk_sort_model->data(wk_sort_model->index(wk_table->currentIndex().row(),7))).toInt(),this);
        if(sw->exec() == 1) fillWKTable();
        _global::updateRgDis();
        wk_table->setCurrentIndex(idx);
        wk_table->setFocus();
    }
}

void Tab_WK::delWK() {
    if (wk_table->currentIndex().isValid()) {
        QMessageBox msg(QMessageBox::Question, "Wettkampf l�schen", "Wollen sie diesen Wettkampf wirklich l�schen?",QMessageBox::Ok | QMessageBox::Cancel);
        if(msg.exec() == QMessageBox::Ok) {
            QSqlQuery query;
            query.prepare("DELETE FROM tfx_wettkaempfe WHERE int_wettkaempfeid=?");
            query.bindValue( 0, QVariant(wk_sort_model->data(wk_sort_model->index(wk_table->currentIndex().row(),7))).toInt() );
            query.exec();
            fillWKTable();
        }
        _global::updateRgDis();
        wk_table->setFocus();
    }
}

void Tab_WK::updateWKFilterColumn(int index) {
    wk_sort_model->setFilterKeyColumn(index);
}

void Tab_WK::updateWKFilterText(QString text) {
    wk_sort_model->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive, QRegExp::Wildcard));
}
