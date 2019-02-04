#ifndef DLG_DB_DIS_H
#define DLG_DB_DIS_H
#include <QDialog>
#include <QItemDelegate>
#include "ui_dlg_db_dis.h"

class QStandardItemModel;

class Db_Dis_Dialog : public QDialog, public Ui::Db_Dis_Dialog_Ui {
      Q_OBJECT;
      public:
             Db_Dis_Dialog(int tid=0, QWidget* parent=0);
      protected slots:
              void save();
              void addListItem();
              void delListItem();
              void up();
              void down();
      protected:
              int disid;
              QStandardItemModel *model;
              virtual void keyPressEvent( QKeyEvent *e );
};

#endif
