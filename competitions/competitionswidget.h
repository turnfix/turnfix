#ifndef COMPETITIONSWIDGET_H
#define COMPETITIONSWIDGET_H
#include <QWidget>

namespace Ui {
class CompetitionsWidget;
}

class Event;
class QSqlQueryModel;
class QSortFilterProxyModel;

class CompetitionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompetitionsWidget(QWidget *parent = nullptr);
    ~CompetitionsWidget() override;

private slots:
    void addWK();
    void editWK();
    void delWK();
    void updateWKFilterColumn(int index);
    void updateWKFilterText(const QString &text);

public:
    void fillWKTable();

private:
    Event *event;
    Ui::CompetitionsWidget *ui;
    QSqlQueryModel *wk_model;
    QSortFilterProxyModel *wk_sort_model;
};
#endif
