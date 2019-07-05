#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H
#include <QWidget>

namespace Ui {
class StatusWidget;
}

class Event;
class QSortFilterProxyModel;
class StatusTableModel;

class StatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(QWidget *parent = nullptr);
    ~StatusWidget();

public slots:
    void updateStatus();

private:
    Ui::StatusWidget *ui;
    Event *event;
    StatusTableModel *mdl_status1;
    StatusTableModel *mdl_status2;
    QSortFilterProxyModel *mdl_sort_status1;
    QSortFilterProxyModel *mdl_sort_status2;
};

#endif
