#ifndef SAVELAYOUTDIALOG_H
#define SAVELAYOUTDIALOG_H

#include <QDialog>

namespace Ui {
class SaveLayoutDialog;
}

class EditorGraphicsItem;

class SaveLayoutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveLayoutDialog(QWidget *parent = 0);
    ~SaveLayoutDialog();
    void setItems(QList<EditorGraphicsItem *>);
    QList<EditorGraphicsItem *> getItems();

private slots:
    void closeDialog();
    void layoutSelectionChange();

private:
    Ui::SaveLayoutDialog *ui;
    QList<EditorGraphicsItem *> items;
};

#endif // SAVELAYOUTDIALOG_H
