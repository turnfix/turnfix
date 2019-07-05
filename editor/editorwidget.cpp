#include "editorwidget.h"
#include "editorgraphicsitem.h"
#include "editorgraphicsscene.h"
#include "savelayoutdialog.h"
#include "selectlayoutdialog.h"
#include "ui_editorwidget.h"
#include <QDomDocument>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QPainter>
#include <QScrollArea>
#include <QSqlQuery>
#include <QTextStream>
#include <QtGroupPropertyManager>
#include <QtSpinBoxFactory>
#include <QtTreePropertyBrowser>

QList<EditorGraphicsItem *> EditorWidget::items = QList<EditorGraphicsItem *>();

EditorWidget::EditorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditorWidget)
{
    ui->setupUi(this);

    ui->cmb_zoom->addItem("25%");
    ui->cmb_zoom->addItem("50%");
    ui->cmb_zoom->addItem("75%");
    ui->cmb_zoom->addItem("100%");
    ui->cmb_zoom->addItem("125%");
    ui->cmb_zoom->addItem("150%");
    ui->cmb_zoom->addItem("175%");
    ui->cmb_zoom->addItem("200%");
    ui->cmb_zoom->setCurrentIndex(3);

    p = new QPrinter(QPrinter::ScreenResolution);
    p->setPaperSize(QPrinter::A4);
    scene = new EditorGraphicsScene();
    scene->setBackgroundBrush(Qt::white);
    ui->sa->setBackgroundRole(QPalette::Dark);
    ui->graphicsView->setFixedSize(mmToPixel(210), mmToPixel(297));
    scene->setSceneRect(0,0,mmToPixel(210),mmToPixel(297));
    ui->graphicsView->setScene(scene);
    QStringList header;
    header << "Typ" << "Inhalt";
    ui->tbl_items->setHorizontalHeaderLabels(header);
    ui->tbl_items->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tbl_items->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    itm_size = new QtRectFPropertyManager(ui->wdg_props);
    itm_font = new QtFontPropertyManager(ui->wdg_props);
    val_db = new QtEnumPropertyManager(ui->wdg_props);
    val_align = new QtEnumPropertyManager(ui->wdg_props);
    val_txt = new QtStringPropertyManager(ui->wdg_props);
    val_line = new QtDoublePropertyManager(ui->wdg_props);

    prop_size = itm_size->addProperty("Maße");
    prop_font = itm_font->addProperty("Schriftart");

    //Alignment
    prop_align = val_align->addProperty("Ausrichtung");
    QStringList alignments;
    alignments <<  "zentriert" << "links" << "rechts";
    val_align->setEnumNames(prop_align,alignments);

    //Properties Database
    man_db = new QtGroupPropertyManager(ui->wdg_props);
    prop_db_root = man_db->addProperty("Datenbankelement");
    prop_db = val_db->addProperty("DB-Feld");
    QStringList fields = _global::getFields();
    val_db->setEnumNames(prop_db,fields);
    prop_db_root->addSubProperty(prop_size);
    prop_db_root->addSubProperty(prop_font);
    prop_db_root->addSubProperty(prop_db);
    prop_db_root->addSubProperty(prop_align);

    //Properties Text
    man_txt = new QtGroupPropertyManager(ui->wdg_props);
    prop_txt_root = man_txt->addProperty("Textelement");
    prop_txt = val_txt->addProperty("Text");
    prop_txt_root->addSubProperty(prop_size);
    prop_txt_root->addSubProperty(prop_font);
    prop_txt_root->addSubProperty(prop_txt);
    prop_txt_root->addSubProperty(prop_align);

    //Properties Grafik
    man_img = new QtGroupPropertyManager(ui->wdg_props);
    prop_img_root = man_txt->addProperty("Grafik");
    prop_img = val_txt->addProperty("Datei");
    prop_img_root->addSubProperty(prop_size);
    prop_img_root->addSubProperty(prop_img);

    //Properties Linie
    man_line = new QtGroupPropertyManager(ui->wdg_props);
    prop_line_root = man_txt->addProperty("Grafik");
    prop_line = val_line->addProperty("Stärke");
    prop_line_root->addSubProperty(prop_size);
    prop_line_root->addSubProperty(prop_line);
    val_line->setMinimum(prop_line,0.0);
    val_line->setSingleStep(prop_line, 0.01);

    QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(ui->wdg_props);
    QtEnumEditorFactory *enumFactory = new QtEnumEditorFactory(ui->wdg_props);
    QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(ui->wdg_props);
    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(ui->wdg_props);
    QtFontEditorFactory *fontFactory = new QtFontEditorFactory(ui->wdg_props);
    QtLineEditFactory *lineEditFactory = new QtLineEditFactory(ui->wdg_props);

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
    ui->wdg_props->layout()->addWidget(prop);

    connect(ui->but_open, SIGNAL(clicked()), this, SLOT(load()));
    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_reset, SIGNAL(clicked()), this, SLOT(removeAllItems()));
    connect(ui->but_front, SIGNAL(clicked()), this, SLOT(sendToFront()));
    connect(ui->but_back, SIGNAL(clicked()), this, SLOT(sendToBack()));
    connect(ui->but_moveup, SIGNAL(clicked()), this, SLOT(layerUp()));
    connect(ui->but_movedown, SIGNAL(clicked()), this, SLOT(layerDown()));
    connect(ui->but_acenter, SIGNAL(clicked()), this, SLOT(alignCenter()));
    connect(ui->tbl_items, SIGNAL(clicked(QModelIndex)), this, SLOT(selectItem()));
    connect(scene, SIGNAL(move(bool)), this, SLOT(move(bool)));
    connect(scene, SIGNAL(updateSize(int,QRectF)), this, SLOT(sizePosChanged(int,QRectF)));
    connect(scene, SIGNAL(selectItemInList(int)), this, SLOT(selectRow(int)));
    connect(itm_size, SIGNAL(valueChanged(QtProperty*,QRectF)), this, SLOT(setNewSizePos(QtProperty*,QRectF)));
    connect(itm_font, SIGNAL(valueChanged(QtProperty*,QFont)), this, SLOT(setNewFont()));
    connect(val_db, SIGNAL(valueChanged(QtProperty*,int)), this, SLOT(setNewData()));
    connect(val_txt, SIGNAL(valueChanged(QtProperty*,QString)), this, SLOT(setNewData()));
    connect(val_line, SIGNAL(valueChanged(QtProperty*,double)), this, SLOT(setNewData()));
    connect(val_align, SIGNAL(valueChanged(QtProperty*,int)), this, SLOT(setNewAlign()));
    connect(ui->but_add, SIGNAL(clicked()), this, SLOT(addField()));
    connect(ui->but_del, SIGNAL(clicked()), this, SLOT(delField()));
    connect(ui->cmb_zoom, SIGNAL(currentIndexChanged(int)), this, SLOT(zoomIn()));
}

