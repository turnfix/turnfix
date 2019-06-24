#ifndef DLG_PASS_H
#define DLG_PASS_H
#include <QDialog>
#include "ui_dlg_pass.h"

class Event;
class QSqlQueryModel;

class Pass_Dialog : public QDialog, public Ui::Pass_Dialog_Ui {
      Q_OBJECT

public:
    Pass_Dialog(Event *event, QWidget* parent = nullptr);
    protected slots:
    void fillTable(int row=0);
    void selectLine();
    void up();
    void down();
    void save();

private:
    Event *event;
    bool eventFilter(QObject *obj, QEvent *ev);
    QSqlQueryModel *model;
};
#endif
