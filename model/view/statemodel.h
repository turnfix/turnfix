#ifndef STATEMODEL_H
#define STATEMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class State;
class Country;

class StateModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit StateModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchStates();

private:
    QList<State *> m_states;
    EntityManager *m_em;
};

#endif // STATEMODEL_H
