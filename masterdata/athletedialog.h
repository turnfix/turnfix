#ifndef ATHLETEDIALOG_H
#define ATHLETEDIALOG_H
#include <QDialog>

namespace Ui {
class AthleteDialog;
}

class AthleteDialog : public QDialog
{
    Q_OBJECT

public:
    AthleteDialog(int tid = 0, QWidget *parent = nullptr);
    ~AthleteDialog();

    void setVerein(QString verein);

private slots:
    void save();
    void changeDat();

private:
    Ui::AthleteDialog *ui;
    int tnid;
};
#endif
