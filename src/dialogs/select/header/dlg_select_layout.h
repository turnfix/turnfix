#ifndef DLG_SELECT_LAYOUT_H
#define DLG_SELECT_LAYOUT_H

#include <QDialog>
#include "ui_dlg_select_layout.h"

class Select_Layout : public QDialog, public Ui::Select_Layout_Ui {
    Q_OBJECT

public:
    Select_Layout(QWidget *parent = 0);
    int getLayoutID();

private slots:
    void layoutSelectionChange();
    void closeDialog();
};

#endif // DLG_SELECT_LAYOUT_H
