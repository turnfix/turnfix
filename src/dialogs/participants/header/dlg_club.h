#ifndef DLG_CLUB_H
#define DLG_CLUB_H

#include <QDialog>

namespace Ui {
    class Club_Dialog;
}

class Event;
class QSqlQueryModel;
class QStandardItemModel;

class Club_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Club_Dialog(Event *event, QWidget *parent = 0);
    ~Club_Dialog();

private slots:
    void fillTable();
    void updateTable();
    void addTn();
    void removeTn();
    void save();

private:
    Event *event;
    Ui::Club_Dialog *ui;
    QStandardItemModel *model;
    QSqlQueryModel *model2;
    QList<int> lst_int_ids;
};

#endif // DLG_CLUB_H
