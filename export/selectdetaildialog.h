#ifndef SELECTDETAILDIALOG_H
#define SELECTDETAILDIALOG_H

#include <QDialog>

class Event;

namespace Ui {
class SelectDetailDialog;
}

class SelectDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDetailDialog(Event *Event, int type, QWidget *parent = nullptr);
    ~SelectDetailDialog();
    int getId();

private slots:
    void closeDialog();

private:
    Ui::SelectDetailDialog *ui;
};

#endif // DLG_SELECT_DETAIL_H
