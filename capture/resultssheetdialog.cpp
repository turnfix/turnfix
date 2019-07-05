#include "resultssheetdialog.h"
#include "libs/fparser/fparser.hh"
#include "model/objects/event.h"
#include "resultssheettablemodel.h"
#include "src/global/header/_delegates.h"
#include "src/global/header/_global.h"
#include "src/global/header/settings.h"
#include "ui_resultssheetdialog.h"
#include <math.h>
#include <QMessageBox>
#include <QSqlQuery>

ResultsSheetDialog::ResultsSheetDialog(Event *event, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResultsSheetDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);

    this->event = event;

    pe_model = new ResultsSheetTableModel(event);
    ui->pe_table->setModel(pe_model);
    ui->chk_jury->setChecked(Settings::juryResults);
    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(saveClose()));
    connect(ui->chk_jury, SIGNAL(stateChanged(int)), this, SLOT(fillPETable()));
    connect(ui->chk_jury, SIGNAL(stateChanged(int)), this, SLOT(saveJuryMethod()));
}

void ResultsSheetDialog::init(QString r, int g, bool k)
{
    riege = r;
    geraet = g;
    kuer = k;
    QSqlQuery query;
    query.prepare("SELECT var_icon, var_name, int_versuche, bol_berechnen FROM tfx_disziplinen WHERE int_disziplinenid=?");
    query.bindValue(0, geraet);
    query.exec();
    query.next();
    ui->lbl_icon->setPixmap(query.value(0).toString());
    QString kp;
    if (kuer) {
        kp = " (Kür)";
    } else {
        kp = " (Pflicht)";
    }
    ui->lbl_disrg->setText(query.value(1).toString() + kp + " - Riege " + riege);
    versuche = query.value(2).toInt();
    berechnen = query.value(3).toBool();
    QSqlQuery query3;
    query3.prepare("SELECT int_statusid, var_name, ary_colorcode FROM tfx_status WHERE bol_bogen='true' ORDER BY int_statusid ASC");
    query3.exec();
    while(query3.next()) {
        ui->cmb_status1->addItem(query3.value(1).toString(), query3.value(0).toInt());
        QList<int> color = _global::splitColorArray(query3.value(2).toString());
        ui->cmb_status1->setItemData(ui->cmb_status1->count() - 1,
                                     QColor(color.at(0), color.at(1), color.at(2)),
                                     Qt::BackgroundColorRole);
    }
    QSqlQuery query2;
    query2.prepare("SELECT int_statusid FROM tfx_riegen_x_disziplinen WHERE int_veranstaltungenid=? AND int_disziplinenid=? AND var_riege=? AND int_runde=?");
    query2.bindValue(0, this->event->getMainEventId());
    query2.bindValue(1, geraet);
    query2.bindValue(2, riege);
    query2.bindValue(3, this->event->getRound());
    query2.exec();
    query2.next();
    ui->cmb_status1->setCurrentIndex(ui->cmb_status1->findData(query2.value(0).toInt()));
    connect(ui->cmb_status1, SIGNAL(currentIndexChanged(int)), this, SLOT(statusChange1()));
    fillPETable();
}

void ResultsSheetDialog::fillPETable()
{
    ui->pe_table->clearSelection();
    pe_model->setTableData(riege, geraet, versuche, kuer, ui->chk_jury->isChecked());
    QList<QHeaderView::ResizeMode> resizeMode;
    resizeMode[0] = QHeaderView::ResizeToContents;
    resizeMode[1] = QHeaderView::Stretch;
    resizeMode[2] = QHeaderView::Stretch;
    resizeMode[3] = QHeaderView::ResizeToContents;
    QList<int> resize;
    resize[0] = 40;
    resize[1] = 200;
    resize[2] = 200;
    resize[3] = 35;
    int k=4;
    for (int i=k;i<pe_model->columnCount();i++) {
        resizeMode[k] = QHeaderView::ResizeToContents;
        resize[k] = 60;
        k++;
    }
    for (int i=0;i<pe_model->columnCount();i++) {
        ui->pe_table->horizontalHeader()->setSectionResizeMode(i, resizeMode[i]);
        ui->pe_table->horizontalHeader()->resizeSection(i, resize[i]);
        if (i > 3) {
            EditorDelegate *ed = new EditorDelegate;
            connect(ed, SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)), this, SLOT(finishEdit()));
            ui->pe_table->setItemDelegateForColumn(i, ed);
        } else if (i==3) {
            ui->pe_table->setItemDelegateForColumn(i, new AlignCItemDelegate);
        }
    }
    if (versuche>1) {
        for (int i=0;i<pe_model->rowCount();i++) {
            if (i%versuche == 0) {
                ui->pe_table->setSpan(i, 0, versuche, 1);
                ui->pe_table->setSpan(i, 1, versuche, 1);
                ui->pe_table->setSpan(i, 2, versuche, 1);
                ui->pe_table->setSpan(i, 3, versuche, 1);
            }
        }
    }
    ui->pe_table->setCurrentIndex(pe_model->index(0, 4));
    ui->pe_table->edit(ui->pe_table->currentIndex());
}

