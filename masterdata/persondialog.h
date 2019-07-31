#ifndef PERSONDIALOG_H
#define PERSONDIALOG_H

#include <QDialog>

namespace Ui {
class PersonDialog;
}

class Person;
class EntityManager;

class PersonDialog : public QDialog
{
    Q_OBJECT

public:
    PersonDialog(Person *person, EntityManager *em, QWidget *parent = nullptr);
    ~PersonDialog() override;

    Person *person();

protected slots:
    void save();

protected:
    Ui::PersonDialog *ui;
    EntityManager *m_em;
    Person *m_person;
};
#endif
