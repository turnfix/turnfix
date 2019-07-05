#ifndef SELECTCOMPETITIONDIALOG_H
#define SELECTCOMPETITIONDIALOG_H
#include <QDialog>

namespace Ui {
class SelectCompetitionDialog;
}

class Event;

class SelectCompetitionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectCompetitionDialog(Event *Event, QWidget *parent = nullptr);
    ~SelectCompetitionDialog();

public slots:
    QString getWk();

private slots:
    void initData();
    void select1();

private:
    Ui::SelectCompetitionDialog *ui;
    Event *event;
    QString wk;
};
#endif
