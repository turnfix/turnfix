#include <QSqlQuery>
#include <QDebug>
#include "model/objects/competition.h"
#include "model/settings/session.h"
#include "header/wdg_tab_er.h"
#include "../global/header/_delegates.h"
#include "../global/header/_global.h"
#include "../global/header/result_calc.h"
#include "../models/header/mdl_result.h"

Tab_ER::Tab_ER(QWidget* parent) : QWidget(parent) {
    setupUi(this);

    this->event = Session::getInstance()->getEvent();
    this->er_model = new QErgebnisTableModel();

    er_table->setModel(er_model);
    connect(cmb_selectwk, SIGNAL(currentIndexChanged(int)), this, SLOT(fillERTable()));
}

void Tab_ER::fillERTable() {
    if (cmb_selectwk->count() > 0) {
        Competition *competition = Competition::getByNumber(this->event, cmb_selectwk->itemData(cmb_selectwk->currentIndex()).toString());
        QList<QStringList> list = Result_Calc::resultArrayNew(competition);
        int wktyp = competition->getType();
        int hwk = competition->getEvent()->getMainEventId();
        QString nr = cmb_selectwk->itemData(cmb_selectwk->currentIndex()).toString();
        er_model->setList(list,nr,hwk,wktyp);
        if (list.size() > 0) {

            int size = list.at(0).size()-1;
            QList<QHeaderView::ResizeMode> resizeModeER;

            resizeModeER << QHeaderView::ResizeToContents;
            resizeModeER << QHeaderView::Stretch;
            resizeModeER << QHeaderView::Stretch;
            resizeModeER << QHeaderView::ResizeToContents;

            for (int i=4;i<size;i++) {
                resizeModeER << QHeaderView::ResizeToContents;
            }

            for (int i=0;i<size;i++) {
                QHeaderView *hv = er_table->horizontalHeader();
                hv->setSectionResizeMode(i, resizeModeER.at(i));
                if (i > 2) {
                    er_table->setItemDelegateForColumn(i,new AlignItemDelegate);
                }
            }
        }
    }
}

void Tab_ER::updateERList() {
    cmb_selectwk->clear();
    QSqlQuery query2;
    query2.prepare("SELECT var_nummer, var_name FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY var_nummer ASC");
    query2.bindValue(0, this->event->getMainEventId());
    query2.exec();
    while (query2.next()) {
        cmb_selectwk->addItem("WkNr. " + query2.value(0).toString() + " " + query2.value(1).toString(),query2.value(0).toString());
    }
}
