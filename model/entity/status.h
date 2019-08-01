#ifndef STATUS_H
#define STATUS_H

#include <QColor>
#include <QObject>

class DBTable;

class Status : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString colorString READ colorString WRITE setColorString)
    Q_PROPERTY(bool scoresheet READ scoresheet WRITE setScoresheet)
    Q_PROPERTY(bool scorecard READ scorecard WRITE setScorecard)

public:
    Q_INVOKABLE Status() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    QString colorString() const;
    void setColorString(const QString &color);

    QColor color();
    void setColor(const QColor &color);

    bool scoresheet() const;
    void setScoresheet(bool scoresheet);

    bool scorecard() const;
    void setScorecard(bool scorecard);

private:
    int m_id = 0;
    QString m_name;
    QString m_colorString;
    QColor m_color;
    bool m_scoresheet;
    bool m_scorecard;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Status *);

#endif // STATUS_H
