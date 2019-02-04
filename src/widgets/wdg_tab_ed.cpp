#include <QScrollArea>
#include <QDomDocument>
#include <QTextStream>
#include <QPainter>
#include <QGraphicsScene>
#include <QtTreePropertyBrowser>
#include <QtGroupPropertyManager>
#include <QtSpinBoxFactory>
#include <QFileDialog>
#include <QSqlQuery>
#include "header/wdg_tab_ed.h"
#include "../misc/header/gsc_editorscene.h"
#include "../misc/header/gri_editoritem.h"
#include "../dialogs/select/header/dlg_select_layout.h"
#include "../dialogs/others/header/dlg_save_layout.h"

QList<EditorTextItem*> Tab_ED::items = QList<EditorTextItem*>();

Tab_ED::Tab_ED(QWidget *parent) : QWidget(parent) {
    setupUi(this);

    cmb_zoom->addItem("25%");
    cmb_zoom->addItem("50%");
    cmb_zoom->addItem("75%");
    cmb_zoom->addItem("100%");
    cmb_zoom->addItem("125%");
    cmb_zoom->addItem("150%");
    cmb_zoom->addItem("175%");
    cmb_zoom->addItem("200%");
    cmb_zoom->setCurrentIndex(3);

    p = new QPrinter(QPrinter::ScreenResolution);
    p->setPaperSize(QPrinter::A4);
    scene = new EditorScene();
    scene->setBackgroundBrush(Qt::white);
    sa->setBackgroundRole(QPalette::Dark);
    graphicsView->setFixedSize(mmToPixel(210),mmToPixel(297));
    scene->setSceneRect(0,0,mmToPixel(210),mmToPixel(297));
    graphicsView->setScene(scene);
    QStringList header;
    header << "Typ" << "Inhalt";
    tbl_items->setHorizontalHeaderLabels(header);
    tbl_items->horizontalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
    tbl_items->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
    itm_size = new QtRectFPropertyManager(wdg_props);
    itm_font = new QtFontPropertyManager(wdg_props);
    val_db = new QtEnumPropertyManager(wdg_props);
    val_align = new QtEnumPropertyManager(wdg_props);
    val_txt = new QtStringPropertyManager(wdg_props);
    val_line = new QtDoublePropertyManager(wdg_props);

    prop_size = itm_size->addProperty("Maße");
    prop_font = itm_font->addProperty("Schriftart");

    //Alignment
    prop_align = val_align->addProperty("Ausrichtung");
    QStringList alignments;
    alignments <<  "zentriert" << "links" << "rechts";
    val_align->setEnumNames(prop_align,alignments);

    //Properties Database
    man_db = new QtGroupPropertyManager(wdg_props);
    prop_db_root = man_db->addProperty("Datenbankelement");
    prop_db = val_db->addProperty("DB-Feld");
    QStringList fields = _global::getFields();
    val_db->setEnumNames(prop_db,fields);
    prop_db_root->addSubProperty(prop_size);
    prop_db_root->addSubProperty(prop_font);
    prop_db_root->addSubProperty(prop_db);
    prop_db_root->addSubProperty(prop_align);

    //Properties Text
    man_txt = new QtGroupPropertyManager(wdg_props);
    prop_txt_root = man_txt->addProperty("Textelement");
    prop_txt = val_txt->addProperty("Text");
    prop_txt_root->addSubProperty(prop_size);
    prop_txt_root->addSubProperty(prop_font);
    prop_txt_root->addSubProperty(prop_txt);
    prop_txt_root->addSubProperty(prop_align);

    //Properties Grafik
    man_img = new QtGroupPropertyManager(wdg_props);
    prop_img_root = man_txt->addProperty("Grafik");
    prop_img = val_txt->addProperty("Datei");
    prop_img_root->addSubProperty(prop_size);
    prop_img_root->addSubProperty(prop_img);

    //Properties Linie
    man_line = new QtGroupPropertyManager(wdg_props);
    prop_line_root = man_txt->addProperty("Grafik");
    prop_line = val_line->addProperty("Stärke");
    prop_line_root->addSubProperty(prop_size);
    prop_line_root->addSubProperty(prop_line);
    val_line->setMinimum(prop_line,0.0);
    val_line->setSingleStep(prop_line,0.01);


    QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(wdg_props);
    QtEnumEditorFactory *enumFactory = new QtEnumEditorFactory(wdg_props);
    QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(wdg_props);
    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(wdg_props);
    QtFontEditorFactory *fontFactory = new QtFontEditorFactory(wdg_props);
    QtLineEditFactory *lineEditFactory = new QtLineEditFactory(wdg_props);

    prop = new QtTreePropertyBrowser();
    prop->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    prop->setFixedWidth(250);
    prop->setFactoryForManager(itm_size->subDoublePropertyManager(), doubleSpinBoxFactory);
    prop->setFactoryForManager(itm_font->subBoolPropertyManager(), checkBoxFactory);
    prop->setFactoryForManager(itm_font->subIntPropertyManager(), spinBoxFactory);
    prop->setFactoryForManager(itm_font->subEnumPropertyManager(), enumFactory);
    prop->setFactoryForManager(itm_font, fontFactory);
    prop->setFactoryForManager(val_db, enumFactory);
    prop->setFactoryForManager(val_txt, lineEditFactory);
    prop->setFactoryForManager(val_line, doubleSpinBoxFactory);
    prop->setFactoryForManager(val_align, enumFactory);
    prop->setHeaderVisible(false);
    wdg_props->layout()->addWidget(prop);

    connect(but_open, SIGNAL(clicked()), this, SLOT(load()));
    connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(but_reset, SIGNAL(clicked()), this, SLOT(removeAllItems()));
    connect(but_front, SIGNAL(clicked()), this, SLOT(sendToFront()));
    connect(but_back, SIGNAL(clicked()), this, SLOT(sendToBack()));
    connect(but_moveup, SIGNAL(clicked()), this, SLOT(layerUp()));
    connect(but_movedown, SIGNAL(clicked()), this, SLOT(layerDown()));
    connect(but_acenter, SIGNAL(clicked()), this, SLOT(alignCenter()));
    connect(tbl_items, SIGNAL(clicked(QModelIndex)), this, SLOT(selectItem()));
    connect(scene, SIGNAL(move(bool)), this, SLOT(move(bool)));
    connect(scene, SIGNAL(updateSize(int,QRectF)), this, SLOT(sizePosChanged(int,QRectF)));
    connect(scene, SIGNAL(selectItemInList(int)), this, SLOT(selectRow(int)));
    connect(itm_size, SIGNAL(valueChanged(QtProperty*,QRectF)), this, SLOT(setNewSizePos(QtProperty*,QRectF)));
    connect(itm_font, SIGNAL(valueChanged(QtProperty*,QFont)), this, SLOT(setNewFont()));
    connect(val_db, SIGNAL(valueChanged(QtProperty*,int)), this, SLOT(setNewData()));
    connect(val_txt, SIGNAL(valueChanged(QtProperty*,QString)), this, SLOT(setNewData()));
    connect(val_line, SIGNAL(valueChanged(QtProperty*,double)), this, SLOT(setNewData()));
    connect(val_align, SIGNAL(valueChanged(QtProperty*,int)), this, SLOT(setNewAlign()));
    connect(but_add, SIGNAL(clicked()), this, SLOT(addField()));
    connect(but_del, SIGNAL(clicked()), this, SLOT(delField()));
    connect(cmb_zoom, SIGNAL(currentIndexChanged(int)), this, SLOT(zoomIn()));
}

