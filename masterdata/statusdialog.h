#ifndef STATUSDIALOG_H
#define STATUSDIALOG_H

#include <QDialog>

namespace Ui {
class StatusDialog;
}

class StatusDialog : public QDialog
{
    Q_OBJECT

public:
    StatusDialog(int tid = 0, QWidget *parent = nullptr);
    ~StatusDialog();

private slots:
    void save();
    void colorChooser();

private:
    Ui::StatusDialog *ui;
    int tnid;
    QColor col;
};
#endif
