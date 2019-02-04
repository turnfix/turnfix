#ifndef DLG_DB_DISGROUP_H
#define DLG_DB_DISGROUP_H

#include <QDialog>

namespace Ui {
    class Db_Disgroup_Dialog;
}

class QStandardItemModel;

class Db_Disgroup_Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Db_Disgroup_Dialog(int dgid=0, QWidget *parent = 0);
    ~Db_Disgroup_Dialog();

private slots:
    void addItem();
    void removeItem();
    void moveUp();
    void moveDown();
    void save();

private:
    Ui::Db_Disgroup_Dialog *ui;
    int dgid;
    QStandardItemModel *model_all;
    QStandardItemModel *model_group;

};

#endif // DLG_DB_DISGROUP_H
