#ifndef DLG_SELECT_TN_H
#define DLG_SELECT_TN_H
#include <QDialog>
#include "ui_dlg_select_tn.h"

class QErgebnisTableModel;

class Select_Tn_Dialog : public QDialog, public Ui::Select_Tn_Dialog_Ui {
    Q_OBJECT;
public:
    Select_Tn_Dialog(QWidget* parent=0);
public slots:
    QList<int> getTnList();
    QString getTnWk();
protected slots:
    void initData();
    void updateList();
    void submit();
protected:
    QList<int> tnlist;
    QString tnwk;
    QErgebnisTableModel *er_model;
};
#endif
