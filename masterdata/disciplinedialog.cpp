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
