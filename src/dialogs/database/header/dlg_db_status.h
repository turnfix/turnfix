#ifndef DLG_DB_STATUS_H
#define DLG_DB_STATUS_H
#include <QDialog>
#include "ui_dlg_db_status.h"

class Db_St_Dialog : public QDialog, public Ui::Db_St_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_St_Dialog(int tid=0, QWidget* parent=0);
      protected slots:
              void save();
              void colorChooser();
      protected:
              int tnid;
              QColor col;
};
#endif
