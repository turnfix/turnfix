#ifndef DLG_TEAM_H
#define DLG_TEAM_H
#include <QDialog>
#include "ui_dlg_team.h"

class Event;
class QStandardItemModel;
class QSortFilterProxyModel;
class QSqlQueryModel;

class Team_Dialog : public QDialog, public Ui::Team_Dialog_Ui {
    Q_OBJECT

public:
    Team_Dialog(Event *event, int edit=0, QWidget* parent = nullptr);

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
