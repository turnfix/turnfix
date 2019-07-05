#ifndef EDITORGRAPHICSSCENE_H
#define EDITORGRAPHICSSCENE_H

#include "editorgraphicsscene.h"
#include "src/global/header/_global.h"
#include <QGraphicsScene>

class QPrinter;
class EditorGraphicsItem;

class EditorGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    EditorGraphicsScene(QObject *parent = nullptr);
    EditorGraphicsItem *addTurnFixItem(_global::itemdata lst);
    void removeTurnFixItem(int id);
    void removeAllItems();
    void selectItem(int value);

private slots:
    void sizePosChanged(EditorGraphicsItem *, QRectF);
    void connectMove(bool);
    void itemClicked(EditorGraphicsItem *);

private:
    double pixelToMM(int pixel);
    QPrinter *p;

signals:
    void updateSize(int,QRectF);
    void selectItemInList(int);
    void move(bool);
};

#endif // GSC_EDITORSCENE_H
