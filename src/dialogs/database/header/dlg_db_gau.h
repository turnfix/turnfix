#ifndef DLG_DB_GAU_H
#define DLG_DB_GAU_H

#include <QDialog>
#include "ui_dlg_db_gau.h"

class Db_Gau_Dialog : public QDialog, public Ui::Db_Gau_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Gau_Dialog(int v=0, QWidget* parent=0);
      protected slots:
              void save();
              void addVerband();
              void readVerbaende();
      protected:
              int vid;
};

#endif // DLG_DB_GAU_H
