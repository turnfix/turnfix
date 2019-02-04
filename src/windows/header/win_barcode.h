#ifndef WIN_STATUSTOOL_H
#define WIN_STATUSTOOL_H
#include <QMainWindow>
#include "../../export/header/drucken.h"
#include "ui_win_barcode.h"

class Barcode_Tool : public QMainWindow, public Ui::Barcode_Tool_Ui {
    Q_OBJECT;
public:
    Barcode_Tool();
protected slots:
    void edit();
    void nostart();
    void changeStatus();
protected:
    QList<QStringList> values;
    int id;
};
#endif
