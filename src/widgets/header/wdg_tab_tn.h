#ifndef WDG_TAB_TN_H
#define WDG_TAB_TN_H
#include <QWidget>
#include "ui_wdg_tab_tn.h"

class Event;
class QSqlQueryModel;
class QSortFilterProxyModel;

class Tab_TN : public QWidget, public Ui::Tab_TN_Ui {
    Q_OBJECT

public:
    Tab_TN(QWidget* parent = nullptr);

public slots:
    void loadBestView();
    void refresh();

private slots:
    void fillTNTable();
    void fillTNTable2();
    void fillTNTable3();
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
    QSqlQueryModel *tn_model;
    QSqlQueryModel *tn_model2;
    QSqlQueryModel *tn_model3;
    QSortFilterProxyModel *tn_sort_model;
    QSortFilterProxyModel *tn_sort_model2;
    QSortFilterProxyModel *tn_sort_model3;
};
#endif
