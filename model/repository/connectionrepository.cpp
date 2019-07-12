#include "connectionrepository.h"
#include "model/entity/abstractconnection.h"
#include "model/entity/postgresqlconnection.h"
#include "model/entity/sqliteconnection.h"
#include <QMetaProperty>
#include <QSettings>

ConnectionRepository::ConnectionRepository() {}

QList<AbstractConnection *> ConnectionRepository::loadAll()
{
    QSettings settings("connections", QSettings::NativeFormat);

    QList<AbstractConnection *> connections;
    for (auto groupName : settings.childGroups()) {
        settings.beginGroup(groupName);

        QString type = settings.value("type").toString();
        AbstractConnection *connection = nullptr;

        if (type == "SQLiteConnection") {
            connection = new SQLiteConnection(groupName);
        } else if (type == "PostgreSQLConnection") {
            connection = new PostgreSQLConnection(groupName);
        }

        if (connection != nullptr) {
            const QMetaObject *metaobject = connection->metaObject();

            int count = metaobject->propertyCount();
            for (int i = 0; i < count; ++i) {
                QMetaProperty metaproperty = metaobject->property(i);
                const char *name = metaproperty.name();

                connection->setProperty(name, settings.value(name));
            }

            connections.append(connection);
        }

        settings.endGroup();
    }

    return connections;
}

void ConnectionRepository::persist(AbstractConnection *connection)
{
    QSettings settings("connections", QSettings::NativeFormat);

    settings.beginGroup(connection->uuid().toString());

    const QMetaObject *metaobject = connection->metaObject();
    settings.setValue("type", metaobject->className());

    int count = metaobject->propertyCount();
    for (int i = 0; i < count; ++i) {
        QMetaProperty metaproperty = metaobject->property(i);
        const char *name = metaproperty.name();

        settings.setValue(name, connection->property(name));
    }

    settings.endGroup();
}

void ConnectionRepository::remove(AbstractConnection *connection)
{
    QSettings settings("connections", QSettings::NativeFormat);
    settings.beginGroup(connection->uuid().toString());
    settings.remove("");
    settings.endGroup();
}
