#include <QSqlQuery>
#include <QList>
#include "header/dlg_select_tn.h"
#include "../../global/header/_global.h"
#include "../../models/header/mdl_result.h"
#include "../../global/header/_delegates.h"
#include "../../global/header/result_calc.h"

Select_Tn_Dialog::Select_Tn_Dialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    er_model = new QErgebnisTableModel();
    tbl_tn->setModel(er_model);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QObject::connect(cmb_wk, SIGNAL(currentIndexChanged(int)), this, SLOT(updateList()));
    QObject::connect(but_select, SIGNAL(clicked()), this, SLOT(submit()));
    initData();
}

QList<int> Select_Tn_Dialog::getTnList() {
    return tnlist;
}

QString Select_Tn_Dialog::getTnWk() {
    return tnwk;
}

void Select_Tn_Dialog::initData() {
    QSqlQuery query2;
    query2.prepare("SELECT var_nummer, var_name FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY var_nummer ASC");
    query2.bindValue(0,_global::checkHWK());
    query2.exec();
    while (query2.next()) {
        cmb_wk->addItem(query2.value(0).toString() + " " + query2.value(1).toString(),query2.value(0).toString());
    }
    updateList();
}

void Select_Tn_Dialog::updateList() {
    if (cmb_wk->count() > 0) {
       QList<QStringList> list = Result_Calc::resultArrayNew(cmb_wk->itemData(cmb_wk->currentIndex()).toString());
        int wktyp = _global::checkTyp(cmb_wk->itemData(cmb_wk->currentIndex()).toString());
        int hwk = _global::checkHWK();
        QString nr = cmb_wk->itemData(cmb_wk->currentIndex()).toString();
        er_model->setList(list,nr,hwk,wktyp,false);
        if (list.size() > 0) {
            int size = list.at(0).size();
            QList<QHeaderView::ResizeMode> resizeModeER;

            resizeModeER[0] = QHeaderView::ResizeToContents;
            resizeModeER[1] = QHeaderView::Stretch;
            resizeModeER[2] = QHeaderView::Stretch;
            resizeModeER[3] = QHeaderView::ResizeToContents;

            QStringList headersER;
            headersER[0] = "Platz";

            int i;
            if (wktyp == 0 || wktyp == 2) {
                i = 4;
            } else {
                i = 3;
            }
            for (int j=i;j<(size-1);j++) {
                resizeModeER[i] = QHeaderView::ResizeToContents;
                i++;
            }
            resizeModeER[i] = QHeaderView::ResizeToContents;
            for (int i=0;i<size;i++) {
                tbl_tn->horizontalHeader()->setSectionResizeMode(i, resizeModeER[i]);
                if (i > 2) {
                    tbl_tn->setItemDelegateForColumn(i,new alignItemDelegate);
                }
            }
            tbl_tn->hideColumn(3);
        }
    }
}

void Select_Tn_Dialog::submit() {
    QModelIndexList indexes = tbl_tn->selectionModel()->selectedRows();
    for (int i=0;i<indexes.size();i++) {
        tnlist << er_model->data(er_model->index(indexes.at(i).row(),3),Qt::DisplayRole).toInt();
    }
    tnwk = cmb_wk->itemData(cmb_wk->currentIndex()).toString();
    done(1);
}
