#ifndef DLG_SELECT_CLUB_H
#define DLG_SELECT_CLUB_H
#include <QDialog>
#include "ui_dlg_select_club.h"

class QListWidget;

class Select_Club_Dialog : public QDialog, public Ui::Select_Club_Dialog_Ui {
      Q_OBJECT;
      public:
             Select_Club_Dialog(QWidget* parent=0);
      public slots:
             QStringList returnVereine();
      protected slots:
             void initData();
              void select1();
      protected:
              QStringList vereine;
};
#endif
