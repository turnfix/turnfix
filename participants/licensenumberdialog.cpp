#include "licensenumberdialog.h"
#include "model/entity/event.h"
#include "src/global/header/_global.h"
#include "ui_licensenumberdialog.h"
#include <QKeyEvent>
#include <QSqlQuery>
#include <QSqlQueryModel>

LicenseNumberDialog::LicenseNumberDialog(Event *event, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LicenseNumberDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    this->m_event = event;
    this->model = new QSqlQueryModel();

    ui->tbl_pass->setModel(model);

    connect(ui->but_close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->cmb_club, SIGNAL(currentIndexChanged(int)), this, SLOT(fillTable()));
    connect(ui->tbl_pass->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this,
            SLOT(selectLine()));

    ui->txt_number->installEventFilter(this);
    QSqlQuery query;
    query.prepare("SELECT int_vereineid, tfx_vereine.var_name FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid INNER JOIN tfx_vereine USING (int_vereineid) WHERE int_veranstaltungenid=? GROUP BY int_vereineid, tfx_vereine.var_name, int_start_ort ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name");
    query.bindValue(0, this->m_event->mainEvent()->id());
    query.exec();
    while (query.next()) {
        ui->cmb_club->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    ui->cmb_club->setCurrentIndex(0);
}

LicenseNumberDialog::~LicenseNumberDialog()
{
    delete ui;
}

void LicenseNumberDialog::fillTable(int row)
{
    QSqlQuery query;
    query.prepare("SELECT var_nachname || ', ' || var_vorname, int_startpassnummer, int_teilnehmerid FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) INNER JOIN tfx_vereine USING (int_vereineid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE int_veranstaltungenid=? AND int_vereineid=? AND int_runde=? ORDER BY tfx_wettkaempfe.var_nummer, "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", var_nachname, var_vorname");
    query.bindValue(0, this->m_event->mainEvent()->id());
    query.bindValue(1, QVariant(ui->cmb_club->itemData(ui->cmb_club->currentIndex())).toInt());
    query.bindValue(2, this->m_event->round());
    query.exec();
    model->setQuery(query);
    ui->tbl_pass->setFocus();
    ui->tbl_pass->selectRow(row);
    QHeaderView::ResizeMode resizeMode[] = {QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resize[] = {400,133,0};
    QString headers[3] = {"Name","Nummer","id"};
    for (int i=0;i<3;i++) {
        model->setHeaderData(i, Qt::Horizontal, headers[i]);
        ui->tbl_pass->horizontalHeader()->setSectionResizeMode(i, resizeMode[i]);
        ui->tbl_pass->horizontalHeader()->resizeSection(i, resize[i]);
    }
}

void LicenseNumberDialog::selectLine()
{
    ui->txt_number->setText(
        QVariant(model->data(model->index(ui->tbl_pass->currentIndex().row(), 1))).toString());
    ui->lbl_name->setText(
        QVariant(model->data(model->index(ui->tbl_pass->currentIndex().row(), 0))).toString());
    ui->txt_number->setFocus();
    ui->txt_number->setSelection(0, 1);
}

void LicenseNumberDialog::up()
{
    int row = ui->tbl_pass->currentIndex().row() - 1;
    if (row >= 0) {
        ui->tbl_pass->selectRow(row);
    }
}

void LicenseNumberDialog::down()
{
    int row = ui->tbl_pass->currentIndex().row() + 1;
    if (row < model->rowCount()) {
        ui->tbl_pass->selectRow(row);
    }
}

void LicenseNumberDialog::save()
{
    int row = ui->tbl_pass->currentIndex().row();
    QSqlQuery query;
    query.prepare("UPDATE tfx_teilnehmer SET int_startpassnummer=? WHERE int_teilnehmerid=?");
    query.bindValue(0, ui->txt_number->text());
    query.bindValue(1,
                    QVariant(model->data(model->index(ui->tbl_pass->currentIndex().row(), 2)))
                        .toInt());
    query.exec();
    fillTable(row);
}

bool LicenseNumberDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->txt_number) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *e = static_cast<QKeyEvent*>(event);
            if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Down) {
                save();
                down();
            } else if (e->key() == Qt::Key_Up) {
                save();
                up();
            } else if (e->key() == Qt::Key_0 || e->key() == Qt::Key_1 || e->key() == Qt::Key_2 || e->key() == Qt::Key_3 || e->key() == Qt::Key_4 || e->key() == Qt::Key_5 || e->key() == Qt::Key_6 || e->key() == Qt::Key_7 || e->key() == Qt::Key_8 || e->key() == Qt::Key_9 || e->key() == Qt::Key_Left || e->key() == Qt::Key_Right || e->key() == Qt::Key_Space) {
                return QDialog::eventFilter(obj, event);
            }
            return true;
        } else {
            return false;
        }
    } else {
        return QDialog::eventFilter(obj, event);
    }
}
