#include "selectcompetitiondialog.h"
#include "model/entity/event.h"
#include "src/global/header/_global.h"
#include "ui_selectcompetitiondialog.h"
#include <QSqlQuery>

SelectCompetitionDialog::SelectCompetitionDialog(Event *event, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectCompetitionDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    this->event = event;

    connect(ui->but_select, SIGNAL(clicked()), this, SLOT(select1()));
    initData();
}

SelectCompetitionDialog::~SelectCompetitionDialog()
{
    delete ui;
}

void SelectCompetitionDialog::initData()
{
    QSqlQuery query2;
    query2.prepare("SELECT var_nummer || ' ' || var_name, var_nummer FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY var_nummer");
    query2.bindValue(0, this->event->mainEventId());
    query2.exec();
    while (query2.next()) {
        ui->cmb_dis->addItem(query2.value(0).toString(), query2.value(1).toString());
    }
}

void SelectCompetitionDialog::select1()
{
    wk = QVariant(ui->cmb_dis->itemData(ui->cmb_dis->currentIndex())).toString();
    done(1);
}

QString SelectCompetitionDialog::getWk()
{
    return wk;
}
