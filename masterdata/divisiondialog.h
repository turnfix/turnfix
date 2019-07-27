#ifndef DIVISIONDIALOG_H
#define DIVISIONDIALOG_H

#include <QDialog>

namespace Ui {
class DivisionDialog;
}

class Division;
class EntityManager;

class DivisionDialog : public QDialog
{
    Q_OBJECT

public:
    DivisionDialog(Division *division, EntityManager *em, QWidget *parent = nullptr);
    ~DivisionDialog();

    Division *division();

private slots:
    void save();

private:
    Ui::DivisionDialog *ui;
    EntityManager *m_em;
    Division *m_division;
};
#endif
