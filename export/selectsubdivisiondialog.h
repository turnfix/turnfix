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
    explicit SelectSubdivisionDialog(Event *m_event, QWidget *parent = nullptr);
    ~SelectSubdivisionDialog();

public slots:
    QStringList getRg();

private slots:
    void select1();

private:
    void initData();

    Ui::SelectSubdivisionDialog *ui;
    Event *m_event;
    QStringList rg;
};
#endif
