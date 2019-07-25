#ifndef QUALIFICATIONSTANDARDSDIALOG_H
#define QUALIFICATIONSTANDARDSDIALOG_H
#include <QDialog>

namespace Ui {
    class QualificationStandardsDialog;
}

class QualificationStandardsTableModel;
class Event;

class QualificationStandardsDialog : public QDialog {
    Q_OBJECT

public:
    QualificationStandardsDialog(Event *m_event, int edit=0, QWidget* parent = nullptr);
    ~QualificationStandardsDialog();

protected slots:
    void initData();
    void finishEdit();

protected:
    Event *m_event;
    Ui::QualificationStandardsDialog *ui;
    int editid;
    QualificationStandardsTableModel *model;
};

#endif
