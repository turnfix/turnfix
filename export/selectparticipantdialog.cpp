#include "selectparticipantdialog.h"
#include "model/entity/competition.h"
#include "model/entitymanager.h"
#include "model/repository/competitionrepository.h"
#include "results/resultstablemodel.h"
#include "src/global/header/_delegates.h"
#include "src/global/header/_global.h"
#include "src/global/header/result_calc.h"
#include "ui_selectparticipantdialog.h"
#include <QList>
#include <QSqlQuery>

SelectParticipantDialog::SelectParticipantDialog(Event *event, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectParticipantDialog)
{
    ui->setupUi(this);

    this->m_event = event;
    this->er_model = new ResultsTableModel();

    ui->tbl_tn->setModel(er_model);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    connect(ui->cmb_wk, SIGNAL(currentIndexChanged(int)), this, SLOT(updateList()));
    connect(ui->but_select, SIGNAL(clicked()), this, SLOT(submit()));
    initData();
}

SelectParticipantDialog::~SelectParticipantDialog()
{
    delete ui;
}

QList<int> SelectParticipantDialog::getTnList()
{
    return tnlist;
}

QString SelectParticipantDialog::getTnWk()
{
    return tnwk;
}

void SelectParticipantDialog::initData()
{
    QSqlQuery query2;
    query2.prepare("SELECT var_nummer, var_name FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY var_nummer ASC");
    query2.bindValue(0, this->m_event->mainEvent()->id());
    query2.exec();
    while (query2.next()) {
        ui->cmb_wk->addItem(query2.value(0).toString() + " " + query2.value(1).toString(),
                            query2.value(0).toString());
    }
    updateList();
}

void SelectParticipantDialog::updateList()
{
    if (ui->cmb_wk->count() > 0) {
        Competition *competition = m_em->competitionRepository()->fetchByNumber(
            this->m_event, ui->cmb_wk->itemData(ui->cmb_wk->currentIndex()).toString());
        QList<QStringList> list = Result_Calc::resultArrayNew(competition);
        int wktyp = competition->type();
        int hwk = this->m_event->mainEvent()->id();
        QString nr = ui->cmb_wk->itemData(ui->cmb_wk->currentIndex()).toString();
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
                ui->tbl_tn->horizontalHeader()->setSectionResizeMode(i, resizeModeER[i]);
                if (i > 2) {
                    ui->tbl_tn->setItemDelegateForColumn(i, new AlignItemDelegate);
                }
            }
            ui->tbl_tn->hideColumn(3);
        }
    }
}

void SelectParticipantDialog::submit()
{
    QModelIndexList indexes = ui->tbl_tn->selectionModel()->selectedRows();
    for (int i=0;i<indexes.size();i++) {
        tnlist << er_model->data(er_model->index(indexes.at(i).row(),3),Qt::DisplayRole).toInt();
    }
    tnwk = ui->cmb_wk->itemData(ui->cmb_wk->currentIndex()).toString();
    done(1);
}
