#ifndef DLG_BARCODE_H
#define DLG_BARCODE_H
#include <QDialog>
#include "ui_dlg_barcode.h"

class Barcode_Dialog : public QDialog, public Ui::Barcode_Dialog_Ui {
      Q_OBJECT;
      public:
             Barcode_Dialog(QWidget* parent=0);
      public slots:
              QString getInput();
      protected:
              QString str;
              bool eventFilter(QObject *obj, QEvent *ev);
};
#endif
