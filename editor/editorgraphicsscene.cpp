#include "editorgraphicsscene.h"
#include "editorgraphicsitem.h"
#include "editorwidget.h"
#include <QPrinter>

EditorGraphicsScene::EditorGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
    p = new QPrinter(QPrinter::ScreenResolution);
    p->setPaperSize(QPrinter::A4);
}

EditorGraphicsItem *EditorGraphicsScene::addTurnFixItem(_global::itemdata lst)
{
    EditorGraphicsItem *e = new EditorGraphicsItem(lst);
    connect(e,
            SIGNAL(sizePosChanged(EditorGraphicsItem *, QRectF)),
            this,
            SLOT(sizePosChanged(EditorGraphicsItem *, QRectF)));

    connect(e, SIGNAL(move(bool)),
            this, SLOT(connectMove(bool)));

    connect(e, SIGNAL(clicked(EditorGraphicsItem *)), this, SLOT(itemClicked(EditorGraphicsItem *)));

    //e->setZValue(Tab_ED::items.size());
    this->addItem(e);
    //Tab_ED::items.append(e);
    return e;
}

void EditorGraphicsScene::removeTurnFixItem(int id)
{
    EditorGraphicsItem *itm = EditorWidget::items.at(id);
    this->removeItem(itm);
    EditorWidget::items.removeAt(id);
}

void EditorGraphicsScene::removeAllItems()
{
    EditorWidget::items.clear();
    this->clear();
}

void EditorGraphicsScene::itemClicked(EditorGraphicsItem *itm)
{
    int id = EditorWidget::items.indexOf(itm);
    emit selectItemInList(id);
}

void EditorGraphicsScene::sizePosChanged(EditorGraphicsItem *item, QRectF r)
{
    _global::itemdata lst = item->getItemData();
    lst.x      = pixelToMM((int)r.x());
    lst.y      = pixelToMM((int)r.y());
    lst.width  = pixelToMM((int)r.width());
    lst.height = pixelToMM((int)r.height());
    item->updateItemData(lst);
    emit updateSize(item->zValue(),QRectF(lst.x,lst.y,lst.width,lst.height));
}

void EditorGraphicsScene::selectItem(int i)
{
    EditorWidget::items.at(i)->setSelected(true);
}

double EditorGraphicsScene::pixelToMM(int px)
{
    double mm = (double)px * (double)p->widthMM() / (double)p->width();
    return mm;
}

void EditorGraphicsScene::connectMove(bool moving)
{
    emit move(moving);
}
