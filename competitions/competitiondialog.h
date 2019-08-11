#ifndef DLG_WK_H
#define DLG_WK_H
#include <QDialog>

namespace Ui {
class CompetitionDialog;
}

class Competition;
class EntityManager;
class CompetitionDisciplineModel;
class DisciplineOrderModel;

class CompetitionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompetitionDialog(Competition *competition,
                               EntityManager *em,
                               QWidget *parent = nullptr);
    ~CompetitionDialog() override;

private slots:
    void save();
    void enableOptions(int typ);
    void moveUp();
    void moveDown();
    void markGroup();
    void loadDisciplines();

    void orderMoveUp();
    void orderMoveDown();
    void orderMoveLeft();
    void orderMoveRight();

private:
    Competition *m_competition;
    EntityManager *m_em;
    Ui::CompetitionDialog *ui;
    CompetitionDisciplineModel *m_competitionDisciplineModel;
    DisciplineOrderModel *m_orderModel;
};
#endif
