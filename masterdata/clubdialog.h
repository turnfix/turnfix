#ifndef CLUBDIALOG_H
#define CLUBDIALOG_H
#include <QDialog>

namespace Ui {
class ClubDialog;
}

class Club;
class EntityManager;
class PersonModel;

class ClubDialog : public QDialog
{
    Q_OBJECT

public:
    ClubDialog(Club *club, EntityManager *em, QWidget *parent = nullptr);
    ~ClubDialog();

    Club *club();

private slots:
    void save();
    void updateSlider();
    void updateOrt();
    void addPerson();
    void addGau();
    void readGaue();
    void updateGauInfo();

private:
    Ui::ClubDialog *ui;
    EntityManager *m_em;
    Club *m_club;
    PersonModel *m_personModel;
};
#endif
