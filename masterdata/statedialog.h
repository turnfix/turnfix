#ifndef STATEDIALOG_H
#define STATEDIALOG_H

#include <QDialog>

namespace Ui {
class StateDialog;
}

class StateDialog : public QDialog
{
    Q_OBJECT

public:
    StateDialog(int v = 0, QWidget *parent = nullptr);
    ~StateDialog();

private slots:
    void save();
    void addCountry();
    void readCountries();

private:
    Ui::StateDialog *ui;
    int vid;
};

#endif
