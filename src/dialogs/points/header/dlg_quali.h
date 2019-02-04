#ifndef DLG_QUALI_H
#define DLG_QUALI_H
#include <QDialog>
#include "ui_dlg_quali.h"

class QQualiTableModel;

class Quali_Dialog : public QDialog, public Ui::Quali_Dialog_Ui {
    Q_OBJECT;
public:
    Quali_Dialog(int edit=0, QWidget* parent=0);
protected slots:
    void initData();
    void finishEdit();
protected:
    int editid;
    QQualiTableModel *model;
};

#endif