void Tab_ED::initPreview(QList<_global::itemdata> val) {
    for (int i=0;i<val.size();i++) {
        addNewItem(val.at(i));
    }
}

QList<_global::itemdata> Tab_ED::getItems() {
    QList<_global::itemdata> lst;
    for (int i=0;i<items.size();i++) {
        lst.append(items.at(i)->getItemData());
    }
    return lst;
}

QList<EditorTextItem*> Tab_ED::getEditorItems() {
    return items;
}

void Tab_ED::addNewItem(_global::itemdata lst) {
    scene->addTurnFixItem(lst);
    int row = items.size()-1;
    QStringList types;
    types << "DB" << "TEXT" << "GRAFIK" << "LINIE";
    QStringList fields = _global::getFields();
    QString text;
    switch (lst.typ) {
    case 0: text = fields.at(lst.value.toInt()); break;
    case 1:
    case 2: text = lst.value.toString(); break;
    case 3: text = "Linienstärke: " + lst.value.toString(); break;
    }
    tbl_items->insertRow(row);
    tbl_items->setItem(row,0,new QTableWidgetItem(types.at(lst.typ)));
    tbl_items->setItem(row,1,new QTableWidgetItem(text));
    tbl_items->horizontalHeader()->reset();
}

void Tab_ED::addField() {
    _global::itemdata lst = {-1,cmb_typ->currentIndex(),0.0,0.0,45.0,35.0,QFont("Tahoma",12),0,0};
    addNewItem(lst);
}

