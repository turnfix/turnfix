#ifndef CHECKDATABASEDIALOG_H
#define CHECKDATABASEDIALOG_H

#include <QDialog>

namespace Ui {
    class CheckDatabaseDialog;
}

class CheckDatabaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CheckDatabaseDialog(QWidget *parent = 0);
    ~CheckDatabaseDialog();

private:
    Ui::CheckDatabaseDialog *ui;
    void check();

private slots:
    void enableCloseButton();
};

#endif // CHECKDATABASEDIALOG_H
