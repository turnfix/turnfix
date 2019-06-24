#ifndef DLG_WK_H
#define DLG_WK_H
#include <QDialog>
#include "ui_dlg_wk.h"

class Event;
class QStandardItemModel;

class Wk_Dialog : public QDialog, public Ui::Wk_Dialog_Ui {
    Q_OBJECT

public:
    Wk_Dialog(Event *event, int edit=0, QWidget* parent = nullptr);

private slots:
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

private:
    Event *event;
    int editid;
    QStandardItemModel *model;
    QStandardItemModel *model2;
};
#endif
