#ifndef QUALIFICATIONSTANDARDSDIALOG_H
#define QUALIFICATIONSTANDARDSDIALOG_H
#include <QDialog>
#include "ui_qualificationstandardsdialog.h"

class QQualiTableModel;
class Event;

class QualificationStandardsDialog : public QDialog, public Ui::QualificationStandardsDialogUi {
    Q_OBJECT

public:
    QualificationStandardsDialog(Event *event, int edit=0, QWidget* parent=0);

protected slots:
    void initData();
    void finishEdit();

protected:
    Event *event;
    int editid;
    QQualiTableModel *model;
};

#endif
