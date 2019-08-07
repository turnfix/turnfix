#ifndef ATHLETEMODEL_H
#define ATHLETEMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Athlete;

class AthleteModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit AthleteModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchAthletes();

private:
    QList<Athlete *> m_athletes;
    EntityManager *m_em;
};

#endif // ATHLETEMODEL_H
