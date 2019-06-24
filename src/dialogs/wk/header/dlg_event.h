#ifndef DLG_EVENT_H
#define DLG_EVENT_H
#include <QDialog>
#include "ui_dlg_event.h"

class Event;

class Event_Dialog : public QDialog, public Ui::Event_Dialog_Ui {
    Q_OBJECT

public:
    Event_Dialog(Event *event, int editid=0, QWidget* parent = nullptr);

public slots:
    int getWknr();

private slots:
    void initData();
    void save();
    void addPerson();
    void addLocation();
    void addAccount();
    void readPersons();
    void readLocations();
    void readAccounts();

private:
    Event *event;
    int wknr;
    int editid;
};
#endif
