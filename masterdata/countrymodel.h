#ifndef COUNTRYMODEL_H
#define COUNTRYMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Country;

class CountryModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CountryModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchCountries();

private:
    QList<Country *> m_countries;
    EntityManager *m_em;
};

#endif // COUNTRYMODEL_H
