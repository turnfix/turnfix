#ifndef DLG_SELECT_RG_H
#define DLG_SELECT_RG_H
#include <QDialog>
#include "ui_dlg_select_rg.h"

class Event;

class Select_Rg_Dialog : public QDialog, public Ui::Select_Rg_Dialog_Ui {
    Q_OBJECT
public:
    Select_Rg_Dialog(Event *event, QWidget* parent = nullptr);

public slots:
    QStringList getRg();

private slots:
    void select1();

private:
    void initData();

    Event *event;
    QStringList rg;
};
#endif
