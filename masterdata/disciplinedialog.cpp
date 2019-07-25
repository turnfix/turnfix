#include "disciplinedialog.h"
#include "model/entity/discipline.h"
#include "model/view/disciplinefieldmodel.h"
#include "model/view/formulamodel.h"
#include "model/view/sportmodel.h"
#include "ui_disciplinedialog.h"
#include <QKeyEvent>

DisciplineDialog::DisciplineDialog(Discipline *discipline, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DisciplineDialog)
    , m_em(em)
    , m_discipline(discipline)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    SportModel *sportModel = new SportModel(m_em, this);
    sportModel->fetchSports();
    ui->cmb_sport->setModel(sportModel);

    FormulaModel *formulaModel = new FormulaModel(m_em, this);
    formulaModel->fetchFormulas();
    ui->cmb_formel->setModel(formulaModel);

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_add, SIGNAL(clicked()), this, SLOT(addListItem()));
    connect(ui->but_del, SIGNAL(clicked()), this, SLOT(delListItem()));
    connect(ui->but_up, SIGNAL(clicked()), this, SLOT(up()));
    connect(ui->but_down, SIGNAL(clicked()), this, SLOT(down()));

    if (m_discipline == nullptr) {
        m_discipline = new Discipline();
    }

    m_fieldModel = new DisciplineFieldModel(em, this);
    m_fieldModel->fetchFields(m_discipline);

    ui->tbl_fields->setModel(m_fieldModel);
    ui->tbl_fields->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tbl_fields->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tbl_fields->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tbl_fields->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    ui->txt_name->setText(m_discipline->name());
    ui->txt_formula->setText(m_discipline->resultFormula());
    ui->cmb_mask->setCurrentIndex(ui->cmb_mask->findText(m_discipline->inputMask()));
    ui->chk_m->setChecked(m_discipline->men());
    ui->chk_w->setChecked(m_discipline->women());
    ui->cmb_sport->setCurrentIndex(
        ui->cmb_sport->findData(QVariant::fromValue(m_discipline->sport())));
    ui->sbx_try->setValue(m_discipline->attempts());
    ui->txt_icon->setText(m_discipline->icon());
    ui->txt_shortcut->setText(m_discipline->shortcut());
    ui->sbx_comma->setValue(m_discipline->decimals());
    ui->txt_unit->setText(m_discipline->unit());
    ui->txt_short1->setText(m_discipline->shortName1());
    ui->txt_short2->setText(m_discipline->shortName2());
    ui->chk_run->setChecked(m_discipline->lanes());
    ui->cmb_formel->setCurrentIndex(
        ui->cmb_formel->findData(QVariant::fromValue(m_discipline->formula())));
    ui->gbx_berechnen->setChecked(m_discipline->calculate());
}

DisciplineDialog::~DisciplineDialog()
{
    delete ui;
}

