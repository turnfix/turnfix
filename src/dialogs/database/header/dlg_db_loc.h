#ifndef DLG_DB_LOC_H
#define DLG_DB_LOC_H
#include <QDialog>
#include "ui_dlg_db_loc.h"

class Db_Loc_Dialog : public QDialog, public Ui::Db_Loc_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Loc_Dialog(int tid=0, QWidget* parent=0);
      protected slots:
              void save();
      protected:
              int lid;
};
#endif
