#ifndef DLG_ABOUT_H
#define DLG_ABOUT_H
#include <QDialog>
#include "ui_dlg_about.h"

class About_Dialog : public QDialog, public Ui::About_Dialog_Ui {
      Q_OBJECT;
      public:
             About_Dialog(QWidget* parent=0) {
                 setupUi(this);
                 setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
                }
};
#endif
