#ifndef STATEDIALOG_H
#define STATEDIALOG_H

#include <QDialog>

namespace Ui {
class StateDialog;
}

class State;
class EntityManager;
class CountryModel;

class StateDialog : public QDialog
{
    Q_OBJECT

public:
    StateDialog(State *state, EntityManager *em, QWidget *parent = nullptr);
    ~StateDialog() override;

private slots:
    void save();
    void addCountry();

private:
    Ui::StateDialog *ui;
    EntityManager *m_em;
    CountryModel *m_countryModel;
    State *m_state;
};

#endif
