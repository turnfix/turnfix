#ifndef WDG_TAB_RG_H
#define WDG_TAB_RG_H
#include <QWidget>
#include <QStandardItem>
#include "ui_wdg_tab_rg.h"

class QEinteilTableModel;
class Riegen_Widget;
class QStandardItemModel;
class QItemSelection;

class Tab_RG : public QWidget, public Ui::Tab_RG_Ui {
    Q_OBJECT;
public:
    Tab_RG(QWidget* parent=0);
public slots:
    void fillRETable2();
private slots:
    void sendData();
    void getData();
    void updateRiege();
    void addRiege();
    void fetchRgData();
    void setRiegenData();
    void removeRiege();
protected:
    QEinteilTableModel *re_model;
    QEinteilTableModel *re_model2;
    QStandardItemModel *rg_model;
};

#endif
