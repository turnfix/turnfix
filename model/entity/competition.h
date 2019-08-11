#ifndef COMPETITION_H
#define COMPETITION_H

#include "division.h"
#include "event.h"
#include <QObject>

class DBTable;
class Event;

class Competition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int eventId READ eventId WRITE setEventId)
    Q_PROPERTY(int divisionId READ divisionId WRITE setDivisionId)
    Q_PROPERTY(int type READ type WRITE setType)
    Q_PROPERTY(QString number READ number WRITE setNumber)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int minYear READ minYear WRITE setMinYear)
    Q_PROPERTY(int maxYear READ maxYear WRITE setMaxYear)
    Q_PROPERTY(int qualifiers READ qualifiers WRITE setQualifiers)
    Q_PROPERTY(bool dropScores READ dropScores WRITE setDropScores)
    Q_PROPERTY(bool showAgeGroup READ showAgeGroup WRITE setShowAgeGroup)
    Q_PROPERTY(bool apparatusChoices READ apparatusChoices WRITE setApparatusChoices)
    Q_PROPERTY(int round READ round WRITE setRound)
    Q_PROPERTY(int group READ group WRITE setGroup)
    Q_PROPERTY(QTime startTime READ startTime WRITE setStartTime)
    Q_PROPERTY(QTime warmUpTime READ warmUpTime WRITE setWarmUpTime)
    Q_PROPERTY(bool showInfo READ showInfo WRITE setShowInfo)
    Q_PROPERTY(bool freeAndCompulsary READ freeAndCompulsary WRITE setFreeAndCompulsary)
    Q_PROPERTY(bool sortAsc READ sortAsc WRITE setSortAsc)
    Q_PROPERTY(bool manualSort READ manualSort WRITE setManualSort)
    Q_PROPERTY(bool extraApparatusPoints READ extraApparatusPoints WRITE setExtraApparatusPoints)
    Q_PROPERTY(int numDropScores READ numDropScores WRITE setNumDropScores)

    Q_PROPERTY(Division *division READ division WRITE setDivision STORED false)

public:
    Q_INVOKABLE Competition() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    int eventId() const;
    void setEventId(int eventId);

    int divisionId() const;
    void setDivisionId(int divisionId);

    Division *division() const;
    void setDivision(Division *division);

    int type() const;
    void setType(int type);

    QString number() const;
    void setNumber(const QString &number);

    QString name() const;
    void setName(const QString &name);

    int minYear() const;
    QString minYearText() const;
    void setMinYear(int minYear);

    int maxYear() const;
    QString maxYearText() const;
    void setMaxYear(int maxYear);

    int qualifiers() const;
    void setQualifiers(int qualifiers);

    int scores() const;
    void setScores(int scores);

    bool dropScores() const;
    void setDropScores(bool dropScores);

    bool showAgeGroup() const;
    void setShowAgeGroup(bool showAgeGroup);

    bool apparatusChoices() const;
    void setApparatusChoices(bool apparatusChoices);

    int round() const;
    void setRound(int round);

    int group() const;
    void setGroup(int group);

    QTime startTime() const;
    void setStartTime(const QTime &startTime);

    QTime warmUpTime() const;
    void setWarmUpTime(const QTime &warmUpTime);

    bool showInfo() const;
    void setShowInfo(bool showInfo);

    bool freeAndCompulsary() const;
    void setFreeAndCompulsary(bool freeAndCompulsary);

    bool sortAsc() const;
    void setSortAsc(bool sortAsc);

    bool manualSort() const;
    void setManualSort(bool manualSort);

    bool extraApparatusPoints() const;
    void setExtraApparatusPoints(bool extraApparatusPoints);

    int numDropScores() const;
    void setNumDropScores(int numDropScores);

    Event *event() const;
    void setEvent(Event *event);

private:
    int m_id = 0;
    int m_eventId;
    Event *m_event;
    int m_divisionId;
    Division *m_division;
    int m_type;
    QString m_number;
    QString m_name;
    int m_minYear;
    int m_maxYear;
    int m_qualifiers;
    int m_scores;
    bool m_dropScores;
    bool m_showAgeGroup;
    bool m_apparatusChoices;
    int m_round;
    int m_group;
    QTime m_startTme;
    QTime m_warmUpTime;
    bool m_showInfo;
    bool m_freeAndCompulsary;
    bool m_sortAsc;
    bool m_manualSort;
    bool m_extraApparatusPoints;
    int m_numDropScores;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Competition *);

#endif // COMPETITION_H
