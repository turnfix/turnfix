#ifndef PARTICIPANTSWIDGET_H
#define PARTICIPANTSWIDGET_H
#include <QWidget>

namespace Ui {
    class ParticipantsWidget;
}

class Event;
class ParticipantsTableModel;
class QSortFilterProxyModel;

class ParticipantsWidget : public QWidget {
    Q_OBJECT

public:
    explicit ParticipantsWidget(QWidget* parent = nullptr);
    ~ParticipantsWidget();

    void refresh();

public slots:
    void loadBestView();

private slots:
    void addTN();
    void addCL();
    void editTN();
    void delTN();
    void meldeTN();
    void updateMelde();
    void syncTN();
    void updateTNFilterColumn(int index);
    void updateTNFilterText(QString text);
    void viewChanged(int);

private:
    Event *event;
    Ui::ParticipantsWidget *ui;
    ParticipantsTableModel *participantsModel;
    QSortFilterProxyModel *sortModel;
};
#endif