void Tab_ED::delField() {
    if (scene->selectedItems().isEmpty()) return;
    scene->removeTurnFixItem(tbl_items->currentRow());
    scene->update(scene->sceneRect());
    tbl_items->removeRow(tbl_items->currentRow());
    if (tbl_items->rowCount() > 0) {
        selectRow(0);
    }
}

void Tab_ED::load() {
    Select_Layout *sel = new Select_Layout();
    if (sel->exec() == 1) {
        removeAllItems();
        QSqlQuery layoutItemQuery;
        layoutItemQuery.prepare("SELECT int_layout_felderid, int_typ, var_font, rel_x, rel_y, rel_w, rel_h, var_value, int_align FROM tfx_layout_felder WHERE int_layoutid=? ORDER BY int_layer");
        layoutItemQuery.bindValue(0,sel->getLayoutID());
        layoutItemQuery.exec();
        while (layoutItemQuery.next()) {
            QFont font;
            font.fromString(layoutItemQuery.value(2).toString());
            _global::itemdata v = {
                layoutItemQuery.value(0).toInt(),
                layoutItemQuery.value(1).toInt(),
                layoutItemQuery.value(3).toDouble(),
                layoutItemQuery.value(4).toDouble(),
                layoutItemQuery.value(5).toDouble(),
                layoutItemQuery.value(6).toDouble(),
                font,
                layoutItemQuery.value(7),
                layoutItemQuery.value(8).toInt()
            };
            addNewItem(v);
        }
    }
}

void Tab_ED::removeAllItems() {
    scene->removeAllItems();
    scene->update(scene->sceneRect());
    prop->clear();
    int rows = tbl_items->rowCount();
    for (int i=0;i<rows;i++) tbl_items->removeRow(0);
}

void Tab_ED::save() {
    Save_Layout *sld = new Save_Layout();
    sld->setItems(items);
    if (sld->exec() == 1) {
        items = sld->getItems();
    }
}

void Tab_ED::sizePosChanged(int row,QRectF r) {
    if (tbl_items->currentRow() != -1) {
        itm_size->setValue(prop_size,QRectF(r.x(), r.y(), r.width(),r.height()));
    }
}

void Tab_ED::move(bool moving) {
    if (moving) {
        disconnect(itm_size,0,0,0);
    } else {
        connect(itm_size, SIGNAL(valueChanged(QtProperty*,QRectF)), this, SLOT(setNewSizePos(QtProperty*,QRectF)));
    }
}

double Tab_ED::pixelToMM(int px) {
    double mm = (double)px * (double)p->widthMM() / (double)p->width();
    return mm;
}

int Tab_ED::mmToPixel(double mm) {
    int px = (double)p->width() * mm / (double)p->widthMM();
    return px;
}

