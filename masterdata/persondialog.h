#ifndef PERSONDIALOG_H
#define PERSONDIALOG_H

#include <QDialog>

namespace Ui {
class PersonDialog;
}

class PersonDialog : public QDialog
{
    Q_OBJECT

public:
    PersonDialog(int tid = 0, QWidget *parent = nullptr);
    ~PersonDialog();

protected slots:
    void save();

protected:
    Ui::PersonDialog *ui;
    int tnid;
};
#endif
