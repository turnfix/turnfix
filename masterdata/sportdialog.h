#ifndef SPORTDIALOG_H
#define SPORTDIALOG_H
#include <QDialog>

namespace Ui {
class SportDialog;
}

class SportDialog : public QDialog
{
    Q_OBJECT

public:
    SportDialog(int tid = 0, QWidget *parent = nullptr);
    ~SportDialog();

private slots:
    void save();

private:
    Ui::SportDialog *ui;
    int tnid;
};
#endif
