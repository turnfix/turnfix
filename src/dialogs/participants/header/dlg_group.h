#ifndef DLG_GROUP_H
#define DLG_GROUP_H
#include <QDialog>
#include "ui_dlg_group.h"

class QStandardItemModel;
class QSqlQueryModel;

class Group_Dialog : public QDialog, public Ui::Group_Dialog_Ui {
    Q_OBJECT;
public:
    Group_Dialog(int edit=0, QWidget* parent=0);
protected slots:
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
protected:
    QStandardItemModel *model;
    QSqlQueryModel *model2;
    QList<int> lst_int_ids;
    int editid;
};
#endif
