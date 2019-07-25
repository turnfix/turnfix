#ifndef SECTION_H
#define SECTION_H

#include <QObject>

class DBTable;

class Division : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(bool men READ men WRITE setMen)
    Q_PROPERTY(bool women READ women WRITE setWomen)

public:
    Q_INVOKABLE Division() = default;

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    bool men() const;
    void setMen(bool men);

    bool women() const;
    void setWomen(bool women);

    static const DBTable *mapping();

private:
    int m_id = 0;
    QString m_name;
    bool m_men;
    bool m_women;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Division *);

#endif // SECTION_H
