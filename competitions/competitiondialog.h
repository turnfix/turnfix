#ifndef DLG_WK_H
#define DLG_WK_H
#include <QDialog>

namespace Ui {
class CompetitionDialog;
}

class Competition;
class EntityManager;
class CompetitionDisciplineModel;
class QStandardItemModel;

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
    void fillTable();
    void fillTable2();
    void enableOptions(int typ);
    void moveUp();
    void moveDown();
    void markGroup();

    void orderMoveUp();
    void orderMoveDown();
    void orderMoveLeft();
    void orderMoveRight();

private:
    Competition *m_competition;
    EntityManager *m_em;
    Ui::CompetitionDialog *ui;
    CompetitionDisciplineModel *m_competitionDisciplineModel;
    QStandardItemModel *model2;
};
#endif
