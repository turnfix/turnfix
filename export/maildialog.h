#ifndef MAILDIALOG_H
#define MAILDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "ui_maildialog.h"

class QProgressDialog;

class Event;

class MailDialog : public QDialog, public Ui::MailDialogUi {
    Q_OBJECT

public:
    MailDialog(Event *m_event, QWidget* parent=0);

public slots:
    void setDetailInfo(int di);

private slots:
    void selClubs();
    void sendMails();
    void updateDialogLabel(QString text);
    void closeDialog();
    void message(QString icon,QString titel, QString text);

private:
    Event *m_event;
    QStringList vereine;
    QProgressDialog *dlg;
    int detailinfo;
};


#endif
