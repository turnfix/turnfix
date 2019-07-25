#ifndef SPORTMODEL_H
#define SPORTMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Sport;

class SportModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SportModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchSports();

private:
    QList<Sport *> m_sports;
    EntityManager *m_em;
};

#endif // SPORTMODEL_H
