#include "disciplinegroupdialog.h"
#include "src/global/header/_global.h"
#include "ui_disciplinegroupdialog.h"
#include <QSqlQuery>
#include <QStandardItemModel>

DisciplineGroupDialog::DisciplineGroupDialog(int im_dgid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DisciplineGroupDialog)
{
    dgid = im_dgid;
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    model_all = new QStandardItemModel();
    model_group = new QStandardItemModel();
    model_all->setColumnCount(5);
    model_group->setColumnCount(5);
    ui->tbl_all->setModel(model_all);
    ui->tbl_group->setModel(model_group);
    QHeaderView::ResizeMode resizeMode[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resize[] = {24,300,250,40,0};
    QStringList headers;
    headers << "" << "Sport" << "Disziplin" << "m/w" << "";
    for (int i=0;i<5;i++) {
        ui->tbl_all->horizontalHeader()->setSectionResizeMode(i, resizeMode[i]);
        ui->tbl_all->horizontalHeader()->resizeSection(i, resize[i]);
        ui->tbl_group->horizontalHeader()->setSectionResizeMode(i, resizeMode[i]);
        ui->tbl_group->horizontalHeader()->resizeSection(i, resize[i]);
        model_all->setHeaderData(i, Qt::Horizontal, headers.at(i));
        model_group->setHeaderData(i, Qt::Horizontal, headers.at(i));
    }

    QSqlQuery getDisziplinen;
    getDisziplinen.prepare("SELECT int_disziplinenid, tfx_sport.var_name, tfx_disziplinen.var_name, var_icon, CASE WHEN bol_m = 'true' AND bol_w = 'true' THEN 'm/w' ELSE CASE WHEN bol_m = 'true' THEN 'm' ELSE 'w' END END FROM tfx_disziplinen INNER JOIN tfx_sport USING (int_sportid) ORDER BY tfx_sport.var_name");
    getDisziplinen.exec();
    while (getDisziplinen.next()) {
        QStandardItem *icon = new QStandardItem();
        icon->setIcon(QIcon(getDisziplinen.value(3).toString()));
        icon->setEditable(false);
        model_all->setItem(getDisziplinen.at(),0,icon);
        QStandardItem *sport = new QStandardItem(getDisziplinen.value(1).toString());
        sport->setEditable(false);
        model_all->setItem(getDisziplinen.at(),1,sport);
        QStandardItem *name = new QStandardItem(getDisziplinen.value(2).toString());
        name->setEditable(false);
        model_all->setItem(getDisziplinen.at(),2,name);
        QStandardItem *sex = new QStandardItem(getDisziplinen.value(4).toString());
        sex->setEditable(false);
        sex->setTextAlignment(Qt::AlignCenter);
        model_all->setItem(getDisziplinen.at(),3,sex);
        model_all->setItem(getDisziplinen.at(),4,new QStandardItem(getDisziplinen.value(0).toString()));
    }
    if (dgid > 0) {
        QSqlQuery disGroupInfo;
        disGroupInfo.prepare("SELECT var_name, txt_comment FROM tfx_disziplinen_gruppen WHERE int_disziplinen_gruppenid=?");
        disGroupInfo.bindValue(0,dgid);
        disGroupInfo.exec();
        disGroupInfo.next();
        ui->txt_name->setText(disGroupInfo.value(0).toString());
        ui->txt_comment->setText(disGroupInfo.value(1).toString());
        QSqlQuery disziplinen;
        disziplinen.prepare("SELECT int_disziplinenid FROM tfx_disgrp_x_disziplinen WHERE int_disziplinen_gruppenid=? ORDER BY int_pos");
        disziplinen.bindValue(0,dgid);
        disziplinen.exec();
        while (disziplinen.next()) {
            int row = model_all->findItems(disziplinen.value(0).toString(),Qt::MatchExactly,4).at(0)->row();
            model_group->insertRow(model_group->rowCount(),model_all->takeRow(row));
        }
    }
    connect(ui->but_add,SIGNAL(clicked()),this,SLOT(addItem()));
    connect(ui->but_del,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(ui->but_up, SIGNAL(clicked()),this,SLOT(moveUp()));
    connect(ui->but_down, SIGNAL(clicked()), this, SLOT(moveDown()));
    connect(ui->bbx_done, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui->bbx_done, SIGNAL(accepted()), this, SLOT(save()));
}

DisciplineGroupDialog::~DisciplineGroupDialog()
{
    delete ui;
}

void DisciplineGroupDialog::addItem()
{
    if (ui->tbl_all->selectionModel()->selectedIndexes().size()==0) return;
    model_group->insertRow(model_group->rowCount(),model_all->takeRow(ui->tbl_all->currentIndex().row()));
}

void DisciplineGroupDialog::removeItem()
{
    if (ui->tbl_group->selectionModel()->selectedIndexes().size()==0) return;
    model_all->insertRow(model_all->rowCount(),model_group->takeRow(ui->tbl_group->currentIndex().row()));
}

void DisciplineGroupDialog::moveUp()
{
    if (ui->tbl_group->currentIndex().isValid() && ui->tbl_group->currentIndex().row() > 0) {
        int insert = ui->tbl_group->currentIndex().row()-1;
        model_group->insertRow(insert,model_group->takeRow(ui->tbl_group->currentIndex().row()));
        ui->tbl_group->selectRow(insert);
    }
}

void DisciplineGroupDialog::moveDown()
{
    if (ui->tbl_group->currentIndex().isValid() && ui->tbl_group->currentIndex().row() < (model_group->rowCount()-1)) {
        int insert = ui->tbl_group->currentIndex().row()+1;
        model_group->insertRow(insert,model_group->takeRow(ui->tbl_group->currentIndex().row()));
        ui->tbl_group->selectRow(insert);
    }
}

void DisciplineGroupDialog::save()
{
    QSqlQuery saveQuery;
    if (dgid>0) {
        saveQuery.prepare("UPDATE tfx_disziplinen_gruppen SET var_name=?, txt_comment=? WHERE int_disziplinen_gruppenid=?");
        saveQuery.bindValue(2,dgid);
    } else {
        saveQuery.prepare("INSERT INTO tfx_disziplinen_gruppen (var_name,txt_comment) VALUES (?,?)");
    }
    saveQuery.bindValue(0,ui->txt_name->text());
    saveQuery.bindValue(1,ui->txt_comment->text());
    saveQuery.exec();
    if (dgid == 0) {
        if (_global::getDBTyp() == 0) {
            QSqlQuery getGroupID("SELECT last_value FROM tfx_disziplinen_gruppen_int_disziplinen_gruppenid_seq");
            getGroupID.next();
            dgid = getGroupID.value(0).toInt();
        } else {
            dgid = saveQuery.lastInsertId().toInt();
        }
    }
    QList<int> ids;
    for (int i=0;i<model_group->rowCount();i++) {
        int disid = model_group->item(i,4)->text().toInt();
        QSqlQuery checkID;
        checkID.prepare("SELECT int_disgrp_x_disziplinenid FROM tfx_disgrp_x_disziplinen WHERE int_disziplinen_gruppenid=? AND int_disziplinenid=?");
        checkID.bindValue(0,dgid);
        checkID.bindValue(1,disid);
        checkID.exec();
        QSqlQuery fieldUpdate;
        if (_global::querySize(checkID)>0) {
            checkID.next();
            fieldUpdate.prepare("UPDATE tfx_disgrp_x_disziplinen SET int_pos=? WHERE int_disgrp_x_disziplinenid=?");
            fieldUpdate.bindValue(1,checkID.value(0).toInt());
        } else {
            fieldUpdate.prepare("INSERT INTO tfx_disgrp_x_disziplinen (int_pos,int_disziplinen_gruppenid,int_disziplinenid) VALUES (?,?,?)");
            fieldUpdate.bindValue(1,dgid);
            fieldUpdate.bindValue(2,disid);
        }
        fieldUpdate.bindValue(0,i);
        fieldUpdate.exec();
        if (_global::querySize(checkID)>0) {
            ids.append(checkID.value(0).toInt());
        } else {
            if (_global::getDBTyp() == 0) {
                QSqlQuery getFieldID("SELECT last_value FROM tfx_disgrp_x_disziplinen_int_disgrp_x_disziplinenid_seq");
                getFieldID.next();
                ids.append(getFieldID.value(0).toInt());
            } else {
                ids.append(fieldUpdate.lastInsertId().toInt());
            }
        }
    }
    QSqlQuery deleteOldFields;
    deleteOldFields.prepare("DELETE FROM tfx_disgrp_x_disziplinen WHERE int_disziplinen_gruppenid=? AND int_disgrp_x_disziplinenid NOT IN ("+_global::intListToString(ids)+")");
    deleteOldFields.bindValue(0,dgid);
    deleteOldFields.exec();
    done(1);
}
