#ifndef MULTIPARTICIPANTSDIALOG_H
#define MULTIPARTICIPANTSDIALOG_H

#include <QDialog>

namespace Ui {
class MultiParticipantsDialog;
}

class Event;
class QSqlQueryModel;
class QStandardItemModel;

class MultiParticipantsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultiParticipantsDialog(Event *event, QWidget *parent = nullptr);
    ~MultiParticipantsDialog();

private slots:
    void fillTable();
    void updateTable();
    void addTn();
    void removeTn();
    void save();

private:
    Event *event;
    Ui::MultiParticipantsDialog *ui;
    QStandardItemModel *model;
    QSqlQueryModel *model2;
    QList<int> lst_int_ids;
};

#endif // MULTIPARTICIPANTSDIALOG_H
