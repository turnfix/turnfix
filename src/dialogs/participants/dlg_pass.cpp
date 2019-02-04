#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QKeyEvent>
#include "header/dlg_pass.h"
#include "../../global/header/_global.h"

Pass_Dialog::Pass_Dialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    model = new QSqlQueryModel();
    tbl_pass->setModel(model);
    connect(but_close, SIGNAL(clicked()), this, SLOT(close()));
    connect(cmb_club, SIGNAL(currentIndexChanged(int)), this, SLOT(fillTable()));
    connect(tbl_pass->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectLine()));
    txt_number->installEventFilter(this);
    QSqlQuery query;
    query.prepare("SELECT int_vereineid, tfx_vereine.var_name FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid INNER JOIN tfx_vereine USING (int_vereineid) WHERE int_veranstaltungenid=? GROUP BY int_vereineid, tfx_vereine.var_name, int_start_ort ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name");
    query.bindValue(0,_global::checkHWK());
    query.exec();
    while (query.next()) {
        cmb_club->addItem(query.value(1).toString(),query.value(0).toInt());
    }
    cmb_club->setCurrentIndex(0);
}

void Pass_Dialog::fillTable(int row) {
    QSqlQuery query;
    query.prepare("SELECT var_nachname || ', ' || var_vorname, int_startpassnummer, int_teilnehmerid FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) INNER JOIN tfx_vereine USING (int_vereineid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE int_veranstaltungenid=? AND int_vereineid=? AND int_runde=? ORDER BY tfx_wettkaempfe.var_nummer, "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", var_nachname, var_vorname");
    query.bindValue(0,_global::checkHWK());
    query.bindValue(1,QVariant(cmb_club->itemData(cmb_club->currentIndex())).toInt());
    query.bindValue(2,_global::getRunde());
    query.exec();
    model->setQuery(query);
    tbl_pass->setFocus();
    tbl_pass->selectRow(row);
    QHeaderView::ResizeMode resizeMode[] = {QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resize[] = {400,133,0};
    QString headers[3] = {"Name","Nummer","id"};
    for (int i=0;i<3;i++) {
        model->setHeaderData(i, Qt::Horizontal, headers[i]);
        tbl_pass->horizontalHeader()->setResizeMode(i, resizeMode[i]);
        tbl_pass->horizontalHeader()->resizeSection(i, resize[i]);
    }
}

void Pass_Dialog::selectLine() {
    txt_number->setText(QVariant(model->data(model->index(tbl_pass->currentIndex().row(),1))).toString());
    lbl_name->setText(QVariant(model->data(model->index(tbl_pass->currentIndex().row(),0))).toString());
    txt_number->setFocus();
    txt_number->setSelection(0,1);
}

void Pass_Dialog::up() {
    int row = tbl_pass->currentIndex().row()-1;
    if (row >= 0) {
        tbl_pass->selectRow(row);
    }
}

void Pass_Dialog::down() {
    int row = tbl_pass->currentIndex().row()+1;
    if (row < model->rowCount()) {
        tbl_pass->selectRow(row);
    }
}

void Pass_Dialog::save() {
    int row = tbl_pass->currentIndex().row();
    QSqlQuery query;
    query.prepare("UPDATE tfx_teilnehmer SET int_startpassnummer=? WHERE int_teilnehmerid=?");
    query.bindValue(0,txt_number->text());
    query.bindValue(1,QVariant(model->data(model->index(tbl_pass->currentIndex().row(),2))).toInt());
    query.exec();
    fillTable(row);
}

bool Pass_Dialog::eventFilter(QObject *obj, QEvent *event) {
    if (obj == txt_number) {
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
