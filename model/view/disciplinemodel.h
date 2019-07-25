#ifndef DISCIPLINEMODEL_H
#define DISCIPLINEMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Discipline;

class DisciplineModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DisciplineModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchDisciplines();

private:
    QList<Discipline *> m_disciplines;
    EntityManager *m_em;
};

#endif // DISCIPLINEMODEL_H
