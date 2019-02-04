#ifndef WIN_DISPLAY_H
#define WIN_DISPLAY_H
#include <QDialog>
#include "ui_win_display.h"
#include "../../models/header/mdl_result.h"

class QTimer;

class Display_Window : public QDialog, public Ui::Display_Window_Ui {
    Q_OBJECT;
public:
    Display_Window();
    QErgebnisTableModel *er_model;
public slots:
    void updateLast(QString name, QString points);
    void updateCurrent(QString name, QString vorname, QString verein, QString snr);
    void updateNext(QString name);
    void updateDis(QString icon, QString name);
    void updateAmpel(int mode);
    void startClock();
    void stopClock();
    void resetClock();
    void updateClock();
    void hideClock();
    void resize();
private:
    QTimer *time;
    int secs;
signals:
    void clock(const QString&);
};
#endif
