#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include "model/dbcolumn.h"
#include "model/dbtable.h"
#include <QDebug>
#include <QList>
#include <QMetaObject>
#include <QMetaProperty>
#include <QSqlQuery>
#include <QString>
#include <QStringList>

template<class T>
class QueryBuilder
{
public:
    QueryBuilder select(QMetaObject metaObject, const DBTable *mapping)
    {
        m_select = "SELECT ";
        m_from = QString("FROM %1").arg(mapping->name());
        m_selectMetaObject = metaObject;
        m_mappings.insert(metaObject.className(), mapping);

        for (int i = metaObject.propertyOffset(); i < metaObject.propertyCount(); i++) {
            QMetaProperty property = metaObject.property(i);
            if (!property.isStored()) {
                continue;
            }
            DBColumn *column = mapping->columnByProperty(property.name());
            if (column == nullptr) {
                continue;
            }

            QString columnName = column->name();

            m_select += QString("%1.%2, ").arg(mapping->name(), columnName);
        }

        m_select = m_select.left(m_select.length() - 2);

        return *this;
    }

    QueryBuilder join(QMetaObject metaObject,
                      const DBTable *mapping,
                      const QString &joinClass,
                      const QString &propertyName,
                      const QString &idName,
                      const QString &key = "id")
    {
        m_select += ", ";
        m_from += QString(" LEFT JOIN %1 ON %2.%3 = %4.%5")
                      .arg(mapping->name(),
                           m_mappings.value(joinClass)->name(),
                           m_mappings.value(joinClass)->columnByProperty(idName)->name(),
                           mapping->name(),
                           mapping->columnByProperty(key)->name());
        m_joinMetaObjects.insert(propertyName, metaObject);

        for (int i = metaObject.propertyOffset(); i < metaObject.propertyCount(); i++) {
            QMetaProperty property = metaObject.property(i);
            if (!property.isStored()) {
                continue;
            }

            auto *column = mapping->columnByProperty(property.name());
            if (column == nullptr) {
                continue;
            }

            QString columnName = column->name();

            m_select += QString("%1.%2, ").arg(mapping->name(), columnName);
        }

        m_select = m_select.left(m_select.length() - 2);

        return *this;
    }

    QueryBuilder orderBy(const QString &className,
                         const QString &propertyName,
                         const QString &order = "ASC")
    {
        if (m_order != "") {
            m_order += ", ";
        }

        m_order += QString("%1.%2 %3")
                       .arg(m_mappings.value(className)->name(),
                            m_mappings.value(className)->columnByProperty(propertyName)->name(),
                            order);

        return *this;
    }

    QueryBuilder where(const QString &className, const QString &propertyName, const QVariant &value)
    {
        if (m_where != "") {
            m_where += " AND ";
        }

        m_where += QString("%1.%2 = ?")
                       .arg(m_mappings.value(className)->name(),
                            m_mappings.value(className)->columnByProperty(propertyName)->name());
        m_bindValues.append(value);

        return *this;
    }

    QList<T *> query(QSqlDatabase db)
    {
        QString queryString = QString("%1 %2").arg(m_select, m_from);
        if (m_where != "") {
            queryString += QString(" WHERE %1").arg(m_where);
        }
        if (m_order != "") {
            queryString += QString(" ORDER BY %1").arg(m_order);
        }

        qDebug() << queryString;

        QSqlQuery query(db);
        query.prepare(queryString);
        for (int i = 0; i < m_bindValues.length(); i++) {
            query.bindValue(i, m_bindValues.at(i));
        }
        query.exec();

        QList<T *> output;
        while (query.next()) {
            auto *obj = dynamic_cast<T *>(convert(query));

            output.append(obj);
        }

        return output;
    }

