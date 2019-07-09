#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QDialog>

namespace Ui {
class EventDialog;
}

class Event;

class EventDialog : public QDialog
{
    Q_OBJECT

public:
    EventDialog(Event *event, int editid = 0, QWidget *parent = nullptr);
    ~EventDialog();

public slots:
    int getWknr();

private slots:
    void initData();
    void save();
    void addPerson();
    void addLocation();
    void addAccount();
    void readPersons();
    void readLocations();
    void readAccounts();

private:
    Ui::EventDialog *ui;
    Event *event;
    int wknr;
    int editid;
};
#endif
