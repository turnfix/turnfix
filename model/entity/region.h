#ifndef REGION_H
#define REGION_H

#include "state.h"
#include <QObject>

class DBTable;

class Region : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int stateId READ stateId WRITE setStateId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString code READ code WRITE setCode)

    Q_PROPERTY(State *state READ state WRITE setState STORED false)

public:
    Q_INVOKABLE Region() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    int stateId() const;
    void setStateId(int stateId);

    State *state() const;
    void setState(State *state);

    QString name() const;
    void setName(const QString &name);

    QString code() const;
    void setCode(const QString &code);

private:
    int m_id = 0;
    int m_stateId;
    State *m_state;
    QString m_name;
    QString m_code;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Region *);

#endif // REGION_H
