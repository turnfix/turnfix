#ifndef TEAMDIALOG_H
#define TEAMDIALOG_H
#include <QDialog>

namespace Ui {
    class TeamDialog;
}

class EntityManager;
class Event;
class QStandardItemModel;
class QSortFilterProxyModel;
class QSqlQueryModel;

class TeamDialog : public QDialog {
    Q_OBJECT

public:
    TeamDialog(Event *m_event, EntityManager *em, int edit = 0, QWidget *parent = nullptr);
    ~TeamDialog();

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
    Event *m_event;
    EntityManager *m_em;
    Ui::TeamDialog *ui;
    QStandardItemModel *model;
    QSqlQueryModel *model2;
    QSortFilterProxyModel *sortmodel;
    QList<int> lst_int_ids;
    int editid;
};
#endif