    bool persist(QSqlDatabase db, QMetaObject metaObject, const DBTable *mapping, T *obj)
    {
        QObject *qobj = obj;
        int id = qobj->property("id").toInt();

        if (id == 0) {
            QStringList columns;
            QStringList placeholders;
            QMap<QString, QVariant> values;

            for (int i = metaObject.propertyOffset(); i < metaObject.propertyCount(); i++) {
                QMetaProperty property = metaObject.property(i);
                if (!property.isStored() || QString(property.name()) == "id") {
                    continue;
                }

                auto *column = mapping->columnByProperty(property.name());
                if (column == nullptr) {
                    continue;
                }

                columns.append(column->name());
                placeholders.append(QString(":%1").arg(property.name()));
                values.insert(QString(":%1").arg(property.name()), property.read(qobj));
            }

            QString queryString = QString("INSERT INTO %1 (%2) VALUES(%3)")
                                      .arg(mapping->name(),
                                           columns.join(", "),
                                           placeholders.join(", "));

            qDebug() << queryString;

            QSqlQuery query(db);
            query.prepare(queryString);
            QStringList keys = values.keys();
            for (auto key : keys) {
                query.bindValue(key, values.value(key));
            }

            return query.exec();
        }

        QStringList columns;
        QMap<QString, QVariant> values;
        QString idColumn;

        for (int i = metaObject.propertyOffset(); i < metaObject.propertyCount(); i++) {
            QMetaProperty property = metaObject.property(i);
            if (!property.isStored()) {
                continue;
            }

            auto *column = mapping->columnByProperty(property.name());
            if (column == nullptr) {
                continue;
            }

            if (QString(property.name()) == "id") {
                idColumn = column->name();
                continue;
            }

            columns.append(
                QString("%1 = %2").arg(column->name(), QString(":%1").arg(property.name())));
            values.insert(QString(":%1").arg(property.name()), property.read(qobj));
        }

        QString queryString = QString("UPDATE %1 SET %2 WHERE %3 = :id")
                                  .arg(mapping->name(), columns.join(", "), idColumn);

        qDebug() << queryString;

        QSqlQuery query(db);
        query.prepare(queryString);
        QStringList keys = values.keys();
        for (auto key : keys) {
            query.bindValue(key, values.value(key));
        }
        query.bindValue(":id", id);

        return query.exec();
    }

    bool remove(QSqlDatabase db, const DBTable *mapping, T *obj)
    {
        QString queryString = QString("DELETE FROM %1 WHERE %2 = :id")
                                  .arg(mapping->name(), mapping->columnByProperty("id")->name());

        qDebug() << queryString;

        QSqlQuery query(db);
        query.prepare(queryString);
        query.bindValue(":id", obj->id());

        return query.exec();
    }

private:
    QObject *convert(const QSqlQuery &query)
    {
        auto *obj = m_selectMetaObject.newInstance();
        auto *metaObj = obj->metaObject();

        int index = 0;
        for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); i++) {
            auto property = metaObj->property(i);
            if (!property.isStored()) {
                continue;
            }

            obj->setProperty(property.name(), query.value(index));
            index++;
        }

        QStringList keys = m_joinMetaObjects.keys();
        for (const QString &key : keys) {
            auto joinMetaObject = m_joinMetaObjects.value(key);
            auto *joinObj = joinMetaObject.newInstance();
            auto *joinMetaObj = joinObj->metaObject();
            for (int i = joinMetaObj->propertyOffset(); i < joinMetaObj->propertyCount(); i++) {
                auto property = joinMetaObj->property(i);
                if (!property.isStored()) {
                    continue;
                }

                joinObj->setProperty(property.name(), query.value(index));

                index++;
            }

            obj->setProperty(key.toUtf8(), QVariant::fromValue(joinObj));
        }

        return obj;
    }

    QString m_select;
    QString m_from;
    QString m_where;
    QString m_order;
    QMetaObject m_selectMetaObject;
    QMap<QString, QMetaObject> m_joinMetaObjects;
    QMap<QString, const DBTable *> m_mappings;
    QList<QVariant> m_bindValues;
};

#endif // QUERYBUILDER_H
