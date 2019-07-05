#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include "src/global/header/_global.h"
#include <QPrinter>
#include <QWidget>

namespace Ui {
class EditorWidget;
}

class EditorGraphicsScene;
class QtTreePropertyBrowser;
class QtProperty;
class QtRectFPropertyManager;
class QtFontPropertyManager;
class QtEnumPropertyManager;
class QtStringPropertyManager;
class QtGroupPropertyManager;
class QtDoublePropertyManager;
class QComboBox;
class EditorGraphicsItem;

class EditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditorWidget(QWidget *parent = nullptr);
    ~EditorWidget();

    static QList<EditorGraphicsItem *> items;
public slots:
    static QList<_global::itemdata> getItems();
    static QList<EditorGraphicsItem *> getEditorItems();
    void initPreview(QList<_global::itemdata> values);
private slots:
    void addField();
    void delField();
    void load();
    void save();
    void sizePosChanged(int,QRectF);
    void move(bool moving);
    void addNewItem(_global::itemdata);
    void removeAllItems();
    double pixelToMM(int pixel);
    int mmToPixel(double mm);
    void enableDataWidget();
    void selectRow(int row);
    void selectItem();
    void setNewSizePos(QtProperty* prop, QRectF rect);
    void setNewData();
    void setNewFont();
    void setNewAlign();
    void zoomIn();
    void sendToFront();
    void sendToBack();
    void layerUp();
    void layerDown();
    void alignCenter();
    QList<EditorGraphicsItem *> sortItems(QList<EditorGraphicsItem *>);

private:
    Ui::EditorWidget *ui;
    EditorGraphicsScene *scene;
    QPrinter *p;

    QtTreePropertyBrowser *prop;

    QtGroupPropertyManager *man_db;
    QtGroupPropertyManager *man_txt;
    QtGroupPropertyManager *man_img;
    QtGroupPropertyManager *man_line;

    QtProperty *prop_db_root;
    QtProperty *prop_db;
    QtProperty *prop_txt_root;
    QtProperty *prop_txt;
    QtProperty *prop_img_root;
    QtProperty *prop_img;
    QtProperty *prop_line_root;
    QtProperty *prop_line;
    QtProperty *prop_size;
    QtProperty *prop_font;
    QtProperty *prop_align;

    QtRectFPropertyManager *itm_size;

    QtFontPropertyManager *itm_font;

    QtEnumPropertyManager *val_db;
    QtEnumPropertyManager *val_align;
    QtStringPropertyManager *val_txt;
    QtDoublePropertyManager *val_line;
signals:
    void sendData(QList<_global::itemdata>);
};

#endif // EDITORWIDGET_H