EditorWidget::~EditorWidget()
{
    delete ui;
}

void EditorWidget::initPreview(QList<_global::itemdata> val)
{
    for (int i=0;i<val.size();i++) {
        addNewItem(val.at(i));
    }
}

QList<_global::itemdata> EditorWidget::getItems()
{
    QList<_global::itemdata> lst;
    for (int i=0;i<items.size();i++) {
        lst.append(items.at(i)->getItemData());
    }
    return lst;
}

QList<EditorGraphicsItem *> EditorWidget::getEditorItems()
{
    return items;
}

void EditorWidget::addNewItem(_global::itemdata lst)
{
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
    ui->tbl_items->insertRow(row);
    ui->tbl_items->setItem(row, 0, new QTableWidgetItem(types.at(lst.typ)));
    ui->tbl_items->setItem(row, 1, new QTableWidgetItem(text));
    ui->tbl_items->horizontalHeader()->reset();
}

void EditorWidget::addField()
{
    _global::itemdata lst
        = {-1, ui->cmb_typ->currentIndex(), 0.0, 0.0, 45.0, 35.0, QFont("Tahoma", 12), 0, 0};
    addNewItem(lst);
}

void EditorWidget::delField()
{
    if (scene->selectedItems().isEmpty()) return;
    scene->removeTurnFixItem(ui->tbl_items->currentRow());
    scene->update(scene->sceneRect());
    ui->tbl_items->removeRow(ui->tbl_items->currentRow());
    if (ui->tbl_items->rowCount() > 0) {
        selectRow(0);
    }
}

