#ifndef _DELEGATES_H
#define _DELEGATES_H

#include <QItemDelegate>

class QPainter;

class editorDelegate : public QItemDelegate {
    Q_OBJECT

public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool eventFilter ( QObject * editor, QEvent * event );
};

class cmbDelegate : public QItemDelegate {
    Q_OBJECT

public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class alignItemDelegate : public QItemDelegate {
    Q_OBJECT
    public:
        void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

class alignCItemDelegate : public QItemDelegate {
    Q_OBJECT
    public:
        void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

class dsbxDelegate : public QItemDelegate {
    Q_OBJECT

public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif
