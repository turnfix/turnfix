#ifndef DISCIPLINEDIALOG_H
#define DISCIPLINEDIALOG_H

#include <QDialog>

namespace Ui {
class DisciplineDialog;
}

class QStandardItemModel;

class DisciplineDialog : public QDialog
{
    Q_OBJECT

public:
    DisciplineDialog(int tid = 0, QWidget *parent = nullptr);
    ~DisciplineDialog();

private slots:
    void save();
    void addListItem();
    void delListItem();
    void up();
    void down();

private:
    Ui::DisciplineDialog *ui;
    int disid;
    QStandardItemModel *model;
    virtual void keyPressEvent(QKeyEvent *e);
};

#endif
