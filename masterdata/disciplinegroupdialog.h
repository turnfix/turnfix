#ifndef DISCIPLINEGROUPDIALOG_H
#define DISCIPLINEGROUPDIALOG_H

#include <QDialog>

namespace Ui {
class DisciplineGroupDialog;
}

class DisciplineGroup;
class DisciplineModel;
class DisciplineGroupItemModel;
class EntityManager;

class DisciplineGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DisciplineGroupDialog(DisciplineGroup *disciplineGroup,
                                   EntityManager *em,
                                   QWidget *parent = nullptr);
    ~DisciplineGroupDialog() override;

private slots:
    void addItem();
    void removeItem();
    void moveUp();
    void moveDown();
    void save();

private:
    Ui::DisciplineGroupDialog *ui;
    DisciplineGroup *m_disciplineGroup;
    EntityManager *m_em;
    DisciplineModel *m_disciplineModel;
    DisciplineGroupItemModel *m_itemModel;
};

#endif
