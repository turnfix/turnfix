#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
    class LoginDialog;
}

class EventTableModel;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
    EventTableModel *eventTableModel;

private slots:
    void doLogin();
    void errorHandler(QString errorText);
    void checkDatabase();
    void createEvent();
    void selectCurrentEvent();
};

#endif // LOGINDIALOG_H
