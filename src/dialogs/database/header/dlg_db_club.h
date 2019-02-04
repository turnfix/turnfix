#ifndef DLG_DB_CLUB_H
#define DLG_DB_CLUB_H
#include <QDialog>
#include "ui_dlg_db_club.h"

class Db_Club_Dialog : public QDialog, public Ui::Db_Club_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Club_Dialog(int tid=0, QWidget* parent=0);
      protected slots:
              void save();
              void updateSlider();
              void updateOrt();
              void addPerson();
              void readPersons();
              void addGau();
              void readGaue();
              void updateGauInfo();
      protected:
              int vnid;
};
#endif
