#ifndef STATUSDIALOG_H
#define STATUSDIALOG_H

#include <QDialog>

namespace Ui {
class StatusDialog;
}

class Status;
class EntityManager;
class CountryModel;

class StatusDialog : public QDialog
{
    Q_OBJECT

public:
    StatusDialog(Status *status, EntityManager *em, QWidget *parent = nullptr);
    ~StatusDialog() override;

private slots:
    void save();
    void colorChooser();

private:
    Ui::StatusDialog *ui;
    EntityManager *m_em;
    Status *m_status;
    QColor m_color;
};
#endif
