#ifndef ABSTRACTREPOSITORY_H
#define ABSTRACTREPOSITORY_H

#include <QList>

template<typename T>
class AbstractRepository
{
public:
    virtual QList<T> loadAll() = 0;
    virtual void persist(T object) = 0;
};

#endif // ABSTRACTREPOSITORY_H
