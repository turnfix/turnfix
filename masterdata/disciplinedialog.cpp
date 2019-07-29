#include "disciplinedialog.h"
#include "model/entity/discipline.h"
#include "model/entitymanager.h"
#include "model/repository/disciplinerepository.h"
#include "model/view/disciplinefieldmodel.h"
#include "model/view/formulamodel.h"
#include "model/view/sportmodel.h"
#include "ui_disciplinedialog.h"
#include <QKeyEvent>
#include <QMessageBox>

DisciplineDialog::DisciplineDialog(Discipline *discipline, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DisciplineDialog)
    , m_em(em)
    , m_discipline(discipline)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    auto sportModel = new SportModel(m_em, this);
    sportModel->fetchSports();
    ui->cmb_sport->setModel(sportModel);

    auto formulaModel = new FormulaModel(m_em, this);
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
        ui->cmb_sport->findData(m_discipline->sportId(), TF::ItemDataRole::IdRole));
    ui->sbx_try->setValue(m_discipline->attempts());
    ui->txt_icon->setText(m_discipline->icon());
    ui->txt_shortcut->setText(m_discipline->shortcut());
    ui->sbx_comma->setValue(m_discipline->decimals());
    ui->txt_unit->setText(m_discipline->unit());
    ui->txt_short1->setText(m_discipline->shortName1());
    ui->txt_short2->setText(m_discipline->shortName2());
    ui->chk_run->setChecked(m_discipline->lanes());
    ui->cmb_formel->setCurrentIndex(
        ui->cmb_formel->findData(m_discipline->formulaId(), TF::ItemDataRole::IdRole));
    ui->gbx_berechnen->setChecked(m_discipline->calculate());
}

DisciplineDialog::~DisciplineDialog()
{
    delete ui;
}

void DisciplineDialog::save()
{
    if (ui->cmb_sport->currentIndex() == -1) {
        QMessageBox::warning(this, tr("Fehlender Wert"), tr("Bitte eine Sportart wählen."));
        return;
    }

    m_discipline->setName(ui->txt_name->text());
    m_discipline->setResultFormula(ui->txt_formula->text());
    m_discipline->setInputMask(ui->cmb_mask->currentText());
    m_discipline->setSport(qvariant_cast<Sport *>(ui->cmb_sport->currentData()));
    m_discipline->setAttempts(ui->sbx_try->value());
    m_discipline->setIcon(ui->txt_icon->text());
    m_discipline->setDecimals(ui->sbx_comma->value());
    m_discipline->setUnit(ui->txt_unit->text());
    m_discipline->setShortName1(ui->txt_short1->text());
    m_discipline->setShortName2(ui->txt_short2->text());
    m_discipline->setLanes(ui->chk_run->isChecked());
    m_discipline->setMen(ui->chk_m->isChecked());
    m_discipline->setFormula(qvariant_cast<Formula *>(ui->cmb_formel->currentData()));
    m_discipline->setCalculate(ui->gbx_berechnen->isChecked());

    m_em->startTransaction();
    m_em->disciplineRepository()->persist(m_discipline);
    m_fieldModel->persistChanges();
    m_em->commitTransaction();
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
