#include "selectsubdivisiondialog.h"
#include "model/objects/event.h"
#include "src/global/header/_global.h"
#include "ui_selectsubdivisiondialog.h"
#include <QSqlQuery>

SelectSubdivisionDialog::SelectSubdivisionDialog(Event *event, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectSubdivisionDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    this->event = event;

    connect(ui->but_select, SIGNAL(clicked()), this, SLOT(select1()));
    initData();
}

SelectSubdivisionDialog::~SelectSubdivisionDialog()
{
    delete ui;
}

void SelectSubdivisionDialog::initData()
{
    QSqlQuery query2;
    query2.prepare("SELECT DISTINCT(var_riege) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? ORDER BY var_riege");
    query2.bindValue(0, this->event->getMainEventId());
    query2.bindValue(1, this->event->getRound());
    query2.exec();
    while (query2.next()) {
        QListWidgetItem *itm = new QListWidgetItem(query2.value(0).toString());
        ui->lst_rg->addItem(itm);
    }
}

void SelectSubdivisionDialog::select1()
{
    rg.clear();
    QList<QListWidgetItem *> selected = ui->lst_rg->selectedItems();
    for (int i=0;i<selected.size();i++) {
        rg.append(selected.at(i)->text());
    }
    done(1);
}

QStringList SelectSubdivisionDialog::getRg()
{
    return rg;
}
