#ifndef COMPETITION_H
#define COMPETITION_H

#include <QObject>
#include "event.h"

class Competition : public QObject
{
    Q_OBJECT
public:
    static Competition* getByNumber(Event *event, QString number);

    Competition(Event *event, QString number, int type, QObject *parent = nullptr);

    Event* getEvent();
    QString getNumber();
    int getType();

private:
    static QMap<QString, Competition*> competitionsByNumber;

    Event *event;
    QString number;
    int type;
};

#endif // COMPETITION_H
