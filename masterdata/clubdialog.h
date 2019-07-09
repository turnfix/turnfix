#ifndef CLUBDIALOG_H
#define CLUBDIALOG_H
#include <QDialog>

namespace Ui {
class ClubDialog;
}

class ClubDialog : public QDialog
{
    Q_OBJECT

public:
    ClubDialog(int tid = 0, QWidget *parent = nullptr);
    ~ClubDialog();

private slots:
    void save();
    void updateSlider();
    void updateOrt();
    void addPerson();
    void readPersons();
    void addGau();
    void readGaue();
    void updateGauInfo();

private:
    Ui::ClubDialog *ui;
    int vnid;
};
#endif
