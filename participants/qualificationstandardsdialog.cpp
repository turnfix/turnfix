#include "qualificationstandardsdialog.h"
#include "model/entity/event.h"
#include "qualitablemodel.h"
#include "src/global/header/_delegates.h"
#include "src/global/header/_global.h"
#include "ui_qualificationstandardsdialog.h"
#include <QLineEdit>
#include <QSqlQuery>

QualificationStandardsDialog::QualificationStandardsDialog(Event *event, int edit, QWidget* parent) : QDialog(parent), ui(new Ui::QualificationStandardsDialog) {
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    this->editid = edit;
    this->m_event = event;
    this->model = new QualificationStandardsTableModel();

    EditorDelegate *ed = new EditorDelegate;

    ui->tbl_quali->setModel(model);
    ui->tbl_quali->setItemDelegateForColumn(1,ed);
    ui->tbl_quali->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tbl_quali->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    connect(ed, SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)), this, SLOT(finishEdit()));
    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(close()));

    initData();
}

QualificationStandardsDialog::~QualificationStandardsDialog()
{
    delete ui;
}

void QualificationStandardsDialog::initData() {
    QSqlQuery query2;
    query2.prepare("SELECT int_disziplinenid FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_veranstaltungenid=? AND int_wertungenid=? AND bol_bahnen AND (int_disziplinenid IN (SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_disziplinenid=tfx_disziplinen.int_disziplinenid AND int_wertungenid=tfx_wertungen.int_wertungenid) OR (SELECT COUNT(*) FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=tfx_wertungen.int_wertungenid)=0) ORDER BY int_disziplinenid");
    query2.bindValue(0, this->m_event->id());
    query2.bindValue(1, this->editid);
    query2.exec();
    QList<int> disids;
    while (query2.next()) {
        disids.append(query2.value(0).toInt());
    }
    model->setTableData(editid,disids);
    QSqlQuery query;
    query.prepare("SELECT CASE WHEN tfx_gruppen.int_gruppenid IS NULL THEN var_nachname || ', ' || var_vorname ELSE tfx_gruppen.var_name END, tfx_vereine.var_name, yer_bis, yer_von, var_nummer, tfx_wettkaempfe.var_name FROM tfx_wertungen LEFT JOIN tfx_teilnehmer USING (int_teilnehmerid) LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid =  tfx_gruppen.int_vereineid INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE int_wertungenid=? AND int_veranstaltungenid=?");
    query.bindValue(0, this->editid);
    query.bindValue(1, this->m_event->id());
    query.exec();
    query.next();
    ui->lbl_name->setText(query.value(0).toString());
    ui->lbl_club->setText(query.value(1).toString());
    QString to;
    switch (query.value(3).toInt()) {
           case 1  : to =  QString(" und älter"); break;
           case 2  : to =  QString(" und jünger"); break;
           default : to =  QString(" - " + query.value(3).toString()); break;
       }
    if ( query.value(2).toString() == query.value(3).toString() ) {
        to = QString("");
    }
    ui->lbl_wk->setText("Nr. " + query.value(4).toString() + " - " +query.value(5).toString() + " - Jahrgang " + query.value(2).toString() + to);
}

void QualificationStandardsDialog::finishEdit() {
    int row = ui->tbl_quali->currentIndex().row();
    int col = ui->tbl_quali->currentIndex().column();
    if (model->index(row+1,col).isValid()) {
        ui->tbl_quali->setCurrentIndex(model->index(row+1,col));
    } else {
        ui->tbl_quali->setCurrentIndex(model->index(0,col));
    }
}
