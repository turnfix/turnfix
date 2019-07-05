#ifndef _DELEGATES_H
#define _DELEGATES_H

#include <QItemDelegate>

class QPainter;
class Event;

class EditorDelegate : public QItemDelegate {
    Q_OBJECT

public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool eventFilter(QObject *editor, QEvent *event) override;
};

class CmbDelegate : public QItemDelegate {
    Q_OBJECT

public:
    CmbDelegate(Event *event, QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    Event *event;
};

class AlignItemDelegate : public QItemDelegate {
    Q_OBJECT

public:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const override;
};

class AlignCItemDelegate : public QItemDelegate {
    Q_OBJECT

public:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const override;
};

class DsbxDelegate : public QItemDelegate {
    Q_OBJECT

public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif
