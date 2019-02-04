#ifndef WDG_TAB_ED_H
#define WDG_TAB_ED_H

#include <QWidget>
#include <QPrinter>
#include "ui_wdg_tab_ed.h"
#include "../../global/header/_global.h"

class EditorScene;
class QtTreePropertyBrowser;
class QtProperty;
class QtRectFPropertyManager;
class QtFontPropertyManager;
class QtEnumPropertyManager;
class QtStringPropertyManager;
class QtGroupPropertyManager;
class QtDoublePropertyManager;
class QComboBox;
class EditorTextItem;

class Tab_ED : public QWidget, public Ui::Tab_Ed_Ui {
    Q_OBJECT

public:
    Tab_ED(QWidget *parent = 0);
    static QList<EditorTextItem*> items;
public slots:
    static QList<_global::itemdata> getItems();
    static QList<EditorTextItem*> getEditorItems();
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
    QList<EditorTextItem*> sortItems(QList<EditorTextItem*>);
private:

    EditorScene *scene;
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

#endif // WDG_TAB_ED_H
