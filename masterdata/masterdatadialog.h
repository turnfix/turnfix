#ifndef MASTERDATADIALOG_H
#define MASTERDATADIALOG_H

#include <QMainWindow>

namespace Ui {
class MasterdataDialog;
}

class EntityManager;
class QAbstractTableModel;
class QSortFilterProxyModel;

class MasterdataDialog : public QMainWindow
{
    Q_OBJECT

public:
    enum Type {
        AthleteData = 1,
        ClubData = 2,
        DisciplineData = 3,
        SportData = 4,
        DivisionData = 5,
        PersonData = 6,
        StatusData = 7,
        BankAccountData = 8,
        VenueData = 9,
        RegionData = 10,
        StateData = 11,
        CountryData = 12,
        PenaltyData = 13,
        DisciplineGroupData = 14,
        FormulaData = 15
    };

    MasterdataDialog(EntityManager *m_em, QWidget *parent = nullptr, Type type = Type::AthleteData);
    ~MasterdataDialog() override;

private slots:
    void add();
    void edit();
    void del();
    void updateFilterColumn(int index);
    void updateFilterText(const QString &text);

private:
    void updateModel(Type type);

    Ui::MasterdataDialog *ui;
    int m_currentType;
    EntityManager *m_em;
    QAbstractTableModel *m_model;
    QSortFilterProxyModel *m_sortModel;
};
#endif
