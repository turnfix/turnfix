#ifndef PARTICIPANTSTABLEMODEL_H
#define PARTICIPANTSTABLEMODEL_H

#include <QSqlQueryModel>
#include "model/objects/event.h"

class ParticipantsTableModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Type { Individual, Team, Group };

    ParticipantsTableModel(Event *event);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void updateType(Type type);

private:
    Event *event;
    Type type;
};

#endif // PARTICIPANTSTABLEMODEL_H
