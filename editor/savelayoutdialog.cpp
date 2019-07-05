#include "savelayoutdialog.h"
#include "editorgraphicsitem.h"
#include "ui_savelayoutdialog.h"
#include <QMessageBox>
#include <QSqlQuery>

SaveLayoutDialog::SaveLayoutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SaveLayoutDialog)
{
    ui->setupUi(this);
    connect(ui->cmb_layout,SIGNAL(editTextChanged(QString)),this,SLOT(layoutSelectionChange()));
    connect(ui->bbx_done,SIGNAL(accepted()),this,SLOT(closeDialog()));
    connect(ui->bbx_done,SIGNAL(rejected()), this, SLOT(close()));
    QSqlQuery layoutListQuery;
    layoutListQuery.prepare("SELECT int_layoutid, var_name FROM tfx_layouts ORDER BY var_name");
    layoutListQuery.exec();
    while (layoutListQuery.next()) {
        ui->cmb_layout->addItem(layoutListQuery.value(1).toString(),layoutListQuery.value(0).toInt());
    }
}

SaveLayoutDialog::~SaveLayoutDialog()
{
    delete ui;
}

void SaveLayoutDialog::closeDialog()
{
    int layoutid=-1;
    QSqlQuery layoutQuery;
    layoutQuery.prepare("SELECT int_layoutid FROM tfx_layouts WHERE var_name=?");
    layoutQuery.bindValue(0,ui->cmb_layout->currentText());
    layoutQuery.exec();
    if (_global::querySize(layoutQuery)>0) {
        QMessageBox msg(QMessageBox::Warning, "Layout existiert bereits", "Ein Layout mit diesem Namen existiert bereits. Soll es überschrieben werden?",QMessageBox::Yes | QMessageBox::No);
        int ret = msg.exec();
        if (ret == QMessageBox::No) {
            return;
        } else if (ret == QMessageBox::Yes) {
            layoutQuery.next();
            layoutid = layoutQuery.value(0).toInt();
        }
    }
    QSqlQuery createLayout;
    if (layoutid==-1) {
        createLayout.prepare("INSERT INTO tfx_layouts (var_name,txt_comment) VALUES (?,?)");
    } else {
        createLayout.prepare("UPDATE tfx_layouts SET var_name=?, txt_comment=? WHERE int_layoutid=?");
        createLayout.bindValue(2,layoutid);
    }
    createLayout.bindValue(0,ui->cmb_layout->currentText());
    createLayout.bindValue(1,ui->txt_comment->toPlainText());
    createLayout.exec();
    if (layoutid == -1) {
        if (_global::getDBTyp() == 0) {
            QSqlQuery getLayoutID("SELECT last_value FROM tfx_layouts_int_layoutid_seq");
            getLayoutID.next();
            layoutid = getLayoutID.value(0).toInt();
        } else {
            layoutid = createLayout.lastInsertId().toInt();
        }
    }
    QList<int> ids;
    for (int i=0;i<items.size();i++) {
        _global::itemdata data = items.at(i)->getItemData();
        bool insert = true;
        if (data.id > 0) {
            QSqlQuery getLayoutID;
            getLayoutID.prepare("SELECT int_layoutid FROM tfx_layout_felder WHERE int_layout_felderid=?");
            getLayoutID.bindValue(0,items.at(i)->getItemData().id);
            getLayoutID.exec();
            getLayoutID.next();
            if (layoutid == getLayoutID.value(0).toInt()) {
                insert = false;
                ids.append(items.at(i)->getItemData().id);
            }
        }
        QSqlQuery insertLayoutFeld;
        if (insert) {
            insertLayoutFeld.prepare("INSERT INTO tfx_layout_felder (int_typ,var_font,rel_x,rel_y,rel_w,rel_h,var_value,int_align,int_layer,int_layoutid) VALUES (?,?,?,?,?,?,?,?,?,?)");
            insertLayoutFeld.bindValue(9,layoutid);
        } else {
            insertLayoutFeld.prepare("UPDATE tfx_layout_felder SET int_typ=?, var_font=?, rel_x=?, rel_y=?, rel_w=?, rel_h=?, var_value=?, int_align=?, int_layer=? WHERE int_layout_felderid=?");
            insertLayoutFeld.bindValue(9,data.id);
        }
        insertLayoutFeld.bindValue(0,data.typ);
        insertLayoutFeld.bindValue(1,data.font.toString());
        insertLayoutFeld.bindValue(2,data.x);
        insertLayoutFeld.bindValue(3,data.y);
        insertLayoutFeld.bindValue(4,data.width);
        insertLayoutFeld.bindValue(5,data.height);
        insertLayoutFeld.bindValue(6,data.value);
        insertLayoutFeld.bindValue(7,data.align);
        insertLayoutFeld.bindValue(8,items.at(i)->zValue());
        insertLayoutFeld.exec();
        if (insert) {
            if (_global::getDBTyp()==0) {
                QSqlQuery getLayoutFeldID("SELECT last_value FROM tfx_layout_felder_int_layout_felderid_seq");
                getLayoutFeldID.next();
                data.id = getLayoutFeldID.value(0).toInt();
            } else {
                data.id = insertLayoutFeld.lastInsertId().toInt();
            }
            items.at(i)->updateItemData(data);
            ids.append(data.id);
        } else {
            continue;
        }
    }
    QSqlQuery deleteOldFields;
    deleteOldFields.prepare("DELETE FROM tfx_layout_felder WHERE int_layoutid=? AND int_layout_felderid NOT IN ("+_global::intListToString(ids)+")");
    deleteOldFields.bindValue(0,layoutid);
    deleteOldFields.exec();
    done(1);
}

void SaveLayoutDialog::layoutSelectionChange()
{
    QSqlQuery getCommentQuery;
    getCommentQuery.prepare("SELECT txt_comment FROM tfx_layouts WHERE var_name=?");
    getCommentQuery.bindValue(0,ui->cmb_layout->currentText());
    getCommentQuery.exec();
    if (_global::querySize(getCommentQuery)>0) {
        getCommentQuery.next();
        ui->txt_comment->setText(getCommentQuery.value(0).toString());
    } else {
        ui->txt_comment->setText("");
    }
}

void SaveLayoutDialog::setItems(QList<EditorGraphicsItem *> itms)
{
    items = itms;
}

QList<EditorGraphicsItem *> SaveLayoutDialog::getItems()
{
    return items;
}
