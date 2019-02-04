#ifndef DLG_DB_FORMEL_H
#define DLG_DB_FORMEL_H

#include <QDialog>

namespace Ui {
    class Formel_Dialog;
}

class Formel_Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Formel_Dialog(int fid=0, QWidget *parent = 0);
    ~Formel_Dialog();

private slots:
    void save();
    void addPart();
    void checkFormula();


private:
    Ui::Formel_Dialog *ui;
    int formelid;
};

#endif // DLG_DB_FORMEL_H
