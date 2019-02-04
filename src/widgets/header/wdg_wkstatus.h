#ifndef WDG_WKSTATUS_H
#define WDG_WKSTATUS_H
#include <QWidget>
#include "ui_wdg_wkstatus.h"

class Status_Widget : public QWidget, public Ui::Status_Widget_Ui {
    Q_OBJECT;
public:
    Status_Widget(QWidget* parent=0);
public slots:
    void setWkVars(int wknr, int runde);
    void setWkNr(QString nr);
    void setStatus1(QString status);
    void setStatus2(QString status);
    void setStatus3(QString status);
    void setStatus4(QString status);
    void setMode(bool tn);
    void setMode2(bool riege);
protected slots:
    void updateData();
    void timerUpdate();
protected:
    int wknr;
    int runde;
    QString swknr;
    bool tn;
    QString status1;
    QString status2;
    QString status3;
    QString status4;
    int i;
};
#endif
