#ifndef FORMULAMODEL_H
#define FORMULAMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Formula;

class FormulaModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FormulaModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchFormulas();

private:
    QList<Formula *> m_formulas;
    EntityManager *m_em;
};

#endif // FORMULAMODEL_H
