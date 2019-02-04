#ifndef DLG_DB_CHECK_DIALOG_H
#define DLG_DB_CHECK_DIALOG_H

#include <QDialog>
#include "ui_dlg_db_check_dialog.h"

class CheckTable_Thread;

class DB_Check_Dialog : public QDialog, public Ui::DB_Check_Dialog_Ui {
    Q_OBJECT

public:
    DB_Check_Dialog(QWidget *parent = 0);

private slots:
    void addListItem(QString,QString);
    void close();
    void enableButton();

private:
    CheckTable_Thread *trd;
};

#endif // DLG_DB_CHECK_DIALOG_H
