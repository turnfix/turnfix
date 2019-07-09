#include "disciplinedialog.h"
#include "src/global/header/_global.h"
#include "ui_disciplinedialog.h"
#include <QKeyEvent>
#include <QSqlQuery>
#include <QStandardItemModel>

DisciplineDialog::DisciplineDialog(int dis, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DisciplineDialog)
{
    disid = dis;
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QSqlQuery query("SELECT int_sportid, var_name FROM tfx_sport ORDER BY var_name");
    while (query.next()) {
        ui->cmb_sport->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    QSqlQuery query2("SELECT int_formelid, var_name, var_formel FROM tfx_formeln ORDER BY var_name");
    while (query2.next()) {
        ui->cmb_formel->addItem(query2.value(1).toString() + " [" + query2.value(2).toString() + "]",
                                query2.value(0).toInt());
    }
    model = new QStandardItemModel();
    model->setColumnCount(4);
    ui->tbl_fields->setModel(model);
    ui->tbl_fields->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    model->setHeaderData(0, Qt::Horizontal, "Feldbezeichnung");
    ui->tbl_fields->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    model->setHeaderData(1, Qt::Horizontal, "EW");
    ui->tbl_fields->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    model->setHeaderData(2, Qt::Horizontal, "AW");
    ui->tbl_fields->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    model->setHeaderData(3, Qt::Horizontal, "Anz.");
    if (disid != 0) {
        QSqlQuery query;
        query.prepare("SELECT * FROM tfx_disziplinen WHERE int_disziplinenid=?");
        query.bindValue(0,disid);
        query.exec();
        query.next();
        ui->txt_name->setText(query.value(2).toString());
        ui->txt_formula->setText(query.value(5).toString());
        ui->cmb_mask->setCurrentIndex(ui->cmb_mask->findText(query.value(6).toString()));
        ui->chk_m->setChecked(query.value(13).toBool());
        ui->chk_w->setChecked(query.value(14).toBool());
        ui->cmb_sport->setCurrentIndex(ui->cmb_sport->findData(query.value(1).toString()));
        ui->sbx_try->setValue(query.value(7).toInt());
        ui->txt_icon->setText(query.value(8).toString());
        ui->txt_shortcut->setText(query.value(9).toString());
        ui->sbx_comma->setValue(query.value(10).toInt());
        ui->txt_unit->setText(query.value(11).toString());
        ui->txt_short1->setText(query.value(3).toString());
        ui->txt_short2->setText(query.value(4).toString());
        ui->chk_run->setChecked(query.value(12).toBool());
        ui->cmb_formel->setCurrentIndex(ui->cmb_formel->findData(query.value(15).toInt()));
        ui->gbx_berechnen->setChecked(query.value(16).toBool());
        query.prepare("SELECT var_name, bol_endwert, bol_ausgangswert, bol_enabled FROM tfx_disziplinen_felder WHERE int_disziplinenid=? ORDER BY int_sortierung");
        query.bindValue(0,disid);
        query.exec();
        int i=0;
        while (query.next()) {
            model->setItem(i,0,new QStandardItem(query.value(0).toString()));
            QStandardItem *check = new QStandardItem();
            check->setCheckable(true);
            if (query.value(1).toBool()) check->setCheckState(Qt::Checked); else check->setCheckState(Qt::Unchecked);
            model->setItem(i,1,check);
            QStandardItem *check2 = new QStandardItem();
            check2->setCheckable(true);
            if (query.value(2).toBool()) check2->setCheckState(Qt::Checked); else check2->setCheckState(Qt::Unchecked);
            model->setItem(i,2,check2);
            QStandardItem *check3 = new QStandardItem();
            check3->setCheckable(true);
            if (query.value(3).toBool()) check3->setCheckState(Qt::Checked); else check3->setCheckState(Qt::Unchecked);
            model->setItem(i,3,check3);
            i++;
        }
        if (i > 0)
            ui->tbl_fields->selectRow(0);
    }
    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_add, SIGNAL(clicked()), this, SLOT(addListItem()));
    connect(ui->but_del, SIGNAL(clicked()), this, SLOT(delListItem()));
    connect(ui->but_up, SIGNAL(clicked()), this, SLOT(up()));
    connect(ui->but_down, SIGNAL(clicked()), this, SLOT(down()));
}

