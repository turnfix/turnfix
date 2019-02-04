#ifndef WIN_STATUSTOOL_H
#define WIN_STATUSTOOL_H
#include <QMainWindow>
#include "ui_win_statustool.h"

class Status_Tool : public QMainWindow, public Ui::Status_Tool_Ui {
    Q_OBJECT;
public:
    Status_Tool();
protected slots:
    void addWk();
    void addAll();
    void fillStatus();
};
#endif
