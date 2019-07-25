#ifndef INDIVIDUALDIALOG_H
#define INDIVIDUALDIALOG_H

#include <QDialog>

namespace Ui {
    class IndividualDialog;
}

class Event;

class IndividualDialog : public QDialog {
    Q_OBJECT

public:
    IndividualDialog(Event *m_event, int edit=0, QWidget* parent = nullptr);
    ~IndividualDialog();

private slots:
    void initData();
    void save();
    void checkUpdate();
    void checkDisziplinen();
    void checkJg();
    void changeDat();
    void addClub();
    void updateClubs();

private:
    Event *m_event;
    Ui::IndividualDialog *ui;
    int editid;
};

#endif
