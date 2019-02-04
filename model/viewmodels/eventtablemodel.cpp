#include "eventtablemodel.h"
#include "model/objects/event.h"
#include "model/objects/location.h"

EventTableModel::EventTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int EventTableModel::rowCount(const QModelIndex &) const
{
    return events.size();
}

int EventTableModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant EventTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0: return tr("Datum"); break;
        case 1: return tr("Veranstaltung"); break;
        case 2: return tr("Wettkampfort"); break;
        }
    }
    return QVariant();
}

QVariant EventTableModel::data(const QModelIndex &index, int role) const
{
    Event *event;

    if (role == Qt::DisplayRole)
    {
        event = events.at(index.row());

        switch (index.column())
        {
        case 0: return event->getStartDate(); break;
        case 1: return event->getName(); break;
        case 2: return event->getLocation()->getNameAndCity(); break;
        }
    }
    return QVariant();
}

void EventTableModel::getEvents()
{
    events = Event::getAllEvents();
    this->reset();
}

void EventTableModel::clear()
{
    events.clear();
    this->reset();
}

Event* EventTableModel::getEventFromRow(int row)
{
    return events.at(row);
}
