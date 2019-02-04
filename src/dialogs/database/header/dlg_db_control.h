#ifndef DLG_DB_CONTROL_H
#define DLG_DB_CONTROL_H

#include <QMainWindow>
#include "ui_dlg_db_control.h"

class QSqlQueryModel;
class QSortFilterProxyModel;

class Db_Control_Dialog : public QMainWindow, public Ui::Db_Control_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Control_Dialog(QWidget* parent=0, int type=1);
      protected slots:
              void loaddb();
              void getData();
              void add();
              void edit();
              void del();
              void updateFilterColumn(int index);
              void updateFilterText(QString text);
      protected:
          int currtype;
          QSqlQueryModel *db_model;
          QSortFilterProxyModel *db_sort_model;
};
#endif
