#ifndef FORMULADIALOG_H
#define FORMULADIALOG_H

#include <QDialog>

namespace Ui {
class FormulaDialog;
}

class Formula;
class EntityManager;

class FormulaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FormulaDialog(Formula *formula, EntityManager *em, QWidget *parent = nullptr);
    ~FormulaDialog();

    Formula *formula();

private slots:
    void save();
    void addPart();
    void checkFormula();


private:
    Ui::FormulaDialog *ui;
    EntityManager *m_em;
    Formula *m_formula;
};

#endif
