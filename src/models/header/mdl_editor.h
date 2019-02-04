#ifndef MDL_EDITOR_H
#define MDL_EDITOR_H

#include <QAbstractTableModel>
#include "../../misc/header/gri_editoritem.h"

class QEditorModel : public QAbstractTableModel {
    Q_OBJECT
public:
    QEditorModel(QObject *parent = 0)
        : QAbstractTableModel(parent) {}
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    void setTableData(QList<EditorTextItem*> lst);

private:
    QList<EditorTextItem*> items;
};

#endif // MDL_EDITOR_H
