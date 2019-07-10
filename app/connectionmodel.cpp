#include "connectionmodel.h"
#include "model/abstractconnection.h"
#include "model/connectionrepository.h"
#include "model/entitymanager.h"

ConnectionModel::ConnectionModel(EntityManager *em, QObject *parent)
    : QAbstractListModel(parent)
    , connections(em->getConnectionRepository()->loadAll())
{
}

int ConnectionModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return connections.length();
}

QVariant ConnectionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return connections.at(index.row())->getName();
}
