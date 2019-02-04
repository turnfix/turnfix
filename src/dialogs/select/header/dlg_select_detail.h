#ifndef DLG_SELECT_DETAIL_H
#define DLG_SELECT_DETAIL_H

#include <QDialog>

namespace Ui {
    class Select_Detail_Dialog;
}

class Select_Detail_Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Select_Detail_Dialog(int type, QWidget *parent = 0);
    ~Select_Detail_Dialog();
    int getId();

private slots:
    void closeDialog();


private:
    Ui::Select_Detail_Dialog *ui;
};

#endif // DLG_SELECT_DETAIL_H
