#ifndef STATUSMODEL_H
#define STATUSMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Status;

class StatusModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit StatusModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchStatuses();

private:
    QList<Status *> m_statuses;
    EntityManager *m_em;
};

#endif // STATUSMODEL_H
