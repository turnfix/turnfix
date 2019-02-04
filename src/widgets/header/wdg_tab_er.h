#ifndef WDG_TAB_ER_H
#define WDG_TAB_ER_H
#include <QWidget>
#include "ui_wdg_tab_er.h"

class QErgebnisTableModel;

class Tab_ER : public QWidget, public Ui::Tab_ER_Ui {
    Q_OBJECT;
public:
    Tab_ER(QWidget* parent=0);
public slots:
    void fillERTable();
    void updateERList();
protected:
    QErgebnisTableModel *er_model;
};
#endif