DisciplineDialog::~DisciplineDialog()
{
    delete ui;
}

void DisciplineDialog::save()
{
    QSqlQuery query6;
    if (disid == 0) {
        query6.prepare("INSERT INTO tfx_disziplinen (var_name,var_formel,var_maske,int_sportid,int_versuche,var_icon,var_kuerzel,int_berechnung,var_einheit,var_kurz1,var_kurz2,bol_bahnen,bol_m,bol_w, int_formelid, bol_berechnen) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_disziplinen SET var_name=?,var_formel=?,var_maske=?,int_sportid=?,int_versuche=?,var_icon=?,var_kuerzel=?,int_berechnung=?,var_einheit=?,var_kurz1=?,var_kurz2=?,bol_bahnen=?,bol_m=?,bol_w=?, int_formelid=?, bol_berechnen=? WHERE int_disziplinenid=?");
        query6.bindValue(16,disid);
    }
    query6.bindValue(0, ui->txt_name->text());
    query6.bindValue(1, ui->txt_formula->text());
    query6.bindValue(2, ui->cmb_mask->currentText());
    query6.bindValue(3, ui->cmb_sport->itemData(ui->cmb_sport->currentIndex()));
    query6.bindValue(4, ui->sbx_try->value());
    query6.bindValue(5, ui->txt_icon->text());
    query6.bindValue(6, ui->txt_shortcut->text());
    query6.bindValue(7, ui->sbx_comma->value());
    query6.bindValue(8, ui->txt_unit->text());
    query6.bindValue(9, ui->txt_short1->text());
    query6.bindValue(10, ui->txt_short2->text());
    query6.bindValue(11, ui->chk_run->isChecked());
    query6.bindValue(12, ui->chk_m->isChecked());
    query6.bindValue(13, ui->chk_w->isChecked());
    query6.bindValue(14, ui->cmb_formel->itemData(ui->cmb_formel->currentIndex()));
    query6.bindValue(15, ui->gbx_berechnen->isChecked());
    query6.exec();
    if (disid == 0) {
        if (_global::getDBTyp() == 0) {
            QSqlQuery query7("SELECT last_value FROM tfx_disziplinen_int_disziplinenid_seq");
            query7.next();
            disid = query7.value(0).toInt();
        } else {
            disid = query6.lastInsertId().toInt();
        }
    }
    for (int i=0;i<model->rowCount();i++) {
        QSqlQuery query8;
        query8.prepare("SELECT int_disziplinen_felderid FROM tfx_disziplinen_felder WHERE int_disziplinenid=? AND var_name=?");
        query8.bindValue(0,disid);
        query8.bindValue(1,model->data(model->item(i,0)->index()).toString());
        query8.exec();
        if (_global::querySize(query8) == 0) {
            if (model->data(model->item(i,0)->index()).toString().length() > 0) {
                QSqlQuery query9;
                query9.prepare("INSERT INTO tfx_disziplinen_felder (int_disziplinenid,var_name,int_sortierung,bol_endwert,bol_ausgangswert,bol_enabled) VALUES (?,?,?,?,?,?)");
                query9.bindValue(0,disid);
                query9.bindValue(1,model->data(model->item(i,0)->index()).toString());
                query9.bindValue(2,i+1);
                bool checked;
                if (model->item(i,1)->checkState() == Qt::Checked) checked = true; else checked = false;
                query9.bindValue(3,checked);
                if (model->item(i,2)->checkState() == Qt::Checked) checked = true; else checked = false;
                query9.bindValue(4,checked);
                if (model->item(i,3)->checkState() == Qt::Checked) checked = true; else checked = false;
                query9.bindValue(5,checked);
                query9.exec();
            }
        } else {
            query8.next();
            QSqlQuery query12;
            query12.prepare("UPDATE tfx_disziplinen_felder SET int_sortierung=?, bol_endwert=?, bol_ausgangswert=?, bol_enabled=? WHERE int_disziplinen_felderid=?");
            query12.bindValue(0,i+1);
            bool checked;
            if (model->item(i,1)->checkState() == Qt::Checked) checked = true; else checked = false;
            query12.bindValue(1,checked);
            if (model->item(i,2)->checkState() == Qt::Checked) checked = true; else checked = false;
            query12.bindValue(2,checked);
            if (model->item(i,3)->checkState() == Qt::Checked) checked = true; else checked = false;
            query12.bindValue(3,checked);
            query12.bindValue(4,query8.value(0).toInt());
            query12.exec();
        }
    }
    QSqlQuery query10;
    query10.prepare("SELECT var_name, int_disziplinen_felderid FROM tfx_disziplinen_felder WHERE int_disziplinenid=?");
    query10.bindValue(0,disid);
    query10.exec();
    while (query10.next()) {
        int test = 0;
        for (int i=0;i<model->rowCount();i++) {
            if (model->data(model->item(i,0)->index()).toString() == query10.value(0).toString()) {
                test = 1;
                break;
            }
        }
        if (test == 0) {
            QSqlQuery query11;
            query11.prepare("DELETE FROM tfx_disziplinen_felder WHERE int_disziplinen_felderid=?");
            query11.bindValue(0,query10.value(1).toInt());
            query11.exec();
        }
    }
    done(1);
}

