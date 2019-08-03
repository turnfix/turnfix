#ifndef DISCIPLINEORDERMODEL_H
#define DISCIPLINEORDERMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class CompetitionDiscipline;
class DisciplinePosition;
class Competition;

class DisciplineOrderModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DisciplineOrderModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchPositions(Competition *competition);

    QModelIndex moveUp(const QModelIndex &index);
    QModelIndex moveDown(const QModelIndex &index);
    QModelIndex moveLeft(const QModelIndex &index);
    QModelIndex moveRight(const QModelIndex &index);

public slots:
    void addDiscipline(CompetitionDiscipline *discipline);
    void removeDiscipline(CompetitionDiscipline *discipline);

private:
    void insertDiscipline(DisciplinePosition *position);
    void updateFreeAndCompulsary(Qt::CheckState state, CompetitionDiscipline *discipline);
    void updateRow(int row, int column, int targetRow);
    void updateColumn(int row, int column, int targetColumn);
    void updateSize();
    EntityManager *m_em;
    QMap<int, QMap<int, DisciplinePosition *>> m_data;
    int m_rows = 0;
    int m_columns = 0;
};

#endif // DISCIPLINEORDERMODEL_H
