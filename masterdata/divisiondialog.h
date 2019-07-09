#ifndef DIVISIONDIALOG_H
#define DIVISIONDIALOG_H

#include <QDialog>

namespace Ui {
class DivisionDialog;
}

class DivisionDialog : public QDialog
{
    Q_OBJECT

public:
    DivisionDialog(int tid = 0, QWidget *parent = nullptr);
    ~DivisionDialog();

private slots:
    void save();

private:
    Ui::DivisionDialog *ui;
    int tnid;
};
#endif