void DisciplineDialog::addListItem()
{
    int row = model->rowCount();
    model->setItem(row,0,new QStandardItem(""));
    QStandardItem *check = new QStandardItem();
    check->setCheckable(true);
    model->setItem(row,1,check);
    QStandardItem *check2 = new QStandardItem();
    check2->setCheckable(true);
    model->setItem(row,2,check2);
    QStandardItem *check3 = new QStandardItem();
    check3->setCheckable(true);
    model->setItem(row,3,check3);
    ui->tbl_fields->selectRow(row);
}

void DisciplineDialog::delListItem()
{
    model->removeRow(ui->tbl_fields->currentIndex().row());
    ui->tbl_fields->selectRow(0);
}

void DisciplineDialog::up()
{
    int row = ui->tbl_fields->currentIndex().row();
    if (ui->tbl_fields->currentIndex().row() > 0) {
        QStandardItem *item = model->item(row-1,0)->clone();
        QStandardItem *item2 = model->item(row-1,1)->clone();
        QStandardItem *item3 = model->item(row-1,2)->clone();
        QStandardItem *item4 = model->item(row-1,3)->clone();
        model->setItem(row-1,0,model->item(row,0)->clone());
        model->setItem(row,0,item);
        model->setItem(row-1,1,model->item(row,1)->clone());
        model->setItem(row,1,item2);
        model->setItem(row-1,2,model->item(row,2)->clone());
        model->setItem(row,2,item3);
        model->setItem(row-1,3,model->item(row,3)->clone());
        model->setItem(row,3,item4);
        ui->tbl_fields->selectRow(row - 1);
    }
}

void DisciplineDialog::down()
{
    int row = ui->tbl_fields->currentIndex().row();
    if (ui->tbl_fields->currentIndex().row() < model->rowCount() - 1) {
        QStandardItem *item = model->item(row+1,0)->clone();
        QStandardItem *item2 = model->item(row+1,1)->clone();
        QStandardItem *item3 = model->item(row+1,2)->clone();
        QStandardItem *item4 = model->item(row+1,3)->clone();
        model->setItem(row+1,0,model->item(row,0)->clone());
        model->setItem(row,0,item);
        model->setItem(row+1,1,model->item(row,1)->clone());
        model->setItem(row,1,item2);
        model->setItem(row+1,2,model->item(row,2)->clone());
        model->setItem(row,2,item3);
        model->setItem(row+1,3,model->item(row,3)->clone());
        model->setItem(row,3,item4);
        ui->tbl_fields->selectRow(row + 1);
    }
}

void DisciplineDialog::keyPressEvent(QKeyEvent *e)
{
    if (ui->txt_shortcut->hasFocus()) {
        QKeySequence s1 = QKeySequence(e->modifiers() + e->key());
        ui->txt_shortcut->setText(s1.toString().toLatin1());
    }
}
