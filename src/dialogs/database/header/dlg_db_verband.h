#ifndef DLG_DB_VERBAND_H
#define DLG_DB_VERBAND_H

#include <QDialog>
#include "ui_dlg_db_verband.h"

class Db_Verband_Dialog : public QDialog, public Ui::Db_Verband_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Verband_Dialog(int v=0, QWidget* parent=0);
      protected slots:
              void save();
              void addCountry();
              void readCountries();
      protected:
              int vid;
};

#endif // DLG_DB_VERBAND_H
