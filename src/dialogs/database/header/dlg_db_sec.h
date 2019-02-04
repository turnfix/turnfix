#ifndef DLG_DB_SEC_H
#define DLG_DB_SEC_H
#include <QDialog>
#include "ui_dlg_db_sec.h"

class Db_Sec_Dialog : public QDialog, public Ui::Db_Sec_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Sec_Dialog(int tid=0, QWidget* parent=0);
      protected slots:
              void save();
      protected:
              int tnid;
};
#endif
