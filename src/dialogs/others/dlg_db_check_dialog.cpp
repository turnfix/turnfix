#include <QSqlQuery>
#include "header/dlg_db_check_dialog.h"
#include "../../misc/header/trd_checktablesthread.h"

DB_Check_Dialog::DB_Check_Dialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    trd = new CheckTable_Thread;
    connect(trd,SIGNAL(newStatus(QString,QString)),this,SLOT(addListItem(QString,QString)));
    connect(trd,SIGNAL(finished()),this,SLOT(enableButton()));
    trd->start();
    connect(but_close,SIGNAL(clicked()),this,SLOT(close()));
}

void DB_Check_Dialog::addListItem(QString str,QString icon) {
    QListWidgetItem *itm = new QListWidgetItem(str);
    itm->setIcon(QIcon(":/appicons/"+icon));
    lst_checklist->addItem(itm);
    lst_checklist->scrollToBottom();
}

void DB_Check_Dialog::enableButton() {
    but_close->setEnabled(true);
}

void DB_Check_Dialog::close() {
    if (trd->getCheck()) {
        done(1);
    } else {
        done(0);
    }
}
