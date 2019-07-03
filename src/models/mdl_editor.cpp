#include "header/mdl_editor.h"
#include "../global/header/_global.h"

int QEditorModel::rowCount(const QModelIndex &) const {
    return items.size();
}

int QEditorModel::columnCount(const QModelIndex &) const {
    return 3;
}

QVariant QEditorModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (index.row() >= items.size())
        return QVariant();
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            QStringList types;
            types << "DB" << "TEXT" << "GRAFIK" << "LINIE";
            return types.at(items.at(index.row())->getItemData().typ);
        } else if (index.column() == 1) {
            QStringList fields = _global::getFields();
            QString text;
            switch (items.at(index.row())->getItemData().typ) {
            case 0: text = fields.at(items.at(index.row())->getItemData().value.toInt()); break;
            case 1:
            case 2: text = items.at(index.row())->getItemData().value.toString(); break;
            case 3: text = "Linienstärke: " + items.at(index.row())->getItemData().value.toString(); break;
            }
            return text;
        } else if (index.column() == 2) {
            return items.at(index.row())->zValue();
        }
    }
    return QVariant();
}

QVariant QEditorModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Typ";
        case 1: return "Wert";
        }
    }
    return QVariant();
}

void QEditorModel::setTableData(QList<EditorTextItem*> lst) {
    items = lst;
}
