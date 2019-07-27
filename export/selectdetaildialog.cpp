#include "selectdetaildialog.h"
#include "model/entity/event.h"
#include "src/global/header/_global.h"
#include "ui_selectdetaildialog.h"
#include <QSqlQuery>

SelectDetailDialog::SelectDetailDialog(Event *event, int type, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectDetailDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    QSqlQuery query;

    switch (type) {
    case 1: query.prepare("SELECT tfx_gaue.int_gaueid, tfx_gaue.var_name FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_wertungen.int_teilnehmerid = tfx_teilnehmer.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_wertungen.int_gruppenid = tfx_gruppen.int_gruppenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid INNER JOIN tfx_gaue USING (int_gaueid) WHERE int_veranstaltungenid=? GROUP BY tfx_gaue.var_name, tfx_gaue.int_gaueid ORDER BY tfx_gaue.var_name"); break;
    case 2: query.prepare("SELECT tfx_verbaende.int_verbaendeid, tfx_verbaende.var_name FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_wertungen.int_teilnehmerid = tfx_teilnehmer.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_wertungen.int_gruppenid = tfx_gruppen.int_gruppenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid INNER JOIN tfx_gaue USING (int_gaueid) INNER JOIN tfx_verbaende USING (int_verbaendeid) WHERE int_veranstaltungenid=? GROUP BY tfx_verbaende.var_name, tfx_verbaende.int_verbaendeid ORDER BY tfx_verbaende.var_name"); break;
    case 3: query.prepare("SELECT tfx_laender.int_laenderid, tfx_laender.var_name FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_wertungen.int_teilnehmerid = tfx_teilnehmer.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_wertungen.int_gruppenid = tfx_gruppen.int_gruppenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid INNER JOIN tfx_gaue USING (int_gaueid) INNER JOIN tfx_verbaende USING (int_verbaendeid) INNER JOIN tfx_laender USING (int_laenderid) WHERE int_veranstaltungenid=? GROUP BY tfx_laender.var_name, tfx_laender.int_laenderid ORDER BY tfx_laender.var_name"); break;
    }
    query.bindValue(0, event->mainEvent()->id());
    query.exec();
    while (query.next()) {
        ui->cmb_list->addItem(query.value(1).toString(),query.value(0).toInt());
    }

    connect(ui->bbx_done,SIGNAL(accepted()),this,SLOT(closeDialog()));
    connect(ui->bbx_done,SIGNAL(rejected()),this,SLOT(close()));
}

SelectDetailDialog::~SelectDetailDialog()
{
    delete ui;
}

void SelectDetailDialog::closeDialog()
{
    done(1);
}

int SelectDetailDialog::getId()
{
    return ui->cmb_list->itemData(ui->cmb_list->currentIndex()).toInt();
}