void EditorWidget::load()
{
    SelectLayoutDialog *sel = new SelectLayoutDialog();
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

void EditorWidget::removeAllItems()
{
    scene->removeAllItems();
    scene->update(scene->sceneRect());
    prop->clear();
    int rows = ui->tbl_items->rowCount();
    for (int i = 0; i < rows; i++)
        ui->tbl_items->removeRow(0);
}

void EditorWidget::save()
{
    SaveLayoutDialog *sld = new SaveLayoutDialog();
    sld->setItems(items);
    if (sld->exec() == 1) {
        items = sld->getItems();
    }
}

void EditorWidget::sizePosChanged(int row, QRectF r)
{
    if (ui->tbl_items->currentRow() != -1) {
        itm_size->setValue(prop_size,QRectF(r.x(), r.y(), r.width(),r.height()));
    }
}

void EditorWidget::move(bool moving)
{
    if (moving) {
        disconnect(itm_size, nullptr, nullptr, nullptr);
    } else {
        connect(itm_size, SIGNAL(valueChanged(QtProperty*,QRectF)), this, SLOT(setNewSizePos(QtProperty*,QRectF)));
    }
}

double EditorWidget::pixelToMM(int px)
{
    double mm = (double)px * (double)p->widthMM() / (double)p->width();
    return mm;
}

int EditorWidget::mmToPixel(double mm)
{
    int px = (double)p->width() * mm / (double)p->widthMM();
    return px;
}

void EditorWidget::enableDataWidget()
{
    if (prop->properties().size()>0) prop->removeProperty(prop->properties().at(0));
    if (ui->tbl_items->currentIndex().isValid()) {
        int row = ui->tbl_items->currentIndex().row();
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

void EditorWidget::selectRow(int row)
{
    ui->tbl_items->selectRow(row);
    enableDataWidget();
}

void EditorWidget::selectItem()
{
    int row = ui->tbl_items->currentIndex().row();
    scene->clearSelection();
    scene->selectItem(row);
    enableDataWidget();
}

void EditorWidget::setNewSizePos(QtProperty *property, QRectF rect)
{
    _global::itemdata lst = items.at(ui->tbl_items->currentRow())->getItemData();
    lst.x = rect.x();
    lst.y = rect.y();
    lst.width = rect.width();
    lst.height = rect.height();
    items.at(ui->tbl_items->currentIndex().row())->updateItemData(lst);
    items.at(ui->tbl_items->currentIndex().row())
        ->setNewSizePos(rect.x(), rect.y(), rect.width(), rect.height());
}

void EditorWidget::setNewData()
{
    QVariant newstring;
    int row = ui->tbl_items->currentIndex().row();
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
    ui->tbl_items->item(row, 1)->setText(text);
}

void EditorWidget::setNewFont()
{
    QFont font(itm_font->value(prop_font));
    items.at(ui->tbl_items->currentIndex().row())->setNewFont(font);
    _global::itemdata val = items.at(ui->tbl_items->currentIndex().row())->getItemData();
    val.font = font;
    items.at(ui->tbl_items->currentIndex().row())->updateItemData(val);
}

void EditorWidget::setNewAlign()
{
    _global::itemdata val = items.at(ui->tbl_items->currentIndex().row())->getItemData();
    val.align = val_align->value(prop_align);
    items.at(ui->tbl_items->currentIndex().row())->updateItemData(val);
    items.at(ui->tbl_items->currentIndex().row())->setNewAlign(val.align);
}

void EditorWidget::zoomIn()
{
    double newScale
        = ui->cmb_zoom->currentText().left(ui->cmb_zoom->currentText().indexOf(tr("%"))).toDouble()
          / 100.0;
    QMatrix oldMatrix = ui->graphicsView->matrix();
    ui->graphicsView->resetMatrix();
    ui->graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
    ui->graphicsView->scale(newScale, newScale);
    ui->graphicsView->setFixedSize(mmToPixel(210 * newScale), mmToPixel(297 * newScale));
    scene->setSceneRect(0,0,mmToPixel(210*newScale),mmToPixel(297*newScale));
}

void EditorWidget::sendToFront()
{
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

void EditorWidget::sendToBack()
{
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

void EditorWidget::layerUp()
{
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

void EditorWidget::layerDown()
{
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

void EditorWidget::alignCenter()
{
    if (scene->selectedItems().isEmpty()) return;
    _global::itemdata lst = items.at(ui->tbl_items->currentRow())->getItemData();
    double x = (pixelToMM(scene->width())/2)-(lst.width/2);
    lst.x = x;
    items.at(ui->tbl_items->currentIndex().row())->updateItemData(lst);
    items.at(ui->tbl_items->currentIndex().row())->setNewSizePos(lst.x, lst.y, lst.width, lst.height);
    itm_size->setValue(prop_size,QRectF(lst.x,lst.y,lst.width,lst.height));
}

QList<EditorGraphicsItem *> EditorWidget::sortItems(QList<EditorGraphicsItem *> items)
{
    for (int i=0;i<items.size();i++) {
        for (int j=0;j<(i+1);j++) {
            if (items.at(i)->zValue() < items.at(j)->zValue()) {
                items.swapItemsAt(i,j);
            }
        }
    }
    return items;
}
