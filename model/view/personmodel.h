#ifndef PERSONMODEL_H
#define PERSONMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Person;

class PersonModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit PersonModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

    void fetchPersons();

private:
    QList<Person *> m_persons;
    EntityManager *m_em;
};

#endif // PERSONMODEL_H
