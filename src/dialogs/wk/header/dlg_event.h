#ifndef DLG_EVENT_H
#define DLG_EVENT_H
#include <QDialog>
#include "ui_dlg_event.h"

class Event_Dialog : public QDialog, public Ui::Event_Dialog_Ui {
      Q_OBJECT;
      public:
             Event_Dialog(int editid=0, QWidget* parent=0);
      public slots:
             int getWknr();
      protected slots:
             void initData();
              void save();
              void addPerson();
              void addLocation();
              void addAccount();
              void readPersons();
              void readLocations();
              void readAccounts();
      protected:
              int wknr;
              int editid;
};
#endif
