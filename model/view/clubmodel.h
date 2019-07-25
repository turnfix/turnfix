#ifndef CLUBMODEL_H
#define CLUBMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Club;

class ClubModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ClubModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchClubs();

private:
    QList<Club *> m_clubs;
    EntityManager *m_em;
};

#endif // CLUBMODEL_H
