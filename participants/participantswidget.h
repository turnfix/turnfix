#ifndef PARTICIPANTSWIDGET_H
#define PARTICIPANTSWIDGET_H
#include <QWidget>
#include "ui_participantswidget.h"

class Event;
class ParticipantsTableModel;
class QSortFilterProxyModel;

class ParticipantsWidget : public QWidget, public Ui::ParticipantsWidgetUi {
    Q_OBJECT

public:
    ParticipantsWidget(QWidget* parent = nullptr);

public slots:
    void loadBestView();
    void refresh();

private slots:
    void fillTable();
    void addTN();
    void addCL();
    void editTN();
    void delTN();
    void meldeTN();
    void updateMelde();
    void syncTN();
    void updateTNFilterColumn(int index);
    void updateTNFilterText(QString text);

private:
    Event *event;
    ParticipantsTableModel *participantsModel;
    QSortFilterProxyModel *sortModel;
};
#endif
