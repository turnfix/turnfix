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
        case 0: return tr("Datum");
        case 1: return tr("Veranstaltung");
        case 2: return tr("Wettkampfort");
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
        case 0: return event->getStartDate();
        case 1: return event->getName();
        case 2: return event->getLocation()->getNameAndCity();
        }
    }
    return QVariant();
}

void EventTableModel::getEvents()
{
    beginResetModel();
    events = Event::getAllEvents();
    endResetModel();
}

void EventTableModel::clear()
{
    beginResetModel();
    events.clear();
    endResetModel();
}

Event* EventTableModel::getEventFromRow(int row)
{
    return events.at(row);
}
