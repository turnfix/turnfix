#ifndef RESULSWIDGET_H
#define RESULSWIDGET_H
#include <QWidget>

namespace Ui {
class ResultsWidget;
}

class ResultsTableModel;
class Event;
class EntityManager;

class ResultsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultsWidget(QWidget *parent = nullptr);
    ~ResultsWidget();

public slots:
    void fillERTable();
    void updateERList();

private:
    Ui::ResultsWidget *ui;
    ResultsTableModel *er_model;
    Event *m_event;
    EntityManager *m_em;
};
#endif
