#ifndef DLG_SELECT_TN_H
#define DLG_SELECT_TN_H
#include <QDialog>
#include "ui_dlg_select_tn.h"

class Event;
class QErgebnisTableModel;

class Select_Tn_Dialog : public QDialog, public Ui::Select_Tn_Dialog_Ui {
    Q_OBJECT

public:
    Select_Tn_Dialog(Event *event, QWidget* parent = nullptr);

public slots:
    QList<int> getTnList();
    QString getTnWk();

private slots:
    void initData();
    void updateList();
    void submit();

private:
    Event *event;
    QList<int> tnlist;
    QString tnwk;
    QErgebnisTableModel *er_model;
};
#endif
