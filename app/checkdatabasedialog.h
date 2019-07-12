#ifndef CHECKDATABASEDIALOG_H
#define CHECKDATABASEDIALOG_H

#include <QDialog>

namespace Ui {
    class CheckDatabaseDialog;
}

class AbstractConnection;

class CheckDatabaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CheckDatabaseDialog(AbstractConnection *connection, QWidget *parent = nullptr);
    ~CheckDatabaseDialog() override;

private:
    Ui::CheckDatabaseDialog *ui;

private slots:
    void enableCloseButton();
};

#endif // CHECKDATABASEDIALOG_H
