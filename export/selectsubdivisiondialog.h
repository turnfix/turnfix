#ifndef SELECTSUBDIVISIONDIALOG_H
#define SELECTSUBDIVISIONDIALOG_H
#include <QDialog>

namespace Ui {
class SelectSubdivisionDialog;
}

class Event;

class SelectSubdivisionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectSubdivisionDialog(Event *event, QWidget *parent = nullptr);
    ~SelectSubdivisionDialog();

public slots:
    QStringList getRg();

private slots:
    void select1();

private:
    void initData();

    Ui::SelectSubdivisionDialog *ui;
    Event *event;
    QStringList rg;
};
#endif
