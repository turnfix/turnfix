#ifndef GROUPE_H
#define GROUPE_H

#include "club.h"
#include <QObject>

class DBTable;

class Groupe : public QObject
{
public:
    int id() const;
    void setId(int id);

    int clubId() const;
    void setClubId(int clubId);

    Club *club() const;
    void setClub(Club *value);

    QString name() const;
    void setName(const QString &name);

    static const DBTable *mapping();
private:
    int m_id = 0;
    int m_clubId;
    Club *m_club;
    QString m_name;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};

#endif // GROUPE_H
