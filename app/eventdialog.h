#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QDialog>

namespace Ui {
class EventDialog;
}

class Event;
class EntityManager;

class EventDialog : public QDialog
{
    Q_OBJECT

public:
    EventDialog(EntityManager *em, Event *event, QWidget *parent = nullptr);
    ~EventDialog();

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
    Event *m_event;
    EntityManager *m_em;
};
#endif