void Tab_ED::enableDataWidget() {
    if (prop->properties().size()>0) prop->removeProperty(prop->properties().at(0));
    if (tbl_items->currentIndex().isValid()) {
        int row = tbl_items->currentIndex().row();
        switch (items.at(row)->getItemData().typ) {
        case 0: prop->addProperty(prop_db_root);
            prop->setBackgroundColor(prop->items(prop_db_root).at(0),Qt::gray);
            prop->setBackgroundColor(prop->items(prop_size).at(0),QColor(255,248,209));
            prop->setBackgroundColor(prop->items(prop_font).at(0),QColor(207,255,164));
            prop->setBackgroundColor(prop->items(prop_db).at(0),QColor(255,206,184));
            break;
        case 1: prop->addProperty(prop_txt_root);
            prop->setBackgroundColor(prop->items(prop_txt_root).at(0),Qt::gray);
            prop->setBackgroundColor(prop->items(prop_size).at(0),QColor(255,248,209));
            prop->setBackgroundColor(prop->items(prop_font).at(0),QColor(207,255,164));
            prop->setBackgroundColor(prop->items(prop_txt).at(0),QColor(255,206,184));
            break;
        case 2: prop->addProperty(prop_img_root);
            prop->setBackgroundColor(prop->items(prop_img_root).at(0),Qt::gray);
            prop->setBackgroundColor(prop->items(prop_size).at(0),QColor(255,248,209));
            prop->setBackgroundColor(prop->items(prop_img).at(0),QColor(255,206,184));
            break;
        case 3: prop->addProperty(prop_line_root);
            prop->setBackgroundColor(prop->items(prop_line_root).at(0),Qt::gray);
            prop->setBackgroundColor(prop->items(prop_size).at(0),QColor(255,248,209));
            prop->setBackgroundColor(prop->items(prop_line).at(0),QColor(255,206,184));
            break;
        }

        move(true);
        itm_size->setValue(prop_size,QRectF(items.at(row)->getItemData().x,
                                            items.at(row)->getItemData().y,
                                            items.at(row)->getItemData().width,
                                            items.at(row)->getItemData().height));
        move(false);
        switch (items.at(row)->getItemData().typ) {
        case 0: val_db->setValue(prop_db,items.at(row)->getItemData().value.toInt()); break;
        case 1: val_txt->setValue(prop_txt,items.at(row)->getItemData().value.toString()); break;
        case 2: val_txt->setValue(prop_img,items.at(row)->getItemData().value.toString()); break;
        case 3: val_line->setValue(prop_line,items.at(row)->getItemData().value.toDouble()); break;
        }
        if (items.at(row)->getItemData().typ < 2) {
            itm_font->setValue(prop_font,items.at(row)->getItemData().font);
            val_align->setValue(prop_align,items.at(row)->getItemData().align);
        }
    }
}

void Tab_ED::selectRow(int row) {
    tbl_items->selectRow(row);
    enableDataWidget();
}

void Tab_ED::selectItem() {
    int row = tbl_items->currentIndex().row();
    scene->clearSelection();
    scene->selectItem(row);
    enableDataWidget();
}

void Tab_ED::setNewSizePos(QtProperty* property, QRectF rect) {
    _global::itemdata lst = items.at(tbl_items->currentRow())->getItemData();
    lst.x = rect.x();
    lst.y = rect.y();
    lst.width = rect.width();
    lst.height = rect.height();
    items.at(tbl_items->currentIndex().row())->updateItemData(lst);
    items.at(tbl_items->currentIndex().row())->setNewSizePos(rect.x(),rect.y(),rect.width(),rect.height());
}

void Tab_ED::setNewData() {
    QVariant newstring;
    int row = tbl_items->currentIndex().row();
    switch (items.at(row)->getItemData().typ) {
    case 0: newstring = val_db->value(prop_db); break;
    case 1: newstring = val_txt->value(prop_txt); break;
    case 2: newstring = val_txt->value(prop_img); break;
    case 3: newstring = val_line->value(prop_line); break;
    }
    items.at(row)->setNewData(newstring);
    _global::itemdata val = items.at(row)->getItemData();
    val.value = newstring;
    items.at(row)->updateItemData(val);
    QStringList fields = _global::getFields();
    QString text;
    switch (val.typ) {
    case 0: text = fields.at(val.value.toInt()); break;
    case 1:
    case 2: text = val.value.toString(); break;
    case 3: text = "Linienstärke: " + val.value.toString(); break;
    }
    tbl_items->item(row,1)->setText(text);
}