void ResultsSheetDialog::finishEdit()
{
    int row = ui->pe_table->currentIndex().row();
    int col = ui->pe_table->currentIndex().column();
    if (col == pe_model->columnCount()-1) {
        if (pe_model->index(row+1,4).isValid()) {
            ui->pe_table->setCurrentIndex(pe_model->index(row + 1, 4));
        } else {
            ui->pe_table->setCurrentIndex(pe_model->index(0, 4));
        }
    } else {
        ui->pe_table->setCurrentIndex(pe_model->index(row, col + 1));
        if (berechnen) calc();
    }
}

void ResultsSheetDialog::statusChange1()
{
    QSqlQuery query;
    query.prepare("UPDATE tfx_riegen_x_disziplinen SET int_statusid=? WHERE int_veranstaltungenid=? AND int_disziplinenid=? AND var_riege=? AND int_runde=?");
    query.bindValue(0, ui->cmb_status1->itemData(ui->cmb_status1->currentIndex()).toInt());
    query.bindValue(1, this->event->getMainEventId());
    query.bindValue(2, geraet);
    query.bindValue(3, riege);
    query.bindValue(4, this->event->getRound());
    query.exec();
}

void ResultsSheetDialog::saveClose()
{
    finishEdit();
    close();
}

void ResultsSheetDialog::calc()
{
    QStringList lst;
    lst << "A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J"<<"K"<<"L"<<"M"<<"N"<<"O"<<"P"<<"Q"<<"R"<<"S"<<"T"<<"U"<<"V"<<"W"<<"X"<<"Y"<<"Z";
    QSqlQuery query2;
    query2.prepare("SELECT tfx_formeln.var_formel, int_berechnung, var_einheit, var_maske FROM tfx_disziplinen LEFT JOIN tfx_formeln USING (int_formelid) WHERE int_disziplinenid=?");
    query2.bindValue(0,geraet);
    query2.exec();
    query2.next();
    QString vars;
    for (int i=0;i<lst.size();i++) {
        if (query2.value(0).toString().contains(lst.at(i))) {
            vars += lst.at(i)+",";
        }
    }
    vars = vars.left(vars.length()-1);
    FunctionParser fparser;
    fparser.Parse(query2.value(0).toString().toStdString(),vars.toStdString());
    int size = pe_model->columnCount()-5;
    QVector<double> Vars(size);
    double max=0.0;
    for (int i=4;i<pe_model->columnCount()-1;i++) {
        Vars[i - 4] = QVariant(
                          pe_model->data(pe_model->index(ui->pe_table->currentIndex().row(), i)))
                          .toDouble();
        if (Vars[i-4]>max) max = Vars[i-4];
    }
    double res = fparser.Eval(Vars.data());
    if (max==0) res = 0.0;
    pe_model->setData(pe_model->index(ui->pe_table->currentIndex().row(),
                                      pe_model->columnCount() - 1),
                      _global::strLeistung(res,
                                           query2.value(2).toString(),
                                           query2.value(3).toString(),
                                           query2.value(1).toInt()),
                      Qt::EditRole);
}

void ResultsSheetDialog::saveJuryMethod()
{
    Settings::updateJuryResults(ui->chk_jury->isChecked());
}
