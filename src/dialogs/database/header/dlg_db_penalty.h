#ifndef DLG_DB_PENALTY_H
#define DLG_DB_PENALTY_H

#include <QDialog>
#include "ui_dlg_db_penalty.h"

class Db_Penalty_Dialog : public QDialog, public Ui::Db_Penalty_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Penalty_Dialog(int tid=0, QWidget* parent=0);
      protected slots:
              void save();
      protected:
              int pid;
};


#endif // DLG_DB_PENALTY_H
