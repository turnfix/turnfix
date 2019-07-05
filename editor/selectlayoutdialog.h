#ifndef SELECTLAYOUTDIALOG_H
#define SELECTLAYOUTDIALOG_H

#include <QDialog>

namespace Ui {
class SelectLayoutDialog;
}

class SelectLayoutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectLayoutDialog(QWidget *parent = nullptr);
    ~SelectLayoutDialog();

    int getLayoutID();

private slots:
    void layoutSelectionChange();
    void closeDialog();

private:
    Ui::SelectLayoutDialog *ui;
};

#endif // SELECTLAYOUTDIALOG_H
