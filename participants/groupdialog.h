#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include <QDialog>

namespace Ui {
    class GroupDialog;
}

class Event;
class QStandardItemModel;
class QSqlQueryModel;

class GroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupDialog(Event *event, int edit=0, QWidget* parent = nullptr);

private slots:
    void initData();
    void save();
    void addTn();
    void addAv();
    void editAv();
    void removeTn();
    void fillTable();
    void fillTable2();
    void checkDisziplinen();
    void saveWindowState();
    void addClub();
    void updateClubs();

private:
    Event *event;
    Ui::GroupDialog *ui;
    QStandardItemModel *model;
    QSqlQueryModel *model2;
    QList<int> lst_int_ids;
    int editid;
};

#endif