#ifndef DLG_MAIL_H
#define DLG_MAIL_H

#include <QDialog>
#include <QMessageBox>
#include "ui_dlg_mail.h"

class QProgressDialog;

class Mail_Dialog : public QDialog, public Ui::Mail_Dialog_Ui {
    Q_OBJECT;
public:
    Mail_Dialog(QWidget* parent=0);
public slots:
    void setDetailInfo(int di);
private slots:
    void selClubs();
    void sendMails();
    void updateDialogLabel(QString text);
    void closeDialog();
    void message(QString icon,QString titel, QString text);
protected:
    QStringList vereine;
    QProgressDialog *dlg;
    int detailinfo;
};


#endif
