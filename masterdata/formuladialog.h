#ifndef FORMULADIALOG_H
#define FORMULADIALOG_H

#include <QDialog>

namespace Ui {
class FormulaDialog;
}

class FormulaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FormulaDialog(int fid = 0, QWidget *parent = nullptr);
    ~FormulaDialog();

private slots:
    void save();
    void addPart();
    void checkFormula();


private:
    Ui::FormulaDialog *ui;
    int formelid;
};

#endif
