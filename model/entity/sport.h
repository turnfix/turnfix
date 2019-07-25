#ifndef SPORT_H
#define SPORT_H

#include <QObject>

class DBTable;

class Sport : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    Q_INVOKABLE Sport() = default;

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    static const DBTable *mapping();

private:
    int m_id = 0;
    QString m_name;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Sport *);

#endif // SPORT_H
