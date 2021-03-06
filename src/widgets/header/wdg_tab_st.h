#ifndef WDG_TAB_ST_H
#define WDG_TAB_ST_H
#include <QWidget>
#include "ui_wdg_tab_st.h"

class QSortFilterProxyModel;
class QStatusTableModel;

class Tab_ST : public QWidget, public Ui::Tab_ST_Ui {
    Q_OBJECT;
public:
    Tab_ST(QWidget* parent=0);
public slots:
    void updateStatus();
protected:
    QStatusTableModel *mdl_status1;
    QStatusTableModel *mdl_status2;
    QSortFilterProxyModel *mdl_sort_status1;
    QSortFilterProxyModel *mdl_sort_status2;
};

#endif
