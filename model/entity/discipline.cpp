#include "discipline.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Discipline::initializeMapping()
{
    DBTable *discipline = new DBTable("tfx_disziplinen");
    discipline->addColumn("id", "int_disziplinenid", ColumnType::Integer, 0, false, "", "", true);
    discipline->addColumn("sportId", "int_sportid", ColumnType::Integer, 0, false);
    discipline->addColumn("name", "var_name", ColumnType::Varchar, 100);
    discipline->addColumn("shortName1", "var_kurz1", ColumnType::Varchar, 6);
    discipline->addColumn("shortName2", "var_kurz2", ColumnType::Varchar, 20);
    discipline->addColumn("resultFormula", "var_formel", ColumnType::Varchar, 300);
    discipline->addColumn("inputMask", "var_maske", ColumnType::Varchar, 10);
    discipline->addColumn("attempts", "int_versuche", ColumnType::Integer, 0, true, "1");
    discipline->addColumn("icon", "var_icon", ColumnType::Varchar, 50);
    discipline->addColumn("shortcut", "var_kuerzel", ColumnType::Varchar, 50);
    discipline->addColumn("decimals", "int_berechnung", ColumnType::SmallInt, 0, true, "2");
    discipline->addColumn("unit", "var_einheit", ColumnType::Varchar, 5);
    discipline->addColumn("lanes", "bol_bahnen", ColumnType::Boolean, 0, true, "'false'");
    discipline->addColumn("men", "bol_m", ColumnType::Boolean, 0, true, "'true'");
    discipline->addColumn("women", "bol_w", ColumnType::Boolean, 0, true, "'true'");
    discipline
        ->addColumn("formulaId", "int_formelid", ColumnType::Integer, 0, true, "", " USING NULL");
    discipline->addColumn("calculate", "bol_berechnen", ColumnType::Boolean, 0, true, "'true'");
    discipline->addContraint("fky_sportid",
                             "tfx_sport",
                             "int_sportid",
                             "int_sportid",
                             "RESTRICT",
                             "RESTRICT");
    discipline->addContraint("fky_formelid",
                             "tfx_formeln",
                             "int_formelid",
                             "int_formelid",
                             "RESTRICT",
                             "RESTRICT");

    return discipline;
}

const DBTable *Discipline::m_mapping = Discipline::initializeMapping();

const DBTable *Discipline::mapping()
{
    return m_mapping;
}

Formula *Discipline::formula() const
{
    return m_formula;
}

void Discipline::setFormula(Formula *formula)
{
    m_formula = formula;
    if (formula == nullptr) {
        m_formulaId = 0;
        return;
    }

    m_formulaId = formula->id();
}

Sport *Discipline::sport()
{
    return m_sport;
}

void Discipline::setSport(Sport *sport)
{
    m_sport = sport;
    if (sport == nullptr) {
        m_sportId = 0;
        return;
    }

    m_sportId = sport->id();
}

int Discipline::id() const
{
    return m_id;
}

void Discipline::setId(int id)
{
    m_id = id;
}

int Discipline::sportId() const
{
    return m_sportId;
}

void Discipline::setSportId(int sportId)
{
    m_sportId = sportId;
}

QString Discipline::name() const
{
    return m_name;
}

void Discipline::setName(const QString &name)
{
    m_name = name;
}

QString Discipline::shortName1() const
{
    return m_shortName1;
}

void Discipline::setShortName1(const QString &shortName1)
{
    m_shortName1 = shortName1;
}

QString Discipline::shortName2() const
{
    return m_shortName2;
}

void Discipline::setShortName2(const QString &shortName2)
{
    m_shortName2 = shortName2;
}

QString Discipline::resultFormula() const
{
    return m_resultFormula;
}

void Discipline::setResultFormula(const QString &formula)
{
    m_resultFormula = formula;
}

QString Discipline::inputMask() const
{
    return m_inputMask;
}

void Discipline::setInputMask(const QString &inputMask)
{
    m_inputMask = inputMask;
}

int Discipline::attempts() const
{
    return m_attempts;
}

void Discipline::setAttempts(int attempts)
{
    m_attempts = attempts;
}

QString Discipline::icon() const
{
    return m_icon;
}

void Discipline::setIcon(const QString &icon)
{
    m_icon = icon;
}

QString Discipline::shortcut() const
{
    return m_shortcut;
}

void Discipline::setShortcut(const QString &shortcut)
{
    m_shortcut = shortcut;
}

int Discipline::decimals() const
{
    return m_decimals;
}

void Discipline::setDecimals(int decimals)
{
    m_decimals = decimals;
}

QString Discipline::unit() const
{
    return m_unit;
}

void Discipline::setUnit(const QString &unit)
{
    m_unit = unit;
}

bool Discipline::lanes() const
{
    return m_lanes;
}

void Discipline::setLanes(bool lanes)
{
    m_lanes = lanes;
}

bool Discipline::men() const
{
    return m_men;
}

void Discipline::setMen(bool men)
{
    m_men = men;
}

bool Discipline::women() const
{
    return m_women;
}

void Discipline::setWomen(bool women)
{
    m_women = women;
}

int Discipline::formulaId() const
{
    return m_formulaId;
}

void Discipline::setFormulaId(int formulaId)
{
    m_formulaId = formulaId;
}

bool Discipline::calculate() const
{
    return m_calculate;
}

void Discipline::setCalculate(bool calculate)
{
    m_calculate = calculate;
}
