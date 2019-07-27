#ifndef DISCIPLINEFIELDMODEL_H
#define DISCIPLINEFIELDMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Discipline;
class DisciplineField;

class DisciplineFieldModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DisciplineFieldModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void fetchFields(Discipline *discipline);
    void addField();
    void removeField(const QModelIndex &index);

    void moveUp(const QModelIndex &index);
    void moveDown(const QModelIndex &index);

private:
    QList<DisciplineField *> m_fields;
    QList<DisciplineField *> m_removedFields;
    EntityManager *m_em;
};

#endif // DISCIPLINEFIELDMODEL_H
