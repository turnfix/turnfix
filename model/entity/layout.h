#ifndef LAYOUT_H
#define LAYOUT_H

#include <QObject>

class DBTable;

class Layout : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString comment READ comment WRITE setComment)

public:
    Q_INVOKABLE Layout() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    QString comment() const;
    void setComment(const QString &comment);

private:
    int m_id = 0;
    QString m_name;
    QString m_comment;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Layout *);

#endif // LAYOUT_H
