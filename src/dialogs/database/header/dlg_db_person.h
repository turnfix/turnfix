#ifndef DLG_DB_PERSON_H
#define DLG_DB_PERSON_H
#include <QDialog>
#include "ui_dlg_db_person.h"

class Db_Pe_Dialog : public QDialog, public Ui::Db_Pe_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Pe_Dialog(int tid=0, QWidget* parent=0);
      protected slots:
              void save();
      protected:
              int tnid;
};
#endif
