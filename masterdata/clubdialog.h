#ifndef CLUBDIALOG_H
#define CLUBDIALOG_H
#include <QDialog>

namespace Ui {
class ClubDialog;
}

class Club;
class EntityManager;
class PersonModel;
class RegionModel;

class ClubDialog : public QDialog
{
    Q_OBJECT

public:
    ClubDialog(Club *club, EntityManager *em, QWidget *parent = nullptr);
    ~ClubDialog() override;

    Club *club();

private slots:
    void save();
    void updateSlider();
    void updateCity();
    void addPerson();
    void addRegion();
    void updateStateAndCountry();

private:
    Ui::ClubDialog *ui;
    EntityManager *m_em;
    Club *m_club;
    PersonModel *m_personModel;
    RegionModel *m_regionModel;
};
#endif
