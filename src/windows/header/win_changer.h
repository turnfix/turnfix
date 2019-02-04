#ifndef WIN_CHANGER_H
#define WIN_CHANGER_H
#include <QMainWindow>
#include "_global.h"
#include "ui_win_changer.h"


class Changer_Window : public QMainWindow, public Ui::Changer_Window_Ui {
    Q_OBJECT;
public:
    Changer_Window(int wnr);
    protected slots:
    void changeWK();
    void init();
    void save1();
    void save2();
    void save3();
    void save4();
    void save5();
    void save6();
protected:
    _global _gfkt;
    int wknr;
    int runde;
    int hwk;
};
#endif
