#ifndef DLG_TN_H
#define DLG_TN_H
#include <QDialog>
#include "ui_dlg_tn.h"

class Event;

class Tn_Dialog : public QDialog, public Ui::Tn_Dialog_Ui {
    Q_OBJECT

public:
    Tn_Dialog(Event *event, int edit=0, QWidget* parent = nullptr);

private slots:
    void initData();
    void save();
    void checkUpdate();
    void checkDisziplinen();
    void checkJg();
    void changeDat();
    void addClub();
    void updateClubs();

private:
    Event *event;
    int editid;
};
#endif
