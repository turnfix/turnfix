#ifndef GROUP_H
#define GROUP_H

#include "club.h"
#include <QObject>

class DBTable;

class Group : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int clubId READ clubId WRITE setClubId)
    Q_PROPERTY(QString name READ name WRITE setName)

    Q_PROPERTY(Club *club READ club WRITE setClub STORED false)

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
Q_DECLARE_METATYPE(Group *);

#endif // GROUP_H
