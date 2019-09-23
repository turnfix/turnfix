#ifndef SQUADDISCIPLINE_H
#define SQUADDISCIPLINE_H

#include <QObject>

#include "discipline.h"
#include "event.h"
#include "status.h"

class DBTable;

class SquadDiscipline : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int eventId READ eventId WRITE setEventId)
    Q_PROPERTY(int disciplineId READ disciplineId WRITE setDisciplineId)
    Q_PROPERTY(int statusId READ statusId WRITE setStatusId)
    Q_PROPERTY(QString squad READ squad WRITE setSquad)
    Q_PROPERTY(int round READ round WRITE setRound)
    Q_PROPERTY(bool start READ start WRITE setStart)

    Q_PROPERTY(Event *event READ event WRITE setEvent STORED false)
    Q_PROPERTY(Discipline *discipline READ discipline WRITE setDiscipline STORED false)
    Q_PROPERTY(Status *status READ status WRITE setStatus STORED false)

public:
    Q_INVOKABLE SquadDiscipline() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    int eventId() const;
    void setEventId(int eventId);

    Event *event() const;
    void setEvent(Event *event);

    int disciplineId() const;
    void setDisciplineId(int disciplineId);

    Discipline *discipline() const;
    void setDiscipline(Discipline *discipline);

    int statusId() const;
    void setStatusId(int statusId);

    Status *status() const;
    void setStatus(Status *status);

    QString squad() const;
    void setSquad(const QString &squad);

    int round() const;
    void setRound(int round);

    bool start() const;
    void setStart(bool start);

private:
    int m_id = 0;
    int m_eventId;
    Event *m_event;
    int m_disciplineId;
    Discipline *m_discipline;
    int m_statusId;
    Status *m_status;
    QString m_squad;
    int m_round;
    bool m_start;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(SquadDiscipline *);

#endif // SQUADDISCIPLINE_H
