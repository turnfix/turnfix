#ifndef CLUBDIALOG_H
#define CLUBDIALOG_H

#include <QDialog>

namespace Ui {
    class ClubDialog;
}

class Event;
class QSqlQueryModel;
class QStandardItemModel;

class ClubDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClubDialog(Event *event, QWidget *parent = 0);
    ~ClubDialog();

private slots:
    void fillTable();
    void updateTable();
    void addTn();
    void removeTn();
    void save();

private:
    Event *event;
    Ui::ClubDialog *ui;
    QStandardItemModel *model;
    QSqlQueryModel *model2;
    QList<int> lst_int_ids;
};

#endif // CLUBDIALOG_H
