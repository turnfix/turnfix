#ifndef BARCODEDIALOG_H
#define BARCODEDIALOG_H
#include <QDialog>

namespace Ui {
class BarcodeDialog;
}

class BarcodeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BarcodeDialog(QWidget *parent = nullptr);
    ~BarcodeDialog();

public slots:
    QString getInput();

private:
    Ui::BarcodeDialog *ui;
    QString str;
    bool eventFilter(QObject *obj, QEvent *ev);
};
#endif
