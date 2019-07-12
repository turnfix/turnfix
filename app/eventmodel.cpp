#include "eventmodel.h"
#include "model/entity/event.h"
#include "model/entity/location.h"
#include "model/entitymanager.h"
#include "model/repository/eventrepository.h"

EventModel::EventModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int EventModel::rowCount(const QModelIndex &) const
{
    return m_events.size();
}

int EventModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant EventModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0: return tr("Datum");
        case 1: return tr("Veranstaltung");
        case 2: return tr("Wettkampfort");
        }
    }
    return QVariant();
}

QVariant EventModel::data(const QModelIndex &index, int role) const
{
    Event *event;

    if (role == Qt::DisplayRole)
    {
        event = m_events.at(index.row());

        switch (index.column())
        {
        case 0: return event->startDate();
        case 1: return event->name();
        case 2: return event->location()->getNameAndCity();
        }
    }
    return QVariant();
}

void EventModel::getEvents()
{
    beginResetModel();
    m_events = m_em->eventRepository()->loadAll();
    endResetModel();
}

void EventModel::clear()
{
    beginResetModel();
    m_events.clear();
    endResetModel();
}

Event* EventModel::getEventFromRow(int row)
{
    return m_events.at(row);
}
