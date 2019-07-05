#ifndef EDITORGRAPHICSITEM_H
#define EDITORGRAPHICSITEM_H

#include "src/global/header/_global.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPrinter>

class EditorGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    EditorGraphicsItem(_global::itemdata values, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const;
    void setNewSizePos(double x, double y, double w, double h);
    void setNewData(QVariant data);
    void setNewFont(QFont font);
    void setNewAlign(int align);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    _global::itemdata getItemData();
    void updateItemData(_global::itemdata);

protected:
    int selected;
    double x, y, w, h;
    _global::itemdata values;
    qreal mmToPixel(double mm);
    QPrinter *p;
signals:
    void sizePosChanged(EditorGraphicsItem *, QRectF);
    void clicked(EditorGraphicsItem *);
    void move(bool);
};

#endif
