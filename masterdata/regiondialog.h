#ifndef REGIONDIALOG_H
#define REGIONDIALOG_H

#include <QDialog>

namespace Ui {
class RegionDialog;
}

class RegionDialog : public QDialog
{
    Q_OBJECT
public:
    RegionDialog(int v = 0, QWidget *parent = nullptr);
    ~RegionDialog();

private slots:
    void save();
    void addVerband();
    void readVerbaende();

private:
    Ui::RegionDialog *ui;
    int vid;
};

#endif
