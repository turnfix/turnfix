#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H
#include "ui_aboutdialog.h"
#include <QDialog>

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    AboutDialog(QWidget *parent = nullptr)
        : QDialog(parent)
        , ui(new Ui::AboutDialog)
    {
        ui->setupUi(this);
        setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                       | Qt::WindowCloseButtonHint);
    }

    ~AboutDialog() { delete ui; }

private:
    Ui::AboutDialog *ui;
};
#endif
