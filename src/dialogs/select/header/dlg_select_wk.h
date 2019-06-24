#ifndef DLG_SELECT_WK_H
#define DLG_SELECT_WK_H
#include <QDialog>
#include "ui_dlg_select_wk.h"

class Event;

class Select_Wk_Dialog : public QDialog, public Ui::Select_Wk_Dialog_Ui {
    Q_OBJECT

public:
    Select_Wk_Dialog(Event *Event, QWidget* parent = nullptr);

public slots:
    QString getWk();

private slots:
    void initData();
    void select1();

private:
    Event *event;
    QString wk;
};
#endif
