#include "header/gsc_editorscene.h"
#include "../models/header/mdl_editor.h"
#include "../widgets/header/wdg_tab_ed.h"

EditorScene::EditorScene(QObject *parent) : QGraphicsScene(parent) {
    p = new QPrinter(QPrinter::ScreenResolution);
    p->setPaperSize(QPrinter::A4);
}

EditorTextItem *EditorScene::addTurnFixItem(_global::itemdata lst) {
    EditorTextItem *e = new EditorTextItem(lst);
    connect(e, SIGNAL(sizePosChanged(EditorTextItem*,QRectF)),
            this, SLOT(sizePosChanged(EditorTextItem*,QRectF)));

    connect(e, SIGNAL(move(bool)),
            this, SLOT(connectMove(bool)));

    connect(e, SIGNAL(clicked(EditorTextItem*)),
            this, SLOT(itemClicked(EditorTextItem*)));

    e->setZValue(Tab_ED::items.size());
    this->addItem(e);
    Tab_ED::items.append(e);
    return e;
}

void EditorScene::removeTurnFixItem(int id) {
    EditorTextItem *itm = Tab_ED::items.at(id);
    this->removeItem(itm);
    Tab_ED::items.removeAt(id);
}

void EditorScene::removeAllItems() {
    Tab_ED::items.clear();
    this->clear();
}

void EditorScene::itemClicked(EditorTextItem* itm) {
    int id = Tab_ED::items.indexOf(itm);
    emit selectItemInList(id);
}

void EditorScene::sizePosChanged(EditorTextItem* item, QRectF r) {
    _global::itemdata lst = item->getItemData();
    lst.x      = pixelToMM((int)r.x());
    lst.y      = pixelToMM((int)r.y());
    lst.width  = pixelToMM((int)r.width());
    lst.height = pixelToMM((int)r.height());
    item->updateItemData(lst);
    emit updateSize(item->zValue(),QRectF(lst.x,lst.y,lst.width,lst.height));
}

void EditorScene::selectItem(int i) {
    Tab_ED::items.at(i)->setSelected(true);
}

double EditorScene::pixelToMM(int px) {
    double mm = (double)px * (double)p->widthMM() / (double)p->width();
    return mm;
}

void EditorScene::connectMove(bool moving) {
    emit move(moving);
}