void DisciplineDialog::save()
{
    //    QSqlQuery query6;
    //    if (disid == 0) {
    //        query6.prepare("INSERT INTO tfx_disziplinen (var_name,var_formel,var_maske,int_sportid,int_versuche,var_icon,var_kuerzel,int_berechnung,var_einheit,var_kurz1,var_kurz2,bol_bahnen,bol_m,bol_w, int_formelid, bol_berechnen) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    //    } else {
    //        query6.prepare("UPDATE tfx_disziplinen SET var_name=?,var_formel=?,var_maske=?,int_sportid=?,int_versuche=?,var_icon=?,var_kuerzel=?,int_berechnung=?,var_einheit=?,var_kurz1=?,var_kurz2=?,bol_bahnen=?,bol_m=?,bol_w=?, int_formelid=?, bol_berechnen=? WHERE int_disziplinenid=?");
    //        query6.bindValue(16,disid);
    //    }
    //    query6.bindValue(0, ui->txt_name->text());
    //    query6.bindValue(1, ui->txt_formula->text());
    //    query6.bindValue(2, ui->cmb_mask->currentText());
    //    query6.bindValue(3, ui->cmb_sport->itemData(ui->cmb_sport->currentIndex()));
    //    query6.bindValue(4, ui->sbx_try->value());
    //    query6.bindValue(5, ui->txt_icon->text());
    //    query6.bindValue(6, ui->txt_shortcut->text());
    //    query6.bindValue(7, ui->sbx_comma->value());
    //    query6.bindValue(8, ui->txt_unit->text());
    //    query6.bindValue(9, ui->txt_short1->text());
    //    query6.bindValue(10, ui->txt_short2->text());
    //    query6.bindValue(11, ui->chk_run->isChecked());
    //    query6.bindValue(12, ui->chk_m->isChecked());
    //    query6.bindValue(13, ui->chk_w->isChecked());
    //    query6.bindValue(14, ui->cmb_formel->itemData(ui->cmb_formel->currentIndex()));
    //    query6.bindValue(15, ui->gbx_berechnen->isChecked());
    //    query6.exec();
    //    if (disid == 0) {
    //        if (_global::getDBTyp() == 0) {
    //            QSqlQuery query7("SELECT last_value FROM tfx_disziplinen_int_disziplinenid_seq");
    //            query7.next();
    //            disid = query7.value(0).toInt();
    //        } else {
    //            disid = query6.lastInsertId().toInt();
    //        }
    //    }
    //    for (int i = 0; i < m_fieldModel->rowCount(); i++) {
    //        QSqlQuery query8;
    //        query8.prepare("SELECT int_disziplinen_felderid FROM tfx_disziplinen_felder WHERE int_disziplinenid=? AND var_name=?");
    //        query8.bindValue(0,disid);
    //        query8.bindValue(1, m_fieldModel->data(m_fieldModel->item(i, 0)->index()).toString());
    //        query8.exec();
    //        if (_global::querySize(query8) == 0) {
    //            if (m_fieldModel->data(m_fieldModel->item(i, 0)->index()).toString().length() > 0) {
    //                QSqlQuery query9;
    //                query9.prepare("INSERT INTO tfx_disziplinen_felder (int_disziplinenid,var_name,int_sortierung,bol_endwert,bol_ausgangswert,bol_enabled) VALUES (?,?,?,?,?,?)");
    //                query9.bindValue(0,disid);
    //                query9.bindValue(1,
    //                                 m_fieldModel->data(m_fieldModel->item(i, 0)->index()).toString());
    //                query9.bindValue(2,i+1);
    //                bool checked;
    //                if (m_fieldModel->item(i, 1)->checkState() == Qt::Checked)
    //                    checked = true;
    //                else
    //                    checked = false;
    //                query9.bindValue(3,checked);
    //                if (m_fieldModel->item(i, 2)->checkState() == Qt::Checked)
    //                    checked = true;
    //                else
    //                    checked = false;
    //                query9.bindValue(4,checked);
    //                if (m_fieldModel->item(i, 3)->checkState() == Qt::Checked)
    //                    checked = true;
    //                else
    //                    checked = false;
    //                query9.bindValue(5,checked);
    //                query9.exec();
    //            }
    //        } else {
    //            query8.next();
    //            QSqlQuery query12;
    //            query12.prepare("UPDATE tfx_disziplinen_felder SET int_sortierung=?, bol_endwert=?, bol_ausgangswert=?, bol_enabled=? WHERE int_disziplinen_felderid=?");
    //            query12.bindValue(0,i+1);
    //            bool checked;
    //            if (m_fieldModel->item(i, 1)->checkState() == Qt::Checked)
    //                checked = true;
    //            else
    //                checked = false;
    //            query12.bindValue(1,checked);
    //            if (m_fieldModel->item(i, 2)->checkState() == Qt::Checked)
    //                checked = true;
    //            else
    //                checked = false;
    //            query12.bindValue(2,checked);
    //            if (m_fieldModel->item(i, 3)->checkState() == Qt::Checked)
    //                checked = true;
    //            else
    //                checked = false;
    //            query12.bindValue(3,checked);
    //            query12.bindValue(4,query8.value(0).toInt());
    //            query12.exec();
    //        }
    //    }
    //    QSqlQuery query10;
    //    query10.prepare("SELECT var_name, int_disziplinen_felderid FROM tfx_disziplinen_felder WHERE int_disziplinenid=?");
    //    query10.bindValue(0,disid);
    //    query10.exec();
    //    while (query10.next()) {
    //        int test = 0;
    //        for (int i = 0; i < m_fieldModel->rowCount(); i++) {
    //            if (m_fieldModel->data(m_fieldModel->item(i, 0)->index()).toString()
    //                == query10.value(0).toString()) {
    //                test = 1;
    //                break;
    //            }
    //        }
    //        if (test == 0) {
    //            QSqlQuery query11;
    //            query11.prepare("DELETE FROM tfx_disziplinen_felder WHERE int_disziplinen_felderid=?");
    //            query11.bindValue(0,query10.value(1).toInt());
    //            query11.exec();
    //        }
    //    }
    done(1);
}

void DisciplineDialog::addListItem()
{
    m_fieldModel->addField();
    ui->tbl_fields->selectRow(m_fieldModel->rowCount() - 1);
}

void DisciplineDialog::delListItem()
{
    m_fieldModel->removeField(ui->tbl_fields->currentIndex());
    ui->tbl_fields->selectRow(0);
}

void DisciplineDialog::up()
{
    QModelIndex index = ui->tbl_fields->currentIndex();
    if (index.row() > 0) {
        m_fieldModel->moveUp(index);
        ui->tbl_fields->selectRow(index.row() - 1);
    }
}

void DisciplineDialog::down()
{
    QModelIndex index = ui->tbl_fields->currentIndex();
    if (index.row() < m_fieldModel->rowCount() - 1) {
        m_fieldModel->moveDown(index);
        ui->tbl_fields->selectRow(index.row() + 1);
    }
}

void DisciplineDialog::keyPressEvent(QKeyEvent *e)
{
    if (ui->txt_shortcut->hasFocus()) {
        QKeySequence s1 = QKeySequence(e->modifiers() + e->key());
        ui->txt_shortcut->setText(s1.toString().toLatin1());
    }
}
