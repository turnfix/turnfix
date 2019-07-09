#ifndef MASTERDATADIALOG_H
#define MASTERDATADIALOG_H

#include <QMainWindow>

namespace Ui {
class MasterdataDialog;
}

class QSqlQueryModel;
class QSortFilterProxyModel;

class MasterdataDialog : public QMainWindow
{
    Q_OBJECT

public:
    MasterdataDialog(QWidget *parent = nullptr, int type = 1);
    ~MasterdataDialog();

private slots:
    void loaddb();
    void getData();
    void add();
    void edit();
    void del();
    void updateFilterColumn(int index);
    void updateFilterText(QString text);

private:
    Ui::MasterdataDialog *ui;
    int currtype;
    QSqlQueryModel *db_model;
    QSortFilterProxyModel *db_sort_model;
};
#endif
