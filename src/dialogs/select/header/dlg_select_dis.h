#ifndef DLG_SELECT_DIS_H
#define DLG_SELECT_DIS_H
#include <QDialog>
#include "ui_dlg_select_dis.h"

class Select_Dis_Dialog : public QDialog, public Ui::Select_Dis_Dialog_Ui {
      Q_OBJECT;
      public:
             Select_Dis_Dialog(QWidget* parent=0);
      public slots:
             QList< QList<int> > getDis();
      protected slots:
             void initData();
              void select1();
      protected:
              QList< QList<int> > dis;
};
#endif
