#ifndef COUNTRYDIALOG_H
#define COUNTRYDIALOG_H
#include <QDialog>

namespace Ui {
class CountryDialog;
}

class Country;
class EntityManager;

class CountryDialog : public QDialog
{
    Q_OBJECT
public:
    CountryDialog(Country *country, EntityManager *em, QWidget *parent = nullptr);
    ~CountryDialog() override;

    Country *country();

private slots:
    void save();

private:
    Ui::CountryDialog *ui;
    EntityManager *m_em;
    Country *m_country;
};

#endif
