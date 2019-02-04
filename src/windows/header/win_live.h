#ifndef WIN_LIVE_H
#define WIN_LIVE_H

#include <QMainWindow>
#include "ui_win_live.h"

class LiveWindow : public QMainWindow, public Ui::LiveWindow_Ui {
    Q_OBJECT;
public:
    LiveWindow(int wnr);
protected:
    int wknr;
};

#endif // WIN_LIVE_H
