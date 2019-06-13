#ifndef GRI_EDITORITEM_H
#define GRI_EDITORITEM_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPrinter>
#include "../../global/header/_global.h"

class EditorTextItem : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    EditorTextItem(_global::itemdata values,QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const;
    void setNewSizePos(double x, double y, double w, double h);
    void setNewData(QVariant data);
    void setNewFont(QFont font);
    void setNewAlign(int align);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent (  QGraphicsSceneHoverEvent * event );
    _global::itemdata getItemData();
    void updateItemData(_global::itemdata);
protected:
    int selected;
    double x, y, w, h;
    _global::itemdata values;
    qreal mmToPixel(double mm);
    QPrinter *p;
signals:
    void sizePosChanged(EditorTextItem*,QRectF);
    void clicked(EditorTextItem*);
    void move(bool);
};

#endif
