#include "teamdialog.h"
#include "masterdata/athletedialog.h"
#include "masterdata/clubdialog.h"
#include "model/entity/athlete.h"
#include "model/entity/club.h"
#include "model/entity/event.h"
#include "src/global/header/_global.h"
#include "src/global/header/settings.h"
#include "ui_teamdialog.h"
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QToolBar>

TeamDialog::TeamDialog(Event *event, EntityManager *em, int edit, QWidget *parent)
    : QDialog(parent)
    , m_em(em)
    , ui(new Ui::TeamDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    this->m_event = event;
    editid=edit;

    QToolBar *tb = new QToolBar();
    QActionGroup *ag = new QActionGroup(this);
    tb->setAllowedAreas(Qt::LeftToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setOrientation(Qt::Vertical);
    tb->addAction(ui->act_team);
    ag->addAction(ui->act_team);
    tb->addAction(ui->act_tn);
    ag->addAction(ui->act_tn);
    tb->addAction(ui->act_penalty);
    ag->addAction(ui->act_penalty);
    ui->act_team->setChecked(true);
    ui->sidebar->layout()->addWidget(tb);

    connect(ui->act_team, &QAction::triggered, [this](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->act_tn, &QAction::triggered, [this](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->act_penalty, &QAction::triggered, [this](){
        ui->stackedWidget->setCurrentIndex(2);
    });

    model2 = new QSqlQueryModel();
    sortmodel = new QSortFilterProxyModel();
    sortmodel->setSourceModel(model2);
    model = new QStandardItemModel();
    model->setColumnCount(6);
    ui->tbl_tn->setModel(model);
    ui->tbl_avtn->setModel(sortmodel);
    QHeaderView::ResizeMode resizeMode[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::ResizeToContents, QHeaderView::Fixed};
    int resize[] = {40,200,40,23,23,0};
    QString headers[6] = {"StNr.","Name","Geb.","AK","SN","id"};
    for (int i=0;i<6;i++) {
        model->setHeaderData(i, Qt::Horizontal, headers[i]);
        ui->tbl_tn->horizontalHeader()->setSectionResizeMode(i, resizeMode[i]);
        ui->tbl_tn->horizontalHeader()->resizeSection(i, resize[i]);
    }
    QSqlQuery query("SELECT rel_abzug || ' Pkt. ' || var_name, int_mannschaften_abzugid FROM tfx_mannschaften_abzug ORDER BY rel_abzug, var_name");
    while (query.next()) {
        ui->lst_penalty->addItem(query.value(0).toString());
        ui->lst_penalty->item(ui->lst_penalty->count()-1)->setData(Qt::UserRole,query.value(1).toInt());
        ui->lst_penalty->item(ui->lst_penalty->count()-1)->setCheckState(Qt::Unchecked);
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
    connect(ui->cmb_club, SIGNAL(currentIndexChanged(int)), this, SLOT(fillTable2()));
    connect(ui->but_TNadd, SIGNAL(clicked()), this, SLOT(addAv()));
    connect(ui->but_TNedit, SIGNAL(clicked()), this, SLOT(editAv()));
    connect(ui->but_addclub, SIGNAL(clicked()), this, SLOT(addClub()));
    connect(this, SIGNAL(finished(int)), this, SLOT(saveWindowState()));
    initData();
}

TeamDialog::~TeamDialog()
{
    delete ui;
}

void TeamDialog::saveWindowState() {
    Settings::updateFiler(ui->chk_club->isChecked(),ui->chk_planned->isChecked());
}

void TeamDialog::initData() {
    updateClubs();
    QSqlQuery query2;
    query2.prepare("SELECT int_wettkaempfeid, var_nummer, var_name FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? AND int_typ=1 ORDER BY var_nummer ASC");
    query2.bindValue(0, this->m_event->mainEvent()->id());
    query2.exec();
    while (query2.next()) {
        ui->cmb_wk->addItem(query2.value(1).toString() + " " + query2.value(2).toString(),query2.value(0).toInt());
    }
    if (editid != 0) {
        QSqlQuery query;
        query.prepare("SELECT * FROM tfx_mannschaften WHERE int_mannschaftenid=? LIMIT 1");
        query.bindValue( 0, editid );
        query.exec();
        query.next();
        ui->sbx_tno->setValue(query.value(3).toInt());
        ui->txt_rg->setText(query.value(4).toString());
        ui->cmb_wk->setCurrentIndex(ui->cmb_wk->findData(query.value(1).toInt()));
        ui->cmb_club->setCurrentIndex(ui->cmb_club->findData(query.value(2).toInt()));
        QSqlQuery query8;
        query8.prepare("SELECT int_teilnehmerid FROM tfx_wertungen WHERE int_mannschaftenid=? AND int_runde=? ORDER BY int_startnummer");
        query8.bindValue(0, editid);
        query8.bindValue(1, this->m_event->round());
        query8.exec();
        while (query8.next()) {
            lst_int_ids.append(query8.value(0).toInt());
        }
        QSqlQuery query9;
        query9.prepare("SELECT rel_abzug || ' Pkt. ' || var_name FROM tfx_man_x_man_ab INNER JOIN tfx_mannschaften_abzug USING (int_mannschaften_abzugid) WHERE int_mannschaftenid=?");
        query9.bindValue(0,editid);
        query9.exec();
        while (query9.next()) {
            ui->lst_penalty->findItems(query9.value(0).toString(),Qt::MatchExactly).at(0)->setCheckState(Qt::Checked);
        }
    }
    fillTable();
    fillTable2();
    if (this->m_event->round() > 1) {
        ui->cmb_club->setEnabled(false);
        ui->sbx_tno->setEnabled(false);
        ui->cmb_wk->setEnabled(false);
        ui->txt_rg->setEnabled(false);
    }
}

void TeamDialog::save() {
    QSqlQuery getWertungsZahl;
    getWertungsZahl.prepare("SELECT int_wertungen FROM tfx_wettkaempfe WHERE int_wettkaempfeid=?");
    getWertungsZahl.bindValue(0,ui->cmb_wk->itemData(ui->cmb_wk->currentIndex()));
    getWertungsZahl.exec();
    getWertungsZahl.next();
    if (getWertungsZahl.value(0).toInt()>model->rowCount()) {
        int ret = QMessageBox::warning(this,tr("Zu wenig Mannschaftsmitglieder"),tr("Es befinden sich weniger Teilnehmer in der Mannschafts als in die Wertung kommen. Wollen sie trotzdem speichern?"),QMessageBox::Ok,QMessageBox::Cancel);
        if (ret != QMessageBox::Ok) return;
    }
    int vid=ui->cmb_club->itemData(ui->cmb_club->currentIndex()).toInt();
    QSqlQuery query4;
    if (editid == 0) {
        query4.prepare("INSERT INTO tfx_mannschaften (int_vereineid,int_nummer,int_wettkaempfeid,var_riege,int_startnummer) VALUES(?,?,?,?,?)");
        QSqlQuery query0;
        query0.prepare("SELECT MAX(int_startnummer) FROM tfx_mannschaften INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=?");
        query0.bindValue(0, this->m_event->id());
        query0.exec();
        query0.next();
        query4.bindValue( 4, (query0.value(0).toInt()+1));
    } else {
        query4.prepare("UPDATE tfx_mannschaften SET int_vereineid=?, int_nummer=?, int_wettkaempfeid=?, var_riege=? WHERE int_mannschaftenid=?");
        query4.bindValue( 4, editid );
    }
    query4.bindValue( 0, vid );
    query4.bindValue( 1, ui->sbx_tno->value() );
    query4.bindValue( 2, ui->cmb_wk->itemData(ui->cmb_wk->currentIndex()) );
    query4.bindValue( 3, ui->txt_rg->text() );
    query4.exec();
    int team;
    if (editid == 0) {
        if (_global::getDBTyp() == 0) {
            QSqlQuery query5("SELECT last_value FROM tfx_mannschaften_int_mannschaftenid_seq");
            query5.next();
            team = query5.value(0).toInt();
        } else {
            team = query4.lastInsertId().toInt();
        }
    } else {
        team = editid;
    }
    for (int i=0;i<ui->lst_penalty->count();i++) {
        QSqlQuery query;
        query.prepare("SELECT int_man_x_man_abid FROM tfx_man_x_man_ab WHERE int_mannschaftenid=? AND int_mannschaften_abzugid=?");
        query.bindValue(0,team);
        query.bindValue(1,ui->lst_penalty->item(i)->data(Qt::UserRole).toInt());
        query.exec();
        if (ui->lst_penalty->item(i)->checkState() == Qt::Checked) {
            if (_global::querySize(query) == 0) {
                QSqlQuery query2;
                query2.prepare("INSERT INTO tfx_man_x_man_ab (int_mannschaftenid,int_mannschaften_abzugid) VALUES (?,?)");
                query2.bindValue(0,team);
                query2.bindValue(1,ui->lst_penalty->item(i)->data(Qt::UserRole).toInt());
                query2.exec();
            }
        } else {
            if (_global::querySize(query) > 0) {
                query.next();
                QSqlQuery query2;
                query2.prepare("DELETE FROM tfx_man_x_man_ab WHERE int_man_x_man_abid=?");
                query2.bindValue(0,query.value(0).toInt());
                query2.exec();
            }
        }
    }
    if (editid == 0) {
        if (_global::getDBTyp() == 0) {
            QSqlQuery query5("SELECT last_value FROM tfx_mannschaften_int_mannschaftenid_seq");
            query5.next();
            editid = query5.value(0).toInt();
        } else {
            editid = query4.lastInsertId().toInt();
        }
    }
    for (int i=0;i<lst_int_ids.size();i++) {
        QSqlQuery query5;
        query5.prepare("SELECT int_mannschaftenid,int_teilnehmerid FROM tfx_man_x_teilnehmer WHERE int_mannschaftenid=? AND int_teilnehmerid=? AND int_runde=?");
        query5.bindValue(0, editid);
        query5.bindValue(1, lst_int_ids.at(i));
        query5.bindValue(2, this->m_event->round());
        query5.exec();
        if (_global::querySize(query5) == 0) {
            QSqlQuery query6;
            query6.prepare("INSERT INTO tfx_man_x_teilnehmer (int_mannschaftenid,int_teilnehmerid, int_runde) VALUES (?,?,?)");
            query6.bindValue(0, editid);
            query6.bindValue(1, lst_int_ids.at(i));
            query6.bindValue(2, this->m_event->round());
            query6.exec();
            QSqlQuery query20;
            query20.prepare("SELECT MAX(int_startnummer) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid INNER JOIN tfx_vereine USING (int_vereineid) WHERE int_veranstaltungenid=? AND int_runde=?");
            query20.bindValue(0, this->m_event->mainEvent()->id());
            query20.bindValue(1, this->m_event->round());
            query20.exec();
            query20.next();
            QSqlQuery query7;
            query7.prepare("INSERT INTO tfx_wertungen (int_teilnehmerid,int_wettkaempfeid,var_riege,bol_ak,int_mannschaftenid,int_runde,int_startnummer,bol_startet_nicht,int_statusid) VALUES(?,?,?,?,?,?,?,?,?)");
            query7.bindValue( 0, lst_int_ids.at(i) );
            query7.bindValue( 1, ui->cmb_wk->itemData(ui->cmb_wk->currentIndex()) );
            query7.bindValue( 2, ui->txt_rg->text() );
            if (model->item(i,3)->checkState() == Qt::Checked) {
                query7.bindValue(3,true);
            } else {
                query7.bindValue(3,false);
            }
            query7.bindValue(4, editid );
            query7.bindValue(5, this->m_event->round());
            query7.bindValue(6, (query20.value(0).toInt()+1) );
            if (model->item(i,4)->checkState() == Qt::Checked) {
                query7.bindValue(7,true);
            } else {
                query7.bindValue(7,false);
            }
            query7.bindValue( 8, 1);
            query7.exec();
        } else {
            QSqlQuery query6;
            query6.prepare("SELECT int_wertungenid FROM tfx_wertungen WHERE int_mannschaftenid=? AND int_teilnehmerid=? AND int_runde=?");
            query6.bindValue(0, editid);
            query6.bindValue(1, lst_int_ids.at(i));
            query6.bindValue(2, this->m_event->round());
            query6.exec();
            query6.next();
            QSqlQuery query7;
            query7.prepare("UPDATE tfx_wertungen SET int_wettkaempfeid=?, var_riege=?, bol_ak=?, bol_startet_nicht=?, int_statusid=? WHERE int_wertungenid=?");
            query7.bindValue(0,ui->cmb_wk->itemData(ui->cmb_wk->currentIndex()));
            query7.bindValue(1,ui->txt_rg->text());
            if (model->item(i,3)->checkState() == Qt::Checked) {
                query7.bindValue(2,true);
            } else {
                query7.bindValue(2,false);
            }
            if (model->item(i,4)->checkState() == Qt::Checked) {
                query7.bindValue(3,true);
            } else {
                query7.bindValue(3,false);
            }
            query7.bindValue(4,1);
            query7.bindValue(5,query6.value(0).toInt());
            query7.exec();
        }
    }
    QSqlQuery query8;
    query8.prepare("SELECT int_man_x_teilnehmerid,int_teilnehmerid,int_mannschaftenid FROM tfx_man_x_teilnehmer WHERE int_mannschaftenid=?");
    query8.bindValue(0,editid);
    query8.exec();
    while (query8.next()) {
        if (!lst_int_ids.contains(query8.value(1).toInt())) {
            QSqlQuery query9;
            query9.prepare("DELETE FROM tfx_man_x_teilnehmer WHERE int_man_x_teilnehmerid=? AND int_runde=?");
            query9.bindValue(0, query8.value(0).toInt());
            query9.bindValue(1, this->m_event->round());
            query9.exec();
            QSqlQuery query10;
            query10.prepare("DELETE FROM tfx_wertungen WHERE int_mannschaftenid=? AND int_teilnehmerid=? AND int_runde=?");
            query10.bindValue(0, query8.value(2).toInt());
            query10.bindValue(1, query8.value(1).toInt());
            query10.bindValue(2, this->m_event->round());
            query10.exec();
        }
    }
    done(1);
}

void TeamDialog::addTn() {
    QModelIndexList indexes = ui->tbl_avtn->selectionModel()->selectedRows();
    for (int i=0;i<indexes.count();i++) {
        if (lst_int_ids.indexOf(QVariant(sortmodel->data(sortmodel->index(indexes.at(i).row(),3))).toInt()) == -1) {
            lst_int_ids.append(QVariant(sortmodel->data(sortmodel->index(indexes.at(i).row(),3))).toInt());
        }
    }
    fillTable();
}

void TeamDialog::removeTn() {
    QModelIndexList indexes = ui->tbl_tn->selectionModel()->selectedRows();
    std::sort(indexes.begin(), indexes.end());
    for (int i=indexes.count()-1;i>=0;i--) {
        model->removeRow(indexes.at(i).row());
        lst_int_ids.removeAt(indexes.at(i).row());
    }
    fillTable2();
}

void TeamDialog::fillTable() {
    QString pgExtra;
    if (_global::getDBTyp()==0) pgExtra = "::text";
    for (int i=0;i<lst_int_ids.size();i++) {
        QSqlQuery query;
        query.prepare("SELECT (CASE WHEN (SELECT COUNT(*) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_mannschaftenid IS NOT NULL AND int_veranstaltungenid=? AND int_runde=? AND int_teilnehmerid=tfx_teilnehmer.int_teilnehmerid) >0 THEN (SELECT int_startnummer"+pgExtra+" FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_mannschaftenid IS NOT NULL AND int_veranstaltungenid=? AND int_runde=? AND int_teilnehmerid=tfx_teilnehmer.int_teilnehmerid) ELSE 'nA' END) AS stnr, tfx_teilnehmer.var_nachname || ', ' || tfx_teilnehmer.var_vorname, "+_global::date("dat_geburtstag",4)+", tfx_teilnehmer.int_teilnehmerid FROM tfx_teilnehmer WHERE tfx_teilnehmer.int_teilnehmerid=? LIMIT 1");
        query.bindValue(0, this->m_event->mainEvent()->id());
        query.bindValue(1, this->m_event->round());
        query.bindValue(2, this->m_event->mainEvent()->id());
        query.bindValue(3, this->m_event->round());
        query.bindValue(4, lst_int_ids.at(i) );
        query.exec();
        query.next();
        for (int j=0;j<3;j++) {
            model->setItem(i,j,new QStandardItem(query.value(j).toString()));
        }
        QStandardItem *itak = new QStandardItem();
        QStandardItem *itks = new QStandardItem();
        if (editid != 0 && query.value(0).toString().compare("nA") != 0) {
            QSqlQuery query2;
            query2.prepare("SELECT bol_ak, bol_startet_nicht FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) WHERE int_mannschaftenid=? AND int_teilnehmerid=? AND int_runde=? LIMIT 1");
            query2.bindValue(0, editid);
            query2.bindValue(1, query.value(3).toInt());
            query2.bindValue(2, this->m_event->round());
            query2.exec();
            query2.next();
            if (query2.value(0).toBool()) {
                itak->setCheckState(Qt::Checked);
            } else {
                itak->setCheckState(Qt::Unchecked);
            }
            if (query2.value(1).toBool()) {
                itks->setCheckState(Qt::Checked);
            } else {
                itks->setCheckState(Qt::Unchecked);
            }
        }
        itak->setCheckable(true);
        model->setItem(i,3,itak);
        itks->setCheckable(true);
        model->setItem(i,4,itks);
        model->setItem(i,5,new QStandardItem(query.value(3).toString()));

        bool ageCheck = checkJg(query.value(2).toInt());
        if (!ageCheck) {
            for (int j=0;j<model->columnCount();j++) {
                model->item(i,j)->setBackground(Qt::red);
            }
        }
    }
}

void TeamDialog::fillTable2() {
    QSqlQuery query3;
    QString query;
    query = "SELECT var_nachname || ', ' || var_vorname, "+_global::date("dat_geburtstag",2)+", tfx_vereine.var_name, int_teilnehmerid FROM tfx_teilnehmer INNER JOIN tfx_vereine USING (int_vereineid) ";
    if (ui->chk_club->isChecked() && ui->chk_planned->isChecked()) {
        query += "WHERE int_vereineid=? AND int_teilnehmerid NOT IN (SELECT int_teilnehmerid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_teilnehmerid IS NOT NULL AND int_mannschaftenid != "+QString::number(editid)+") ";
        if (lst_int_ids.size()>0) query += "AND int_teilnehmerid NOT IN (" + _global::intListToString(lst_int_ids) + ") ";
    } else if (ui->chk_club->isChecked() ) {
        query += "WHERE int_vereineid=? ";
    } else if (ui->chk_planned->isChecked() ) {
        query += "WHERE int_teilnehmerid NOT IN (SELECT int_teilnehmerid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_teilnehmerid IS NOT NULL AND int_mannschaftenid != "+QString::number(editid)+") ";
        if (lst_int_ids.size()>0) query += "AND int_teilnehmerid NOT IN (" + _global::intListToString(lst_int_ids) + ") ";
    }
    query += "ORDER BY tfx_vereine.var_name, var_nachname, var_vorname";
    query3.prepare(query);
    if (ui->chk_club->isChecked() && ui->chk_planned->isChecked()) {
        query3.bindValue(0,ui->cmb_club->itemData(ui->cmb_club->currentIndex()));
        query3.bindValue(1, this->m_event->mainEvent()->id());
    } else if (ui->chk_club->isChecked() ) {
        query3.bindValue(0,ui->cmb_club->itemData(ui->cmb_club->currentIndex()));
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

void TeamDialog::addAv() {
    AthleteDialog *tu = new AthleteDialog(nullptr,
                                          qvariant_cast<Club *>(ui->cmb_club->currentData()),
                                          m_em,
                                          this);
    tu->exec();
    fillTable2();
}

void TeamDialog::editAv() {
    AthleteDialog *tu = new AthleteDialog(qvariant_cast<Athlete *>(
                                              sortmodel->data(ui->tbl_avtn->currentIndex())),
                                          m_em,
                                          this);
    tu->exec();
    fillTable2();
}

void TeamDialog::updateClubs() {
    QString currtext = ui->cmb_club->currentText();
    ui->cmb_club->clear();
    QSqlQuery query;
    query.prepare("SELECT int_vereineid, var_name FROM tfx_vereine ORDER BY var_name");
    query.exec();
    while (query.next()) {
        ui->cmb_club->addItem(query.value(1).toString(),query.value(0).toInt());
    }
    if (currtext != "") ui->cmb_club->setCurrentIndex(ui->cmb_club->findText(currtext));
}

void TeamDialog::addClub() {
    //TODO reenable
    //ClubDialog *pe = new ClubDialog(0, this);
    //if(pe->exec() == 1) { updateClubs(); }
}

bool TeamDialog::checkJg(int jg) {
    if (jg > 1900) {
        QSqlQuery query;
        query.prepare("SELECT yer_von, yer_bis FROM tfx_wettkaempfe WHERE int_wettkaempfeid=?");
        query.bindValue(0,ui->cmb_wk->itemData(ui->cmb_wk->currentIndex()));
        query.exec();
        query.next();
        if (query.value(1).toInt() == 2) {
            if (jg < query.value(0).toInt()) {
                return false;
            } else {
                return true;
            }
        } else if (query.value(1).toInt() == 1) {
            if (jg > query.value(0).toInt()) {
                return false;
            } else {
                return true;
            }
        } else if (query.value(1).toInt() != 3) {
            if ((query.value(0).toInt() >= query.value(1).toInt() && query.value(1).toInt() <= jg && jg <= query.value(0).toInt()) || (query.value(0).toInt() < query.value(1).toInt() && query.value(1).toInt() >= jg && jg >= query.value(0).toInt())) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }
    return false;
}
