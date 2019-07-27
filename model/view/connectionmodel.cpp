#include "connectionmodel.h"
#include "model/entity/postgresqlconnection.h"
#include "model/entity/sqliteconnection.h"
#include "model/entitymanager.h"
#include "model/repository/connectionrepository.h"

ConnectionModel::ConnectionModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , repository(em->connectionRepository())
    , connections(repository->loadAll())
{
}

int ConnectionModel::columnCount(const QModelIndex &) const
{
    return 5;
}

int ConnectionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return connections.length();
}

QVariant ConnectionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        AbstractConnection *connection = connections.at(index.row());
        QString type = connection->metaObject()->className();
        if (type == "SQLiteConnection") {
            auto *sqlite = dynamic_cast<SQLiteConnection *>(connection);
            switch (index.column()) {
            case 0:
                return sqlite->name();
            case 1:
                return sqlite->fileName();
            }
        } else if (type == "PostgreSQLConnection") {
            auto *postgres = dynamic_cast<PostgreSQLConnection *>(connection);
            switch (index.column()) {
            case 0:
                return postgres->name();
            case 1:
                return postgres->server();
            case 2:
                return postgres->username();
            case 3:
                return postgres->password();
            case 4:
                return postgres->databaseName();
            }
        }
    }

    return QVariant();
}

bool ConnectionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    AbstractConnection *connection = connections.at(index.row());
    QString type = connection->metaObject()->className();
    if (type == "SQLiteConnection") {
        auto *sqlite = dynamic_cast<SQLiteConnection *>(connection);
        switch (index.column()) {
        case 0:
            sqlite->setName(value.toString());
            break;
        case 1:
            sqlite->setFileName(value.toString());
            break;
        }
    } else if (type == "PostgreSQLConnection") {
        auto *postgres = dynamic_cast<PostgreSQLConnection *>(connection);
        switch (index.column()) {
        case 0:
            postgres->setName(value.toString());
            break;
        case 1:
            postgres->setServer(value.toString());
            break;
        case 2:
            postgres->setUsername(value.toString());
            break;
        case 3:
            postgres->setPassword(value.toString());
            break;
        case 4:
            postgres->setDatabaseName(value.toString());
            break;
        }
    }

    repository->persist(connection);
    emit dataChanged(index, index);

    return true;
}

void ConnectionModel::addConnection(AbstractConnection::Type type)
{
    AbstractConnection *connection;

    switch (type) {
    case AbstractConnection::Type::SQLite:
        connection = new SQLiteConnection(this);
        break;
    case AbstractConnection::Type::PostgreSQL:
        connection = new PostgreSQLConnection(this);
        break;
    }

    if (connection == nullptr) {
        return;
    }

    beginInsertRows(QModelIndex(), connections.length(), connections.length() + 1);
    repository->persist(connection);
    connections.append(connection);
    endInsertRows();
}

void ConnectionModel::removeConnection(const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    AbstractConnection *connection = connections.at(index.row());
    repository->remove(connection);
    connections.removeAt(index.row());
    endRemoveRows();
}

AbstractConnection *ConnectionModel::connectionAt(const QModelIndex &index)
{
    return connections.at(index.row());
}

AbstractConnection *ConnectionModel::connectionAt(int index)
{
    return connections.at(index);
}
