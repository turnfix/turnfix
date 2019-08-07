#ifndef DIVISIONMODEL_H
#define DIVISIONMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Division;

class DivisionModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DivisionModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchDivisions();

private:
    QList<Division *> m_divisions;
    EntityManager *m_em;
};

#endif // DIVISIONMODEL_H
