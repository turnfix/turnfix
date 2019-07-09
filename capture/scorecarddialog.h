#ifndef SCORECARDDIALOG_H
#define SCORECARDDIALOG_H

#include <QDialog>
#include "ui_scorecarddialog.h"

namespace Ui {
class ScoreCardDialog;
}

class ScoreCardTableModel;
class Event;

class ScoreCardDialog : public QDialog
{
    Q_OBJECT

public:
    ScoreCardDialog(Event *event, QWidget* parent = nullptr);
    ~ScoreCardDialog();

    void init(int stnr, int wertid, QList<QList<int>> disids);
    ScoreCardTableModel *pe_model;

private slots:
    void fillPETable();
    void finishEdit();
    void statusChange1();
    void saveClose();

private:
    Ui::ScoreCardDialog *ui;
    Event *event;
    QList<QList<int>> disids;
    int stnr;
    int wertid;
};

#endif
