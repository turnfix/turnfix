#ifndef QUALIFICATIONSTANDARDSDIALOG_H
#define QUALIFICATIONSTANDARDSDIALOG_H
#include <QDialog>

namespace Ui {
    class QualificationStandardsDialog;
}

class QQualiTableModel;
class Event;

class QualificationStandardsDialog : public QDialog {
    Q_OBJECT

public:
    QualificationStandardsDialog(Event *event, int edit=0, QWidget* parent = nullptr);
    ~QualificationStandardsDialog();

protected slots:
    void initData();
    void finishEdit();

protected:
    Event *event;
    Ui::QualificationStandardsDialog *ui;
    int editid;
    QQualiTableModel *model;
};

#endif
