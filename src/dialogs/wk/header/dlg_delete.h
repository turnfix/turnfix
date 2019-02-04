#ifndef DLG_DELETE_H
#define DLG_DELETE_H

#include <QDialog>

namespace Ui {
    class Delete_Dialog;
}

class Delete_Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Delete_Dialog(QWidget *parent = 0);
    ~Delete_Dialog();

private slots:
    void closeAndDelete();

private:
    Ui::Delete_Dialog *ui;
};

#endif // DLG_DELETE_H
