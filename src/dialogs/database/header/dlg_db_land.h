#ifndef DLG_DB_LAND_H
#define DLG_DB_LAND_H
#include <QDialog>
#include "ui_dlg_db_land.h"

class Db_Land_Dialog : public QDialog, public Ui::Db_Land_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Land_Dialog(int lid=0, QWidget* parent=0);
      protected slots:
              void save();
      protected:
              int lid;
};

#endif // DLG_DB_LAND_H
