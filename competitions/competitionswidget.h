#ifndef COMPETITIONSWIDGET_H
#define COMPETITIONSWIDGET_H
#include <QWidget>

namespace Ui {
class CompetitionsWidget;
}

class Event;
class EntityManager;
class CompetitionModel;
class QSortFilterProxyModel;

class CompetitionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompetitionsWidget(QWidget *parent = nullptr);
    ~CompetitionsWidget() override;

private slots:
    void addCompetition();
    void editCompetition();
    void removeCompetition();
    void updateFilterColumn(int index);
    void updateFilterText(const QString &text);

public:
    void setup(Event *event, EntityManager *em);

private:
    Event *m_event;
    EntityManager *m_em;
    Ui::CompetitionsWidget *ui;
    CompetitionModel *m_model;
    QSortFilterProxyModel *m_sortModel;
};
#endif
