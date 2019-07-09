#ifndef COUNTRYDIALOG_H
#define COUNTRYDIALOG_H
#include <QDialog>

namespace Ui {
class CountryDialog;
}

class CountryDialog : public QDialog
{
    Q_OBJECT
public:
    CountryDialog(int lid = 0, QWidget *parent = nullptr);
    ~CountryDialog();

private slots:
    void save();

private:
    Ui::CountryDialog *ui;
    int lid;
};

#endif
