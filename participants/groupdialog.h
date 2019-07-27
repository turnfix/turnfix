#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include <QDialog>

namespace Ui {
    class GroupDialog;
}

class Event;
class EntityManager;
class QStandardItemModel;
class QSqlQueryModel;

class GroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupDialog(Event *m_event, EntityManager *em, int edit = 0, QWidget *parent = nullptr);
    ~GroupDialog();

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
    Event *m_event;
    EntityManager *m_em;
    Ui::GroupDialog *ui;
    QStandardItemModel *model;
    QSqlQueryModel *model2;
    QList<int> lst_int_ids;
    int editid;
};

#endif
