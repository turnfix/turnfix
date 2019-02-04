#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QCloseEvent>
#include "ui_mainwindow.h"
#include "../../global/header/_global.h"

class MainWindow : public QMainWindow, public Ui::MainWindow {
    Q_OBJECT

public:
    MainWindow();

protected slots:
    void updateTables(int init=0);
    void showAbout();
    void showWKConf();
    void newNumbers();
    void showTNDB();
    void showVNDB();
    void showDisDB();
    void editPass();
    void changeWK();
    void initTabs();
    void uploadResults();
    void sendMLists();
protected:
    virtual void closeEvent(QCloseEvent * e);
};

#endif
