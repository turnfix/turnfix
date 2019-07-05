#ifndef SELECTDISCIPLINEDIALOG_H
#define SELECTDISCIPLINEDIALOG_H
#include <QDialog>
#include "ui_selectdisciplinedialog.h"

class Event;

class SelectDisciplineDialog : public QDialog, public Ui::SelectDisciplineDialogUi {
    Q_OBJECT

public:
    SelectDisciplineDialog(Event *event, QWidget* parent = nullptr);

public slots:
    QList< QList<int> > getDis();

protected slots:
    void initData();
    void select1();

private:
    Event *event;
    QList<QList<int>> dis;
};
#endif
