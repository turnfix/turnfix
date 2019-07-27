#ifndef EVENTTABLEMODEL_H
#define EVENTTABLEMODEL_H

#include <QAbstractTableModel>

class Event;
class EntityManager;

class EventModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit EventModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

    void getEvents();
    void clear();
    Event* getEventFromRow(int row);

private:
    QList<Event *> m_events;
    EntityManager *m_em;
};

#endif // EVENTTABLEMODEL_H