void Tab_ED::setNewFont() {
    QFont font(itm_font->value(prop_font));
    items.at(tbl_items->currentIndex().row())->setNewFont(font);
    _global::itemdata val = items.at(tbl_items->currentIndex().row())->getItemData();
    val.font = font;
    items.at(tbl_items->currentIndex().row())->updateItemData(val);
}

void Tab_ED::setNewAlign() {
    _global::itemdata val = items.at(tbl_items->currentIndex().row())->getItemData();
    val.align = val_align->value(prop_align);
    items.at(tbl_items->currentIndex().row())->updateItemData(val);
    items.at(tbl_items->currentIndex().row())->setNewAlign(val.align);
}

void Tab_ED::zoomIn() {
    double newScale = cmb_zoom->currentText().left(cmb_zoom->currentText().indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = graphicsView->matrix();
    graphicsView->resetMatrix();
    graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
    graphicsView->scale(newScale, newScale);
    graphicsView->setFixedSize(mmToPixel(210*newScale),mmToPixel(297*newScale));
    scene->setSceneRect(0,0,mmToPixel(210*newScale),mmToPixel(297*newScale));
}

void Tab_ED::sendToFront() {
    if (scene->selectedItems().isEmpty()) return;
    int j = scene->selectedItems().first()->zValue();
    for (int i=0;i<items.size();i++) {
        QGraphicsItem *itm = items.at(i);
        if (itm == scene->selectedItems().at(0)) {
            itm->setZValue(items.size()-1);
        } else {
            if (itm->zValue()>j)
                itm->setZValue(itm->zValue()-1);
        }
    }
}

void Tab_ED::sendToBack() {
    if (scene->selectedItems().isEmpty()) return;
    int j = scene->selectedItems().first()->zValue();
    for (int i=0;i<items.size();i++) {
        QGraphicsItem *itm = items.at(i);
        if (itm == scene->selectedItems().at(0)) {
            itm->setZValue(0);
        } else {
            if (itm->zValue()<j)
                itm->setZValue(itm->zValue()+1);
        }
    }
}

void Tab_ED::layerUp() {
    if (scene->selectedItems().isEmpty()) return;
    if (scene->selectedItems().first()->zValue() < items.size()-1) {
        int z = scene->selectedItems().first()->zValue();
        for (int i=0;i<items.size();i++) {
            if (items.at(i)->zValue() == z+1) {
                items.at(i)->setZValue(z);
                break;
            }
        }
        scene->selectedItems().first()->setZValue(z+1);
    }
}

void Tab_ED::layerDown() {
    if (scene->selectedItems().isEmpty()) return;
    if (scene->selectedItems().first()->zValue() > 0) {
        int z = scene->selectedItems().first()->zValue();
        for (int i=0;i<items.size();i++) {
            if (items.at(i)->zValue() == z-1) {
                items.at(i)->setZValue(z);
                break;
            }
        }
        scene->selectedItems().first()->setZValue(z-1);
    }
}

void Tab_ED::alignCenter() {
    if (scene->selectedItems().isEmpty()) return;
    _global::itemdata lst = items.at(tbl_items->currentRow())->getItemData();
    double x = (pixelToMM(scene->width())/2)-(lst.width/2);
    lst.x = x;
    items.at(tbl_items->currentIndex().row())->updateItemData(lst);
    items.at(tbl_items->currentIndex().row())->setNewSizePos(lst.x,lst.y,lst.width,lst.height);
    itm_size->setValue(prop_size,QRectF(lst.x,lst.y,lst.width,lst.height));
}

QList<EditorTextItem*> Tab_ED::sortItems(QList<EditorTextItem*> items) {
    for (int i=0;i<items.size();i++) {
        for (int j=0;j<(i+1);j++) {
            if (items.at(i)->zValue() < items.at(j)->zValue()) {
                items.swap(i,j);
            }
        }
    }
    return items;
}

