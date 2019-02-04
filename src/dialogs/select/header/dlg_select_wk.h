#ifndef DLG_SELECT_WK_H
#define DLG_SELECT_WK_H
#include <QDialog>
#include "ui_dlg_select_wk.h"

class Select_Wk_Dialog : public QDialog, public Ui::Select_Wk_Dialog_Ui {
      Q_OBJECT;
      public:
             Select_Wk_Dialog(QWidget* parent=0);
      public slots:
             QString getWk();
      protected slots:
             void initData();
              void select1();
      protected:
              QString wk;
};
#endif
