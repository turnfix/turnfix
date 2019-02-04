#ifndef GSC_EDITORSCENE_H
#define GSC_EDITORSCENE_H

#include <QGraphicsScene>
#include "gri_editoritem.h"
#include "../../global/header/_global.h"

class QEditorModel;

class EditorScene : public QGraphicsScene {
    Q_OBJECT

public:
    EditorScene(QObject *parent = 0);
    EditorTextItem* addTurnFixItem(_global::itemdata lst);
    void removeTurnFixItem(int id);
    void removeAllItems();
    void selectItem(int value);

private slots:
    void sizePosChanged(EditorTextItem*,QRectF);
    void connectMove(bool);
    void itemClicked(EditorTextItem*);

private:
    double pixelToMM(int pixel);
    QPrinter *p;

signals:
    void updateSize(int,QRectF);
    void selectItemInList(int);
    void move(bool);

};

#endif // GSC_EDITORSCENE_H
