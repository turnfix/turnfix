#include "resultswidget.h"
#include "model/entity/competition.h"
#include "model/settings/session.h"
#include "resultstablemodel.h"
#include "src/global/header/_delegates.h"
#include "src/global/header/_global.h"
#include "src/global/header/result_calc.h"
#include "ui_resultswidget.h"
#include <QSqlQuery>

ResultsWidget::ResultsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ResultsWidget)
{
    ui->setupUi(this);

    this->event = Session::getInstance()->getEvent();
    this->er_model = new ResultsTableModel();

    ui->er_table->setModel(er_model);
    connect(ui->cmb_selectwk, SIGNAL(currentIndexChanged(int)), this, SLOT(fillERTable()));
}

ResultsWidget::~ResultsWidget()
{
    delete ui;
}

void ResultsWidget::fillERTable()
{
    if (ui->cmb_selectwk->count() > 0) {
        Competition *competition = Competition::getByNumber(
            this->event, ui->cmb_selectwk->itemData(ui->cmb_selectwk->currentIndex()).toString());
        QList<QStringList> list = Result_Calc::resultArrayNew(competition);
        int wktyp = competition->getType();
        int hwk = competition->getEvent()->mainEventId();
        QString nr = ui->cmb_selectwk->itemData(ui->cmb_selectwk->currentIndex()).toString();
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
                QHeaderView *hv = ui->er_table->horizontalHeader();
                hv->setSectionResizeMode(i, resizeModeER.at(i));
                if (i > 2) {
                    ui->er_table->setItemDelegateForColumn(i, new AlignItemDelegate);
                }
            }
        }
    }
}

void ResultsWidget::updateERList()
{
    ui->cmb_selectwk->clear();
    QSqlQuery query2;
    query2.prepare("SELECT var_nummer, var_name FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY var_nummer ASC");
    query2.bindValue(0, this->event->mainEventId());
    query2.exec();
    while (query2.next()) {
        ui->cmb_selectwk->addItem("WkNr. " + query2.value(0).toString() + " "
                                      + query2.value(1).toString(),
                                  query2.value(0).toString());
    }
}
