#ifndef ASSIGNMENTTABLEMODEL_H
#define ASSIGNMENTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMimeData>
#include <QSqlQuery>
#include <QStringList>

class Event;

class AssignmentTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    AssignmentTableModel(Event *event, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QStringList takeRow(int row);
    void insertRow(const QStringList &data);
    void setTableData();

public slots:
    void setRiege(const QString &riege);
    void updateRiege(const QString &riege);

private:
    QList<QStringList> tabledata;
    QString riege;
    Event *event;
};
#endif
