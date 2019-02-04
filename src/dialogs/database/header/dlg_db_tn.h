#ifndef DLG_DB_TN_H
#define DLG_DB_TN_H
#include <QDialog>
#include "ui_dlg_db_tn.h"

class Db_Tn_Dialog : public QDialog, public Ui::Db_Tn_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Tn_Dialog(int tid=0, QWidget* parent=0);
             void setVerein(QString verein);
      protected slots:
              void save();
              void changeDat();
      protected:
              int tnid;
};
#endif
