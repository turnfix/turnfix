#ifndef BANKACCOUNTDIALOG_H
#define BANKACCOUNTDIALOG_H

#include <QDialog>

namespace Ui {
class BankAccountDialog;
}

class BankAccountDialog : public QDialog
{
    Q_OBJECT

public:
    BankAccountDialog(int tid = 0, QWidget *parent = nullptr);
    ~BankAccountDialog();

private slots:
    void save();

private:
    Ui::BankAccountDialog *ui;
    int acid;
};
#endif
