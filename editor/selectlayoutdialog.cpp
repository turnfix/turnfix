#include "selectlayoutdialog.h"
#include "ui_selectlayoutdialog.h"
#include <QSqlQuery>

SelectLayoutDialog::SelectLayoutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectLayoutDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    connect(ui->cmb_layout, SIGNAL(currentIndexChanged(int)), this, SLOT(layoutSelectionChange()));
    connect(ui->bbx_done, SIGNAL(accepted()), this, SLOT(closeDialog()));
    connect(ui->bbx_done, SIGNAL(rejected()), this, SLOT(close()));

    QSqlQuery layoutListQuery;
    layoutListQuery.prepare("SELECT int_layoutid, var_name FROM tfx_layouts ORDER BY var_name");
    layoutListQuery.exec();
    while (layoutListQuery.next()) {
        ui->cmb_layout->addItem(layoutListQuery.value(1).toString(),
                                layoutListQuery.value(0).toInt());
    }
}

SelectLayoutDialog::~SelectLayoutDialog()
{
    delete ui;
}

void SelectLayoutDialog::layoutSelectionChange()
{
    QSqlQuery getCommentQuery;
    getCommentQuery.prepare("SELECT txt_comment FROM tfx_layouts WHERE int_layoutid=?");
    getCommentQuery.bindValue(0, ui->cmb_layout->itemData(ui->cmb_layout->currentIndex()));
    getCommentQuery.exec();
    getCommentQuery.next();
    ui->txt_comment->setText(getCommentQuery.value(0).toString());
}

int SelectLayoutDialog::getLayoutID()
{
    return ui->cmb_layout->itemData(ui->cmb_layout->currentIndex()).toInt();
}

void SelectLayoutDialog::closeDialog()
{
    done(1);
}
