#ifndef DLG_LOGIN_H
#define DLG_LOGIN_H
#include <QDialog>
#include "ui_dlg_login.h"

class QStandardItemModel;
class QWizardPage;

class Login_Dialog : public QDialog, public Ui::Login_Dialog_Ui {
      Q_OBJECT;
      public:
             Login_Dialog(QWidget* parent=0);
      private slots:
              void connectdb ();
              void selectEvent();
              void updateList();
              void deleteEvent();
              bool checkDB();
      public slots:
              void autoLogin();
      private:
              QStandardItemModel *model;
      protected:
              virtual void closeEvent(QCloseEvent * e);
};
#endif
