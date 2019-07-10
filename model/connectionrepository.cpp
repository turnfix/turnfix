#include "connectionrepository.h"
#include "abstractconnection.h"
#include "sqliteconnection.h"
#include <QSettings>

ConnectionRepository::ConnectionRepository() {}

QList<AbstractConnection *> ConnectionRepository::loadAll()
{
    QSettings settings("connections", QSettings::NativeFormat);

    QList<AbstractConnection *> connections;
    for (auto groupName : settings.childGroups()) {
        settings.beginGroup(groupName);
        QString name = settings.value("name").toString();
        auto connection = new SQLiteConnection();
        connection->setName(name);
        settings.endGroup();
        connections.append(connection);
    }

    return connections;
}

void ConnectionRepository::persist(AbstractConnection *connection)
{
    QSettings settings("connections", QSettings::NativeFormat);

    settings.beginGroup(connection->getUuid().toString());
    settings.setValue("type", connection->getType());
    settings.endGroup();
}
