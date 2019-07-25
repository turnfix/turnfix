#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class Event;
class EntityManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(EntityManager *em, Event *m_event);
    ~MainWindow();

private slots:
    void updateTables(int init=0);
    void showAbout();
    void showEventDialog();
    void newNumbers();
    void showTNDB();
    void showVNDB();
    void showDisDB();
    void editPass();
    void changeWK();
    void sendMLists();

private:
    void initEvent();

    Ui::MainWindow *ui;
    Event *m_event;
    EntityManager *m_em;
};

#endif
