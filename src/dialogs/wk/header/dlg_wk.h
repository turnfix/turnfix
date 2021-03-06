#ifndef DLG_WK_H
#define DLG_WK_H
#include <QDialog>
#include "ui_dlg_wk.h"

class QStandardItemModel;

class Wk_Dialog : public QDialog, public Ui::Wk_Dialog_Ui {
    Q_OBJECT;
public:
    Wk_Dialog(int edit=0, QWidget* parent=0);
protected slots:
    void initData();
    void save();
    void fillTable();
    void fillTable2();
    void enableOptions(int typ);
    void moveUp();
    void moveDown();
    void markGroup();

    void orderMoveUp();
    void orderMoveDown();
    void orderMoveLeft();
    void orderMoveRight();
protected:
    int editid;
    QStandardItemModel *model;
    QStandardItemModel *model2;
};
#endif
