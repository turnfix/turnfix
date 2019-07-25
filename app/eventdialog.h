#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QDialog>

namespace Ui {
class EventDialog;
}

class Event;
class EntityManager;
class BankAccountModel;
class PersonModel;
class VenueModel;

class EventDialog : public QDialog
{
    Q_OBJECT

public:
    EventDialog(Event *m_event, EntityManager *em, QWidget *parent = nullptr);
    ~EventDialog();

private slots:
    void save();
    void addEventContact();
    void addRegistrationContact();
    void addLocation();
    void addAccount();

private:
    Ui::EventDialog *ui;
    Event *m_event;
    EntityManager *m_em;
    PersonModel *m_personModel;
    VenueModel *m_venueModel;
    BankAccountModel *m_accountModel;
};
#endif
