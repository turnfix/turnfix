#ifndef DLG_SAVE_LAYOUT_H
#define DLG_SAVE_LAYOUT_H

#include <QDialog>

namespace Ui {
    class Save_Layout;
}

class EditorTextItem;

class Save_Layout : public QDialog {
    Q_OBJECT

public:
    explicit Save_Layout(QWidget *parent = 0);
    ~Save_Layout();
    void setItems(QList<EditorTextItem*>);
    QList<EditorTextItem*> getItems();

private slots:
    void closeDialog();
    void layoutSelectionChange();

private:
    Ui::Save_Layout *ui;
    QList<EditorTextItem*> items;
};

#endif // DLG_SAVE_LAYOUT_H
