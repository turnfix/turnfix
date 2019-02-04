#include <QSqlQuery>
#include "header/dlg_select_layout.h"

Select_Layout::Select_Layout(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    connect(cmb_layout,SIGNAL(currentIndexChanged(int)),this,SLOT(layoutSelectionChange()));
    connect(bbx_done,SIGNAL(accepted()),this,SLOT(closeDialog()));
    connect(bbx_done,SIGNAL(rejected()),this,SLOT(close()));
    QSqlQuery layoutListQuery;
    layoutListQuery.prepare("SELECT int_layoutid, var_name FROM tfx_layouts ORDER BY var_name");
    layoutListQuery.exec();
    while (layoutListQuery.next()) {
        cmb_layout->addItem(layoutListQuery.value(1).toString(),layoutListQuery.value(0).toInt());
    }
}

void Select_Layout::layoutSelectionChange() {
    QSqlQuery getCommentQuery;
    getCommentQuery.prepare("SELECT txt_comment FROM tfx_layouts WHERE int_layoutid=?");
    getCommentQuery.bindValue(0,cmb_layout->itemData(cmb_layout->currentIndex()));
    getCommentQuery.exec();
    getCommentQuery.next();
    txt_comment->setText(getCommentQuery.value(0).toString());
}

int Select_Layout::getLayoutID() {
    return cmb_layout->itemData(cmb_layout->currentIndex()).toInt();
}

void Select_Layout::closeDialog() {
    done(1);
}
