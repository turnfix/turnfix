#ifndef PENALTYDIALOG_H
#define PENALTYDIALOG_H

#include <QDialog>

namespace Ui {
class PenaltyDialog;
}

class Penalty;
class EntityManager;

class PenaltyDialog : public QDialog
{
    Q_OBJECT

public:
    PenaltyDialog(Penalty *penalty, EntityManager *em, QWidget *parent = nullptr);
    ~PenaltyDialog() override;

private slots:
    void save();

private:
    Ui::PenaltyDialog *ui;
    EntityManager *m_em;
    Penalty *m_penalty;
};

#endif
