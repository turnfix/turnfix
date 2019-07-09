#ifndef DISCIPLINEGROUPDIALOG_H
#define DISCIPLINEGROUPDIALOG_H

#include <QDialog>

namespace Ui {
class DisciplineGroupDialog;
}

class QStandardItemModel;

class DisciplineGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DisciplineGroupDialog(int dgid = 0, QWidget *parent = nullptr);
    ~DisciplineGroupDialog();

private slots:
    void addItem();
    void removeItem();
    void moveUp();
    void moveDown();
    void save();

private:
    Ui::DisciplineGroupDialog *ui;
    int dgid;
    QStandardItemModel *model_all;
    QStandardItemModel *model_group;
};

#endif
