#ifndef TEAMDIALOG_H
#define TEAMDIALOG_H
#include <QDialog>
#include "ui_teamdialog.h"

class Event;
class QStandardItemModel;
class QSortFilterProxyModel;
class QSqlQueryModel;

class TeamDialog : public QDialog, public Ui::TeamDialogUi {
    Q_OBJECT

public:
    TeamDialog(Event *event, int edit=0, QWidget* parent = nullptr);

private slots:
    void initData();
    void save();
    void addTn();
    void addAv();
    void editAv();
    void removeTn();
    void fillTable();
    void fillTable2();
    void saveWindowState();
    void addClub();
    void updateClubs();
    bool checkJg(int jg);

private:
    Event *event;
    QStandardItemModel *model;
    QSqlQueryModel *model2;
    QSortFilterProxyModel *sortmodel;
    QList<int> lst_int_ids;
    int editid;
};
#endif
