#ifndef ATHLETEDIALOG_H
#define ATHLETEDIALOG_H
#include <QDialog>

namespace Ui {
class AthleteDialog;
}

class Athlete;
class Club;
class EntityManager;
class ClubModel;

class AthleteDialog : public QDialog
{
    Q_OBJECT

public:
    AthleteDialog(Athlete *athlete, EntityManager *em, QWidget *parent = nullptr);
    AthleteDialog(Athlete *athlete, Club *club, EntityManager *em, QWidget *parent = nullptr);
    ~AthleteDialog() override;

private slots:
    void save();
    void changeDat();

private:
    Ui::AthleteDialog *ui;
    EntityManager *m_em;
    Athlete *m_athlete;
    ClubModel *m_clubModel;
};
#endif
