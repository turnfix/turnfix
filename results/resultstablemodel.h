#ifndef RESULTSTABLEMODEL_H
#define RESULTSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

class ResultsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    void setList(const QList<QStringList> &list, QString nr, int hwk, int typ, bool details=true, bool header=true);

private:
    QList<QStringList> list;
    QStringList headers;
    int wktyp;
    int hwk;
    bool details;
    bool showHeader;
    QString nr;
};
#endif
