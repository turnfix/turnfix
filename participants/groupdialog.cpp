#include "groupdialog.h"
#include "masterdata/athletedialog.h"
#include "masterdata/clubdialog.h"
#include "model/entity/athlete.h"
#include "model/entity/club.h"
#include "model/entity/event.h"
#include "model/entitymanager.h"
#include "src/global/header/_global.h"
#include "src/global/header/settings.h"
#include "ui_groupdialog.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QToolBar>

GroupDialog::GroupDialog(Event *event, EntityManager *em, int edit, QWidget *parent)
    : QDialog(parent)
    , m_em(em)
    , ui(new Ui::GroupDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    this->editid = edit;
    this->m_event = event;

    QToolBar *tb = new QToolBar();
    QActionGroup *ag = new QActionGroup(this);
    tb->setAllowedAreas(Qt::LeftToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setOrientation(Qt::Vertical);
    tb->addAction(ui->act_group);
    ag->addAction(ui->act_group);
    tb->addAction(ui->act_tn);
    ag->addAction(ui->act_tn);
    tb->addAction(ui->act_dis);
    ag->addAction(ui->act_dis);
    ui->act_group->setChecked(true);
    ui->sidebar->layout()->addWidget(tb);

    connect(ui->act_group, &QAction::triggered, [this](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->act_tn, &QAction::triggered, [this](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->act_dis, &QAction::triggered, [this](){
        ui->stackedWidget->setCurrentIndex(2);
    });

    model = new QStandardItemModel();
    model->setColumnCount(3);
    model2 = new QSqlQueryModel();
    ui->tbl_tn->setModel(model);
    ui->tbl_avtn->setModel(model2);
    QHeaderView::ResizeMode resizeMode[] = {QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::Fixed};
    int resize[] = {200,40,0};
    QString headers[3] = {"Name","Geb.","id"};
    for (int i=0;i<3;i++) {
        model->setHeaderData(i, Qt::Horizontal, headers[i]);
        ui->tbl_tn->horizontalHeader()->setSectionResizeMode(i, resizeMode[i]);
        ui->tbl_tn->horizontalHeader()->resizeSection(i, resize[i]);
    }
    QSqlQuery query2;
    query2.prepare("SELECT int_statusid, var_name, ary_colorcode FROM tfx_status WHERE bol_karte='true' ORDER BY int_statusid ASC");
    query2.exec();
    while(query2.next()) {
        ui->cmb_status->addItem(query2.value(1).toString(),query2.value(0).toInt());
        QList<int> color = _global::splitColorArray(query2.value(2).toString());
        ui->cmb_status->setItemData(ui->cmb_status->count()-1,QColor(color.at(0),color.at(1),color.at(2)),Qt::BackgroundColorRole);
    }
    ui->chk_club->setChecked(Settings::clubFilter);
    ui->chk_planned->setChecked(Settings::usedFilter);
    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_add, SIGNAL(clicked()), this, SLOT(addTn()));
    connect(ui->but_del, SIGNAL(clicked()), this, SLOT(removeTn()));
    connect(ui->but_add, SIGNAL(clicked()), this, SLOT(fillTable2()));
    connect(ui->but_del, SIGNAL(clicked()), this, SLOT(fillTable2()));
    connect(ui->chk_club, SIGNAL(clicked()), this, SLOT(fillTable2()));
    connect(ui->chk_planned, SIGNAL(clicked()), this, SLOT(fillTable2()));
    connect(ui->but_TNadd, SIGNAL(clicked()), this, SLOT(addAv()));
    connect(ui->cmb_wk, SIGNAL(currentIndexChanged(int)), this, SLOT(checkDisziplinen()));
    connect(ui->cmb_club, SIGNAL(currentIndexChanged(int)), this, SLOT(fillTable2()));
    connect(ui->but_TNedit, SIGNAL(clicked()), this, SLOT(editAv()));
    connect(this, SIGNAL(finished(int)), this, SLOT(saveWindowState()));
    connect(ui->but_addclub, SIGNAL(clicked()), this, SLOT(addClub()));
    initData();
}

GroupDialog::~GroupDialog()
{
    delete ui;
}

void GroupDialog::saveWindowState() {
    Settings::updateFiler(ui->chk_club->isChecked(), ui->chk_planned->isChecked());
}

void GroupDialog::initData() {
    updateClubs();
    QSqlQuery query2;
    query2.prepare("SELECT int_wettkaempfeid, var_nummer, var_name FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? AND int_typ=2 ORDER BY var_nummer ASC");
    query2.bindValue(0, this->m_event->mainEvent()->id());
    query2.exec();
    while (query2.next()) {
        ui->cmb_wk->addItem(query2.value(1).toString() + " " + query2.value(2).toString(),query2.value(0).toInt());
    }
    if (editid != 0) {
        QSqlQuery query;
        query.prepare("SELECT tfx_gruppen.int_vereineid, tfx_gruppen.var_name, int_wettkaempfeid, var_riege, int_statusid, bol_ak, bol_startet_nicht FROM tfx_wertungen INNER JOIN tfx_gruppen USING (int_gruppenid) WHERE int_gruppenid=? LIMIT 1");
        query.bindValue( 0, editid );
        query.exec();
        query.next();
        ui->cmb_club->setCurrentIndex(ui->cmb_club->findData(query.value(0).toInt()));
        ui->txt_name->setText(query.value(1).toString());
        ui->cmb_wk->setCurrentIndex(ui->cmb_wk->findData(query.value(2).toInt()));
        ui->txt_rg->setText(query.value(3).toString());
        ui->cmb_status->setCurrentIndex(ui->cmb_status->findData(query.value(4).toInt()));
        ui->chk_ak->setChecked(query.value(5).toBool());
        ui->chk_miss->setChecked(query.value(6).toBool());
        QSqlQuery query8;
        query8.prepare("SELECT int_teilnehmerid FROM tfx_gruppen_x_teilnehmer WHERE int_gruppenid=?");
        query8.bindValue(0,editid);
        query8.exec();
        while (query8.next()) {
            lst_int_ids.append(query8.value(0).toInt());
        }
    }
    checkDisziplinen();
    fillTable();
    fillTable2();
}

void GroupDialog::save() {
    int vid=0;
    QSqlQuery query;
    query.prepare("SELECT int_vereineid FROM tfx_vereine WHERE var_name=?");
    query.bindValue(0, ui->cmb_club->currentText());
    query.exec();
    if (_global::querySize(query) > 0) {
        query.next();
        vid = query.value(0).toInt();
    } else {
        QSqlQuery query2;
        query2.prepare("INSERT INTO tfx_vereine (var_name) VALUES (?)");
        query2.bindValue(0, ui->cmb_club->currentText());
        query2.exec();
        if (_global::getDBTyp() == 0) {
            QSqlQuery query3("SELECT last_value FROM tfx_vereine_int_vereineid_seq");
            query3.next();
            vid = query3.value(0).toInt();
        } else {
            vid = query2.lastInsertId().toInt();
        }
    }
    QSqlQuery query4;
    if (editid == 0) {
        query4.prepare("INSERT INTO tfx_gruppen (int_vereineid,var_name) VALUES(?,?)");
    } else {
        query4.prepare("UPDATE tfx_gruppen SET int_vereineid=?, var_name=? WHERE int_gruppenid=?");
        query4.bindValue( 2, editid );
    }
    query4.bindValue( 0, vid );
    query4.bindValue( 1, ui->txt_name->text() );
    query4.exec();
    int group;
    if (editid == 0) {
        if (_global::getDBTyp() == 0) {
            QSqlQuery query5("SELECT last_value FROM tfx_gruppen_int_gruppenid_seq");
            query5.next();
            group = query5.value(0).toInt();
        } else {
            group = query4.lastInsertId().toInt();
        }
    } else {
        group = editid;
    }
    QSqlQuery query6;
    if (editid == 0) {
        query6.prepare("INSERT INTO tfx_wertungen (int_wettkaempfeid,int_gruppenid,int_statusid,int_runde,int_startnummer,bol_ak,bol_startet_nicht,var_riege) VALUES (?,?,?,?,?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_wertungen SET int_wettkaempfeid=?, int_gruppenid=?, int_statusid=?, int_runde=?, int_startnummer=?, bol_ak=?, bol_startet_nicht=?, var_riege=? WHERE int_gruppenid=?");
        query6.bindValue(8, group);
    }
    query6.bindValue(0, ui->cmb_wk->itemData(ui->cmb_wk->currentIndex()));
    query6.bindValue(1, group);
    query6.bindValue(2, ui->cmb_status->itemData(ui->cmb_status->currentIndex()));
    query6.bindValue(3, this->m_event->round());
    if (editid == 0) {
        QSqlQuery query2;
        query2.prepare("SELECT MAX(int_startnummer) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid INNER JOIN tfx_vereine USING (int_vereineid) WHERE int_veranstaltungenid=? AND int_runde=?");
        query2.bindValue(0, this->m_event->id());
        query2.bindValue(1, this->m_event->round());
        query2.exec();
        query2.next();
        query6.bindValue(4,query2.value(0).toInt()+1);
    } else {
        QSqlQuery query2;
        query2.prepare("SELECT int_startnummer FROM tfx_wertungen WHERE int_gruppenid=?");
        query2.bindValue(0, group);
        query2.exec();
        query2.next();
        query6.bindValue(4,query2.value(0).toInt());
    }
    query6.bindValue(5, ui->chk_ak->isChecked());
    query6.bindValue(6, ui->chk_miss->isChecked());
    query6.bindValue(7, ui->txt_rg->text());
    query6.exec();
    int wert;
    if (editid == 0) {
        if (_global::getDBTyp() == 0) {
            QSqlQuery query5("SELECT last_value FROM tfx_wertungen_int_wertungenid_seq");
            query5.next();
            wert = query5.value(0).toInt();
        } else {
            wert = query6.lastInsertId().toInt();
        }
    } else {
        QSqlQuery query5;
        query5.prepare("SELECT int_wertungenid FROM tfx_wertungen WHERE int_gruppenid=?");
        query5.bindValue(0,group);
        query5.exec();
        query5.next();
        wert = query5.value(0).toInt();
    }
    bool all = true;
    for (int i=0;i<ui->lst_dis->count();i++) {
        if (ui->lst_dis->item(i)->checkState() == Qt::Unchecked) {
            all = false;
            break;
        }
    }
    if (!all) {
        for (int i=0;i<ui->lst_dis->count();i++) {
            QSqlQuery query7;
            query7.prepare("SELECT * FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=? AND int_disziplinenid=?");
            query7.bindValue(0,wert);
            query7.bindValue(1,ui->lst_dis->item(i)->data(Qt::UserRole).toInt());
            query7.exec();
            if (_global::querySize(query7) == 0 && ui->lst_dis->item(i)->checkState() == Qt::Checked) {
                QSqlQuery query8;
                query8.prepare("INSERT INTO tfx_wertungen_x_disziplinen (int_wertungenid,int_disziplinenid) VALUES(?,?)");
                query8.bindValue(0,wert);
                query8.bindValue(1,ui->lst_dis->item(i)->data(Qt::UserRole).toInt());
                query8.exec();
            }
        }
        QSqlQuery query9;
        query9.prepare("SELECT * FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=?");
        query9.bindValue(0,wert);
        query9.exec();
        while (query9.next()) {
            int test = 0;
            for (int i=0;i<ui->lst_dis->count();i++) {
                if (ui->lst_dis->item(i)->data(Qt::UserRole).toInt() == query9.value(2).toInt()){
                    QSqlQuery query10;
                    query10.prepare("SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=? AND int_disziplinenid=? LIMIT 1");
                    query10.bindValue(0,wert);
                    query10.bindValue(1,ui->lst_dis->item(i)->data(Qt::UserRole).toInt());
                    query10.exec();
                    query10.next();
                    if (_global::querySize(query10) > 0 && ui->lst_dis->item(i)->checkState() == Qt::Checked) {
                        test = 1;
                        break;
                    } else {
                        test = 0;
                    }
                }
            }
            if (test == 0) {
                QSqlQuery query11;
                query11.prepare("DELETE FROM tfx_wertungen_details WHERE int_wertungenid=? AND int_disziplinenid=?");
                query11.bindValue(0,wert);
                query11.bindValue(1,query9.value(2).toInt());
                query11.exec();
                query11.prepare("DELETE FROM tfx_wertungen_x_disziplinen WHERE int_wertungen_x_disziplinenid=?");
                query11.bindValue(0,query9.value(0).toInt());
                query11.exec();
            }
        }
        QSqlQuery query12;
        query12.prepare("DELETE FROM tfx_wertungen_details WHERE int_wertungenid=? AND int_disziplinenid NOT IN (SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=?)");
        query12.bindValue(0,wert);
        query12.bindValue(1,wert);
        query12.exec();
    } else {
        QSqlQuery query11;
        query11.prepare("DELETE FROM tfx_wertungen_x_disziplinen WHERE int_wertungen_x_disziplinenid=?");
        query11.bindValue(0,wert);
        query11.exec();
    }
    for (int i=0;i<model->rowCount();i++) {
        QSqlQuery query6;
        query6.prepare("SELECT int_gruppen_x_teilnehmerid FROM tfx_gruppen_x_teilnehmer WHERE int_gruppenid=? AND int_teilnehmerid=?");
        query6.bindValue(0, group);
        query6.bindValue(1, lst_int_ids.at(i));
        query6.exec();
        if (_global::querySize(query6) == 0) {
            QSqlQuery query7;
            query7.prepare("INSERT INTO tfx_gruppen_x_teilnehmer (int_gruppenid,int_teilnehmerid) VALUES (?,?)");
            query7.bindValue(0, group);
            query7.bindValue(1, lst_int_ids.at(i));
            query7.exec();
        }
    }
    QSqlQuery query8;
    query8.prepare("SELECT int_teilnehmerid, int_gruppen_x_teilnehmerid FROM tfx_gruppen_x_teilnehmer WHERE int_gruppenid=?");
    query8.bindValue(0, group);
    query8.exec();
    while (query8.next()) {
        if (!lst_int_ids.contains(query8.value(0).toInt())) {
            QSqlQuery query9;
            query9.prepare("DELETE FROM tfx_gruppen_x_teilnehmer WHERE int_gruppen_x_teilnehmerid=?");
            query9.bindValue(0,query8.value(1).toInt());
            query9.exec();
        }
    }
    done(1);
}

void GroupDialog::addTn() {
    QModelIndexList indexes = ui->tbl_avtn->selectionModel()->selectedRows();
    for (int i=0;i<indexes.count();i++) {
        if (lst_int_ids.indexOf(QVariant(model2->data(model2->index(indexes.at(i).row(),3))).toInt()) == -1) {
            lst_int_ids.append(QVariant(model2->data(model2->index(indexes.at(i).row(),3))).toInt());

        }
    }
    fillTable();
}

void GroupDialog::removeTn() {
    QModelIndexList indexes = ui->tbl_tn->selectionModel()->selectedRows();
    std::sort(indexes.begin(), indexes.end());
    for (int i=indexes.count()-1;i>=0;i--) {
        model->removeRow(indexes.at(i).row());
        lst_int_ids.removeAt(indexes.at(i).row());
    }
}

void GroupDialog::fillTable() {
    for (int i=0;i<lst_int_ids.size();i++) {
        QSqlQuery query;
        query.prepare("SELECT tfx_teilnehmer.var_nachname || ', ' || tfx_teilnehmer.var_vorname, tfx_teilnehmer.dat_geburtstag, tfx_teilnehmer.int_teilnehmerid FROM tfx_teilnehmer WHERE tfx_teilnehmer.int_teilnehmerid=? LIMIT 1");
        query.bindValue( 0, lst_int_ids.at(i) );
        query.exec();
        query.next();
        for (int j=0;j<3;j++) {
            model->setItem(i,j,new QStandardItem(query.value(j).toString()));
        }
    }
}

void GroupDialog::fillTable2() {
    QSqlQuery query2;
    query2.prepare("SELECT int_vereineid FROM tfx_vereine WHERE var_name = ?");
    query2.bindValue(0, ui->cmb_club->currentText());
    query2.exec();
    QString numbers;
    for (int i=0;i<lst_int_ids.size();i++) {
        numbers += QString::number(lst_int_ids.at(i));
        if (i<lst_int_ids.size()-1) numbers += ",";
    }
    QSqlQuery query3;
    QString query;
    query = "SELECT var_nachname || ', ' || var_vorname, "+_global::date("dat_geburtstag",2)+", tfx_vereine.var_name, int_teilnehmerid FROM tfx_teilnehmer INNER JOIN tfx_vereine USING (int_vereineid) ";
    if (ui->chk_club->isChecked() && ui->chk_planned->isChecked()) {
        query += "WHERE int_vereineid=? AND int_teilnehmerid NOT IN (SELECT int_teilnehmerid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_teilnehmerid IS NOT NULL AND int_gruppenid != "+QString::number(editid)+") ";
        if (lst_int_ids.size()>0) query += "AND int_teilnehmerid NOT IN (" + _global::intListToString(lst_int_ids) + ") ";
    } else if (ui->chk_club->isChecked() ) {
        query += "WHERE int_vereineid=? ";
    } else if (ui->chk_planned->isChecked() ) {
        query += "WHERE int_teilnehmerid NOT IN (SELECT int_teilnehmerid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_teilnehmerid IS NOT NULL AND int_gruppenid != "+QString::number(editid)+") ";
        if (lst_int_ids.size()>0) query += "AND int_teilnehmerid NOT IN (" + _global::intListToString(lst_int_ids) + ") ";
    }
    query += "ORDER BY tfx_vereine.var_name, var_nachname, var_vorname";
    query3.prepare(query);
    if (ui->chk_club->isChecked() && ui->chk_planned->isChecked()) {
        query2.next();
        query3.bindValue(0,query2.value(0).toInt());
        query3.bindValue(1, this->m_event->mainEvent()->id());
    } else if (ui->chk_club->isChecked() ) {
        query2.next();
        query3.bindValue(0,query2.value(0).toInt());
    } else {
        query3.bindValue(0, this->m_event->mainEvent()->id());
    }
    query3.exec();
    model2->setQuery(query3);
    QHeaderView::ResizeMode resizeMode2[] = {QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed};
    int resize2[] = {200,40,150,0};
    QString headers2[4] = {"Name","Geb.","Verein","id"};
    for (int i=0;i<4;i++) {
        model2->setHeaderData(i, Qt::Horizontal, headers2[i]);
        ui->tbl_avtn->horizontalHeader()->setSectionResizeMode(i, resizeMode2[i]);
        ui->tbl_avtn->horizontalHeader()->resizeSection(i, resize2[i]);
    }
}

void GroupDialog::addAv() {
    AthleteDialog *tu = new AthleteDialog(nullptr,
                                          qvariant_cast<Club *>(ui->cmb_club->currentData()),
                                          m_em,
                                          this);
    tu->exec();
    fillTable2();
}

void GroupDialog::editAv() {
    AthleteDialog *tu = new AthleteDialog(qvariant_cast<Athlete *>(
                                              model2->data(ui->tbl_avtn->currentIndex())),
                                          m_em,
                                          this);
    tu->exec();
    fillTable2();
}

void GroupDialog::checkDisziplinen() {
    ui->lst_dis->clear();
    QSqlQuery query;
    query.prepare("SELECT int_disziplinenid, var_name FROM tfx_disziplinen INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_disziplinenid) WHERE int_wettkaempfeid=? ORDER BY int_sportid, int_disziplinenid");
    query.bindValue(0, ui->cmb_wk->itemData(ui->cmb_wk->currentIndex()));
    query.exec();
    while (query.next()) {
        ui->lst_dis->addItem(query.value(1).toString());
        ui->lst_dis->item(ui->lst_dis->count()-1)->setData(Qt::UserRole,query.value(0).toInt());
        ui->lst_dis->item(ui->lst_dis->count()-1)->setCheckState(Qt::Unchecked);
    }
    if (editid != 0) {
        for (int i=0;i<ui->lst_dis->count();i++) {
            ui->lst_dis->item(i)->setCheckState(Qt::Unchecked);
        }
        QSqlQuery query2;
        query2.prepare("SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen INNER JOIN tfx_wertungen USING (int_wertungenid) WHERE int_gruppenid=?");
        query2.bindValue(0,editid);
        query2.exec();
        if (_global::querySize(query2) > 0) {
            while (query2.next()) {
                for (int i=0;i<ui->lst_dis->count();i++) {
                    if (ui->lst_dis->item(i)->data(Qt::UserRole).toInt() == query2.value(0).toInt()) {
                        ui->lst_dis->item(i)->setCheckState(Qt::Checked);
                        break;
                    }
                }
            }
        } else {
            for (int i=0;i<ui->lst_dis->count();i++) {
                ui->lst_dis->item(i)->setCheckState(Qt::Checked);
            }
        }
    }
}

void GroupDialog::updateClubs() {
    QString currtext = ui->cmb_club->currentText();
    ui->cmb_club->clear();
    QSqlQuery query;
    query.prepare("SELECT int_vereineid, var_name FROM tfx_vereine ORDER BY var_name");
    query.exec();
    while (query.next()) {
        ui->cmb_club->addItem(query.value(1).toString(),query.value(0).toInt());
    }
    ui->cmb_club->setCurrentIndex(ui->cmb_club->findText(currtext));
}

void GroupDialog::addClub() {
    // TODO reanable
    //ClubDialog *pe = new ClubDialog(nullptr, m_em, this);
    //if(pe->exec() == 1) { updateClubs(); }
}

