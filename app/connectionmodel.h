#ifndef CONNECTIONMODEL_H
#define CONNECTIONMODEL_H

#include <QAbstractListModel>

class AbstractConnection;
class EntityManager;

class ConnectionModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ConnectionModel(EntityManager *em, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<AbstractConnection *> connections;
};

#endif // CONNECTIONMODEL_H
