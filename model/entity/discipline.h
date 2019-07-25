#ifndef DISCIPLINE_H
#define DISCIPLINE_H

#include "formula.h"
#include "sport.h"
#include <QObject>

class DBTable;

class Discipline : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int sportId READ sportId WRITE setSportId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString shortName1 READ shortName1 WRITE setShortName1)
    Q_PROPERTY(QString shortName2 READ shortName2 WRITE setShortName2)
    Q_PROPERTY(QString resultFormula READ resultFormula WRITE setResultFormula)
    Q_PROPERTY(QString inputMask READ inputMask WRITE setInputMask)
    Q_PROPERTY(int attempts READ attempts WRITE setAttempts)
    Q_PROPERTY(QString icon READ icon WRITE setIcon)
    Q_PROPERTY(QString shortcut READ shortcut WRITE setShortcut)
    Q_PROPERTY(int decimals READ decimals WRITE setDecimals)
    Q_PROPERTY(QString unit READ unit WRITE setUnit)
    Q_PROPERTY(bool lanes READ lanes WRITE setLanes)
    Q_PROPERTY(bool men READ men WRITE setMen)
    Q_PROPERTY(bool women READ women WRITE setWomen)
    Q_PROPERTY(int formulaId READ formulaId WRITE setFormulaId)
    Q_PROPERTY(bool calculate READ calculate WRITE setCalculate)

    Q_PROPERTY(Sport *sport READ sport WRITE setSport STORED false)

public:
    Q_INVOKABLE Discipline() = default;

    int id() const;
    void setId(int id);

    int sportId() const;
    void setSportId(int sportId);

    Sport *sport();
    void setSport(Sport *sport);

    QString name() const;
    void setName(const QString &name);

    QString shortName1() const;
    void setShortName1(const QString &shortName1);

    QString shortName2() const;
    void setShortName2(const QString &shortName2);

    QString resultFormula() const;
    void setResultFormula(const QString &resultFormula);

    QString inputMask() const;
    void setInputMask(const QString &inputMask);

    int attempts() const;
    void setAttempts(int attempts);

    QString icon() const;
    void setIcon(const QString &icon);

    QString shortcut() const;
    void setShortcut(const QString &shortcut);

    int decimals() const;
    void setDecimals(int decimals);

    QString unit() const;
    void setUnit(const QString &unit);

    bool lanes() const;
    void setLanes(bool lanes);

    bool men() const;
    void setMen(bool men);

    bool women() const;
    void setWomen(bool women);

    int formulaId() const;
    void setFormulaId(int formulaId);

    Formula *formula() const;
    void setFormula(Formula *formula);

    bool calculate() const;
    void setCalculate(bool calculate);

    static const DBTable *mapping();

private:
    int m_id = 0;
    int m_sportId;
    Sport *m_sport;
    QString m_name;
    QString m_shortName1;
    QString m_shortName2;
    QString m_resultFormula;
    QString m_inputMask;
    int m_attempts;
    QString m_icon;
    QString m_shortcut;
    int m_decimals;
    QString m_unit;
    bool m_lanes;
    bool m_men;
    bool m_women;
    int m_formulaId;
    Formula *m_formula;
    bool m_calculate;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Discipline *);

#endif // DISCIPLINE_H
