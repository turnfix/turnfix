#ifndef DLG_DB_ACC_H
#define DLG_DB_ACC_H
#include <QDialog>
#include "ui_dlg_db_acc.h"

class Db_Acc_Dialog : public QDialog, public Ui::Db_Acc_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Acc_Dialog(int tid=0, QWidget* parent=0);
      protected slots:
              void save();
      protected:
              int acid;
};
#endif
