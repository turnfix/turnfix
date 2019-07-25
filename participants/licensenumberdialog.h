#ifndef LICENSENUMBERDIALOG_H
#define LICENSENUMBERDIALOG_H

#include <QDialog>

namespace Ui {
class LicenseNumberDialog;
}

class Event;
class QSqlQueryModel;

class LicenseNumberDialog : public QDialog
{
    Q_OBJECT

public:
    LicenseNumberDialog(Event *m_event, QWidget *parent = nullptr);
    ~LicenseNumberDialog();

private slots:
    void fillTable(int row=0);
    void selectLine();
    void up();
    void down();
    void save();

private:
    Ui::LicenseNumberDialog *ui;
    Event *m_event;
    bool eventFilter(QObject *obj, QEvent *ev);
    QSqlQueryModel *model;
};
#endif
