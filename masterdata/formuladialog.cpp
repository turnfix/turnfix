#include "formuladialog.h"
#include "libs/fparser/fparser.hh"
#include "model/entity/formula.h"
#include "model/entitymanager.h"
#include "model/repository/formularepository.h"
#include "ui_formuladialog.h"
#include <QMessageBox>
#include <QSqlQuery>

FormulaDialog::FormulaDialog(Formula *formula, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FormulaDialog)
    , m_em(em)
    , m_formula(formula)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    connect(ui->bbx_done, SIGNAL(accepted()), this, SLOT(save()));
    connect(ui->bbx_done, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui->but_add, SIGNAL(clicked()), this, SLOT(addPart()));
    connect(ui->but_check, SIGNAL(clicked()), this, SLOT(checkFormula()));

    if (m_formula == nullptr) {
        m_formula = new Formula();
    }

    ui->txt_name->setText(m_formula->name());
    ui->txt_formel->setText(m_formula->formula());
}

FormulaDialog::~FormulaDialog()
{
    delete ui;
}

void FormulaDialog::save()
{
    m_formula->setName(ui->txt_name->text());
    m_formula->setFormula(ui->txt_formel->text());

    m_em->formulaRepository()->persist(m_formula);
    done(1);
}

void FormulaDialog::addPart()
{
    int start = ui->txt_formel->selectionStart();
    if (start==-1) start = 0;
    ui->txt_formel->insert(ui->cmb_formel->currentText().split(": ").at(1));
    ui->txt_formel->setSelection(ui->txt_formel->text().indexOf("(x",start)+1,1);
    ui->txt_formel->setFocus();
}

void FormulaDialog::checkFormula()
{
    QStringList lst;
    lst << "A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J"<<"K"<<"L"<<"M"<<"N"<<"O"<<"P"<<"Q"<<"R"<<"S"<<"T"<<"U"<<"V"<<"W"<<"X"<<"Y"<<"Z";
    QString vars;
    for (int i=0;i<lst.size();i++) {
        if (ui->txt_formel->text().contains(lst.at(i))) {
            vars += lst.at(i)+",";
        }
    }
    vars = vars.left(vars.length()-1);
    FunctionParser fparser;
    fparser.Parse(ui->txt_formel->text().toStdString(),vars.toStdString());
    QString error = fparser.ErrorMsg();
    if (error == "") {
        error = "Keine Fehler gefunden.";
    }
    QMessageBox::information(this,"Formelüberprüfung",error);
}

Formula *FormulaDialog::formula()
{
    return m_formula;
}
