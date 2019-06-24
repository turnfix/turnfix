#ifndef WDG_TAB_WK_H
#define WDG_TAB_WK_H
#include <QWidget>
#include "ui_wdg_tab_wk.h"

class Event;
class QSqlQueryModel;
class QSortFilterProxyModel;

class Tab_WK : public QWidget, public Ui::Tab_WK_Ui {
    Q_OBJECT

public:
    Tab_WK(QWidget* parent = nullptr);

private slots:
    void addWK();
    void editWK();
    void delWK();
    void updateWKFilterColumn(int index);
    void updateWKFilterText(QString text);

public:
    void fillWKTable();

private:
    Event *event;
    QSqlQueryModel *wk_model;
    QSortFilterProxyModel *wk_sort_model;
};
#endif
