#ifndef WDG_TAB_PE_H
#define WDG_TAB_PE_H
#include <QWidget>
#include "ui_wdg_tab_pe.h"

class QSqlQueryModel;
class QStandardItemModel;

class Tab_PE : public QWidget, public Ui::Tab_PE_Ui {
    Q_OBJECT;
public:
    Tab_PE(QWidget* parent=0);
public slots:
    void squadChange(QString squad="");
    void init();
private slots:
    void startBogen();
    void startKarte();
    void startBarcode();
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
};
#endif
