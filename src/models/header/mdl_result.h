#ifndef MDL_RESULT_H
#define MDL_RESULT_H

#include <QAbstractTableModel>
#include <QStringList>

class QErgebnisTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    QErgebnisTableModel(QObject *parent = 0)
        : QAbstractTableModel(parent) {}
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
