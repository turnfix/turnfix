#include "editorgraphicsitem.h"
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

EditorGraphicsItem::EditorGraphicsItem(_global::itemdata v, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    p = new QPrinter(QPrinter::ScreenResolution);
    p->setPaperSize(QPrinter::A4);
    x = 0;
    y = 0;
    setPos(mmToPixel(v.x),mmToPixel(v.y));
    w = mmToPixel(v.width);
    h = mmToPixel(v.height);
    values = v;
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);

    QStringList fields = _global::getFields();
    QStringList types;
    types << "DB: " << "TEXT: " << "GRAFIK: " << "LINIE: ";
    QString text;
    switch (v.typ) {
    case 0: text = fields.at(v.value.toInt()); break;
    case 1:
    case 2: text = v.value.toString(); break;
    case 3: text = "Linienstärke: " + v.value.toString(); break;
    }
}

QRectF EditorGraphicsItem::boundingRect() const
{
    return QRectF(x, y, w, h);
}

void EditorGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    if (this->isSelected()) {
        QPen pen;
        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DotLine);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
        painter->setPen(Qt::SolidLine);
        painter->fillRect(QRectF(boundingRect().x(),boundingRect().y(),5.0,5.0),QBrush(QColor(0,0,0,255)));
        painter->fillRect(QRectF(boundingRect().x()+boundingRect().width()-5.0,boundingRect().y(),5.0,5.0),QBrush(QColor(0,0,0,255)));
        painter->fillRect(QRectF(boundingRect().x(),boundingRect().y()+boundingRect().height()-5.0,5.0,5.0),QBrush(QColor(0,0,0,255)));
        painter->fillRect(QRectF(boundingRect().x()+boundingRect().width()-5.0,boundingRect().y()+boundingRect().height()-5.0,5.0,5.0),QBrush(QColor(0,0,0,255)));
        painter->fillRect(QRectF(boundingRect().x()+boundingRect().width()/2-2.5,boundingRect().y()+boundingRect().height()-5.0,5.0,5.0),QBrush(QColor(0,0,0,255)));
        painter->fillRect(QRectF(boundingRect().x()+boundingRect().width()/2-2.5,boundingRect().y(),5.0,5.0),QBrush(QColor(0,0,0,255)));
        painter->fillRect(QRectF(boundingRect().x(),boundingRect().y()+boundingRect().height()/2-2.5,5.0,5.0),QBrush(QColor(0,0,0,255)));
        painter->fillRect(QRectF(boundingRect().x()+boundingRect().width()-5.0,boundingRect().y()+boundingRect().height()/2-2.5,5.0,5.0),QBrush(QColor(0,0,0,255)));
    }
    QFont font;
    painter->setPen(Qt::SolidLine);
    int align;
    switch (values.align) {
    case 1: align = 0x0001; break;
    case 2: align = 0x0002; break;
    default: align = 0x0004; break;
    }
    if (values.typ == 0) {
        QString field = _global::getFields().at(values.value.toInt());
        painter->setFont(values.font);
        painter->drawText(static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h), align, field);
    } else if (values.typ == 1) {
        painter->setFont(values.font);
        painter->drawText(static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h), align, values.value.toString());
    } else if (values.typ == 2) {
        QPixmap pm;
        pm.load(values.value.toString());
        if (!pm.isNull()) {
            QPixmap pm2 = pm.scaled(static_cast<int>(w), static_cast<int>(h), Qt::KeepAspectRatio,Qt::SmoothTransformation);
            painter->drawPixmap(static_cast<int>(x), static_cast<int>(y), pm2);
        }
    } else if (values.typ == 3) {
        QPen pen;
        pen.setWidth(static_cast<int>(mmToPixel(values.value.toInt())));
        painter->setPen(pen);
        painter->drawLine(static_cast<int>(x), static_cast<int>(y), static_cast<int>(w+x), static_cast<int>(h+y));
        pen.setWidth(0);
        painter->setPen(pen);
    }
        painter->restore();
}

void EditorGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    emit clicked(this);
    emit sizePosChanged(this,this->sceneBoundingRect());
    QGraphicsItem::mousePressEvent(e);
}

void EditorGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    emit move(true);
    switch (selected) {
    case 0: QGraphicsItem::mouseMoveEvent(e);
        break;
    case 1: w = w+(x-e->pos().x());
        h = h+(y-e->pos().y());
        moveBy(e->pos().x(),e->pos().y());
        break;
    case 2: w = e->pos().x()-x;
        h = h+(y-e->pos().y());
        moveBy(0,e->pos().y());
        break;
    case 3: w = w+(x-e->pos().x());
        h = e->pos().y()-y;
        moveBy(e->pos().x(),0);
        break;
    case 4: w = e->pos().x()-x;
        h = e->pos().y()-y;
        break;
    case 5: h = h+(y-e->pos().y());
        y = e->pos().y();
        break;
    case 6: h = e->pos().y()-y;
        break;
    case 7: moveBy(e->pos().x(),0);
        w = w+(e->lastPos().x()-e->pos().x());
        break;
    case 8: w = e->pos().x()-x;
        break;
    }
    scene()->update(this->scene()->sceneRect());
    emit sizePosChanged(this,this->sceneBoundingRect());
    emit move(false);
}

void EditorGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
    if (e->pos().x() > boundingRect().x() && e->pos().x() < boundingRect().x()+5.0 && e->pos().y() > boundingRect().y() && e->pos().y() < boundingRect().y()+5.0) {
        selected = 1;
        setCursor(Qt::SizeFDiagCursor);
    } else if (e->pos().x() > boundingRect().x()+boundingRect().width()-5.0 && e->pos().x() < boundingRect().x()+boundingRect().width() && e->pos().y() > boundingRect().y() && e->pos().y() < boundingRect().y()+5.0) {
        selected = 2;
        setCursor(Qt::SizeBDiagCursor);
    } else if (e->pos().x() > boundingRect().x() && e->pos().x() < boundingRect().x()+5.0 && e->pos().y() > boundingRect().y()+boundingRect().height()-5.0 && e->pos().y() < boundingRect().y()+boundingRect().height()) {
        selected = 3;
        setCursor(Qt::SizeBDiagCursor);
    } else if (e->pos().x() > boundingRect().x()+boundingRect().width()-5.0 && e->pos().x() < boundingRect().x()+boundingRect().width() && e->pos().y() > boundingRect().y()+boundingRect().height()-5.0 && e->pos().y() < boundingRect().y()+boundingRect().height()) {
        selected = 4;
        setCursor(Qt::SizeFDiagCursor);
    } else if (e->pos().x() > boundingRect().x()+boundingRect().width()/2-2.5 && e->pos().x() < boundingRect().x()+boundingRect().width()/2+2.5 && e->pos().y() > boundingRect().y() && e->pos().y() < boundingRect().y()+5.0) {
        selected = 5;
        setCursor(Qt::SizeVerCursor);
    } else if (e->pos().x() > boundingRect().x()+boundingRect().width()/2-2.5 && e->pos().x() < boundingRect().x()+boundingRect().width()/2+2.5 && e->pos().y() > boundingRect().y()+boundingRect().height()-5.0 && e->pos().y() < boundingRect().y()+boundingRect().height()) {
        selected = 6;
        setCursor(Qt::SizeVerCursor);
    } else if (e->pos().x() > boundingRect().x() && e->pos().x() < boundingRect().x()+5.0 && e->pos().y() > boundingRect().y()+boundingRect().height()/2-2.5 && e->pos().y() < boundingRect().y()+boundingRect().height()/2+2.5) {
        selected = 7;
        setCursor(Qt::SizeHorCursor);
    } else if (e->pos().x() > boundingRect().x()+boundingRect().width()-5.0 && e->pos().x() < boundingRect().x()+boundingRect().width() && e->pos().y() > boundingRect().y()+boundingRect().height()/2-2.5 && e->pos().y() < boundingRect().y()+boundingRect().height()/2+2.5) {
        selected = 8;
        setCursor(Qt::SizeHorCursor);
    } else {
        selected = 0;
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsItem::hoverMoveEvent(e);
}

qreal EditorGraphicsItem::mmToPixel(double mm)
{
    qreal px = static_cast<double>(p->width()) * mm / static_cast<int>(p->widthMM());
    return px;
}

void EditorGraphicsItem::setNewSizePos(double nx, double ny, double nw, double nh)
{
    w = mmToPixel(nw);
    h = mmToPixel(nh);
    this->setPos(mmToPixel(nx),mmToPixel(ny));
    scene()->update(this->scene()->sceneRect());
}

void EditorGraphicsItem::setNewData(QVariant data)
{
    values.value = data;
    scene()->update(this->scene()->sceneRect());
}

void EditorGraphicsItem::setNewFont(QFont font)
{
    values.font = font;
    scene()->update(this->scene()->sceneRect());
}

void EditorGraphicsItem::setNewAlign(int align)
{
    values.align = align;
    scene()->update(this->scene()->sceneRect());
}

_global::itemdata EditorGraphicsItem::getItemData()
{
    return values;
}

void EditorGraphicsItem::updateItemData(_global::itemdata data)
{
    values = data;
}
