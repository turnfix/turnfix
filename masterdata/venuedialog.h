#ifndef VENUEDIALOG_H
#define VENUEDIALOG_H
#include <QDialog>

namespace Ui {
class VenueDialog;
}

class VenueDialog : public QDialog
{
    Q_OBJECT

public:
    VenueDialog(int tid = 0, QWidget *parent = nullptr);
    ~VenueDialog();

private slots:
    void save();

private:
    Ui::VenueDialog *ui;
    int lid;
};
#endif
