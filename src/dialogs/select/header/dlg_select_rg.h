#ifndef DLG_SELECT_RG_H
#define DLG_SELECT_RG_H
#include <QDialog>
#include "ui_dlg_select_rg.h"

class Select_Rg_Dialog : public QDialog, public Ui::Select_Rg_Dialog_Ui {
      Q_OBJECT;
      public:
             Select_Rg_Dialog(QWidget* parent=0);
      public slots:
             QStringList getRg();
      protected slots:
             void initData();
              void select1();
      protected:
              QStringList rg;
};
#endif
