#ifndef MDL_RIEGE_H
#define MDL_RIEGE_H

#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QStringList>

class Event;

class ResultsSheetTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ResultsSheetTableModel(Event *event, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                  int role = Qt::EditRole);
    void setTableData(QString riege, int geraet, int versuche, bool kuer, bool jury);
    QList<int> getExtraColumnIDs();
    int getCurrentID(const QModelIndex &index);
    int getNextID(const QModelIndex &index);
    int getLastID(const QModelIndex &index);

private:
    Event *event;
    QString riege;
    QList<QStringList> starter;
    QMap<int, QMap<int,double> > endwerte;
    QMap<int, QMap<int, QMap <int, double > > > detailwerte;
    QSqlQuery disinfo;
    int geraet;
    bool kuer;
    int versuche;
    QList<int> extraColumns;
    QStringList extraColumnNames;
};
#endif
