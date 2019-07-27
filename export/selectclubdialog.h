#ifndef SELECTCLUBDIALOG_H
#define SELECTCLUBDIALOG_H
#include <QDialog>
#include "ui_selectclubdialog.h"

class Event;

class SelectClubDialog : public QDialog, public Ui::SelectClubDialogUi {
    Q_OBJECT

public:
    SelectClubDialog(Event *m_event, QWidget* parent = nullptr);

    QStringList returnVereine();

private slots:
    void select1();

private:
    void initData();

    Event *m_event;
    QStringList vereine;
};
#endif
