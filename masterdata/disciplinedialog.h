#ifndef DISCIPLINEDIALOG_H
#define DISCIPLINEDIALOG_H

#include <QDialog>

namespace Ui {
class DisciplineDialog;
}

class Discipline;
class EntityManager;
class DisciplineFieldModel;

class DisciplineDialog : public QDialog
{
    Q_OBJECT

public:
    DisciplineDialog(Discipline *discipline, EntityManager *em, QWidget *parent = nullptr);
    ~DisciplineDialog() override;

private slots:
    void save();
    void addListItem();
    void delListItem();
    void up();
    void down();

private:
    Ui::DisciplineDialog *ui;
    EntityManager *m_em;
    Discipline *m_discipline;
    DisciplineFieldModel *m_fieldModel;
    void keyPressEvent(QKeyEvent *e) override;
};

#endif
