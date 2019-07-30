#ifndef ABSTRACTREPOSITORY_H
#define ABSTRACTREPOSITORY_H

#include "model/entitymanager.h"
#include "model/querybuilder.h"
#include <QList>

class EntityManager;

template<class T>
class AbstractRepository
{
public:
    explicit AbstractRepository(EntityManager *em) { m_em = em; }
    virtual ~AbstractRepository() = default;
    virtual bool persist(T *object)
    {
        QSqlDatabase db = QSqlDatabase::database(m_em->connectionName());
        QueryBuilder<T> qb;
        return qb.persist(db, T::staticMetaObject, T::mapping(), object);
    }

    virtual bool remove(T *object)
    {
        QSqlDatabase db = QSqlDatabase::database(m_em->connectionName());
        QueryBuilder<T> qb;
        return qb.remove(db, T::mapping(), object);
    }

protected:
    EntityManager *entityManager() { return m_em; }

private:
    EntityManager *m_em;
};

#endif // ABSTRACTREPOSITORY_H
