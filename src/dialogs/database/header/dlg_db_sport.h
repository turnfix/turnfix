#ifndef DLG_DB_SPORT_H
#define DLG_DB_SPORT_H
#include <QDialog>
#include "ui_dlg_db_sport.h"

class Db_Sp_Dialog : public QDialog, public Ui::Db_Sp_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Sp_Dialog(int tid=0, QWidget* parent=0);
      protected slots:
              void save();
      protected:
              int tnid;
};
#endif
