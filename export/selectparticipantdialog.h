#ifndef SELECTPARTICIPANTDIALOG_H
#define SELECTPARTICIPANTDIALOG_H
#include <QDialog>

namespace Ui {
class SelectParticipantDialog;
}

class EntityManager;
class Event;
class ResultsTableModel;

class SelectParticipantDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectParticipantDialog(Event *m_event, QWidget *parent = nullptr);
    ~SelectParticipantDialog();

public slots:
    QList<int> getTnList();
    QString getTnWk();

private slots:
    void initData();
    void updateList();
    void submit();

private:
    Ui::SelectParticipantDialog *ui;
    Event *m_event;
    EntityManager *m_em;
    QList<int> tnlist;
    QString tnwk;
    ResultsTableModel *er_model;
};
#endif
