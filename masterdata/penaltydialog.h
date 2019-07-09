#ifndef PENALTYDIALOG_H
#define PENALTYDIALOG_H

#include <QDialog>

namespace Ui {
class PenaltyDialog;
}

class PenaltyDialog : public QDialog
{
    Q_OBJECT

public:
    PenaltyDialog(int tid = 0, QWidget *parent = nullptr);
    ~PenaltyDialog();

private slots:
    void save();

private:
    Ui::PenaltyDialog *ui;
    int pid;
};

#endif
