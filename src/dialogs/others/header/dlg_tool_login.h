#ifndef DLG_TOOL_LOGIN_H
#define DLG_TOOL_LOGIN_H
#include <QDialog>
#include "ui_dlg_tool_login.h"

class QStandardItemModel;

class Tool_Login_Dialog : public QDialog, public Ui::Tool_Login_Dialog_Ui {
      Q_OBJECT;
      public:
             Tool_Login_Dialog(QWidget* parent=0);
      protected slots:
              void connectdb();
              void selectEvent();
              void updateList();
      public slots:
              void autoLogin();
      protected:
              QStandardItemModel *model;
};
#endif
