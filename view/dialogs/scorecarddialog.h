#ifndef SCORECARDDIALOG_H
#define SCORECARDDIALOG_H
#include <QDialog>
#include "ui_scorecarddialog.h"

class QKartenTableModel;
class Event;

class ScoreCardDialog : public QDialog, public Ui::ScoreCardDialogUi {
    Q_OBJECT

public:
    ScoreCardDialog(Event *event, QWidget* parent = nullptr);
    void init(int stnr, int wertid, QList<QList<int>> disids);
    QKartenTableModel *pe_model;

private slots:
    void fillPETable();
    void finishEdit();
    void statusChange1();
    void saveClose();

private:
    Event *event;
    QList<QList<int>> disids;
    int stnr;
    int wertid;
};

#endif
