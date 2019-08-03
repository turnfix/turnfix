#ifndef CONNECTIONMODEL_H
#define CONNECTIONMODEL_H

#include "model/entity/abstractconnection.h"
#include <QAbstractTableModel>

class ConnectionRepository;
class EntityManager;

class ConnectionModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ConnectionModel(EntityManager *em, QObject *parent = nullptr);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void addConnection(AbstractConnection::Type type);
    void removeConnection(const QModelIndex &index);

    AbstractConnection *connectionAt(const QModelIndex &index);
    AbstractConnection *connectionAt(int index);

private:
    ConnectionRepository *repository;
    QList<AbstractConnection *> connections;
};

#endif // CONNECTIONMODEL_H
