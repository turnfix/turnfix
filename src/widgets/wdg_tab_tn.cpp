#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "model/settings/session.h"
#include "model/objects/event.h"
#include "header/wdg_tab_tn.h"
#include "../dialogs/participants/header/dlg_tn.h"
#include "../dialogs/participants/header/dlg_team.h"
#include "../dialogs/participants/header/dlg_group.h"
#include "../dialogs/participants/header/dlg_club.h"
#include "../dialogs/points/header/dlg_quali.h"
#include "../global/header/_global.h"
#include <QSqlError>

Tab_TN::Tab_TN(QWidget* parent) : QWidget(parent) {
    setupUi(this);
    event = Session::getInstance()->getEvent();
    tn_model = new QSqlQueryModel();
    tn_sort_model = new QSortFilterProxyModel();
    //tn_sort_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    tn_sort_model->setSourceModel(tn_model);
    tn_table->setModel(tn_sort_model);
    tn_model2 = new QSqlQueryModel();
    tn_sort_model2 = new QSortFilterProxyModel();
    //tn_sort_model2->setFilterCaseSensitivity(Qt::CaseInsensitive);
    tn_sort_model2->setSourceModel(tn_model2);
    tn_table2->setModel(tn_sort_model2);
    tn_model3 = new QSqlQueryModel();
    tn_sort_model3 = new QSortFilterProxyModel();
    //tn_sort_model3->setFilterCaseSensitivity(Qt::CaseInsensitive);
    tn_sort_model3->setSourceModel(tn_model3);
    tn_table3->setModel(tn_sort_model3);
    connect(but_addTN, SIGNAL(clicked()), this, SLOT(addTN()));
    connect(but_addCL, SIGNAL(clicked()), this, SLOT(addCL()));
    connect(but_editTN, SIGNAL(clicked()), this, SLOT(editTN()));
    connect(but_delTN, SIGNAL(clicked()), this, SLOT(delTN()));
    connect(but_timeTN, SIGNAL(clicked()), this, SLOT(meldeTN()));
    connect(but_copyTN, SIGNAL(clicked()), this, SLOT(syncTN()));
    connect(tn_table->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateMelde()));
    connect(cmb_filterTN, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTNFilterColumn(int)));
    connect(txt_filterTN, SIGNAL(textChanged(QString)), this, SLOT(updateTNFilterText(QString)));
    connect(cmb_typ, SIGNAL(currentIndexChanged(int)), this, SLOT(refresh()));
    connect(tn_table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editTN()));
    connect(tn_table2, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editTN()));
    connect(tn_table3, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editTN()));
}

void Tab_TN::loadBestView() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(CASE WHEN int_typ=0 THEN 1 END), COUNT(CASE WHEN int_typ=1 THEN 1 END), COUNT(CASE WHEN int_typ=2 THEN 1 END) FROM tfx_wettkaempfe WHERE int_veranstaltungenid=?");
    query.bindValue(0, this->event->getMainEventId());
    query.exec();
    query.next();
    if (query.value(1).toInt()>query.value(0).toInt()) {
        if (query.value(2).toInt()>query.value(0).toInt()) {
            cmb_typ->setCurrentIndex(2);
        } else {
            cmb_typ->setCurrentIndex(1);
        }
    } else if (query.value(2).toInt()>query.value(0).toInt()) {
        cmb_typ->setCurrentIndex(2);
    } else {
        cmb_typ->setCurrentIndex(0);
    }
}

void Tab_TN::refresh() {
    if (sta_tables->currentIndex() == 0) {
        fillTNTable();
        if (tn_table->currentIndex().isValid()) {
            but_timeTN->setEnabled(true);
        } else {
            but_timeTN->setEnabled(false);
        }
        but_addCL->setEnabled(true);
        but_copyTN->setVisible(false);
    } else if (sta_tables->currentIndex() == 1) {
        fillTNTable2();
        but_timeTN->setEnabled(false);
        but_addCL->setEnabled(false);
        if (!this->event->isMultiRoundEvent()) {
            but_copyTN->setVisible(false);
        } else if (this->event->getRound() > 1) {
            but_copyTN->setVisible(true);
        }
    } else if (sta_tables->currentIndex() == 2) {
        fillTNTable3();
        but_timeTN->setEnabled(false);
        but_addCL->setEnabled(false);
        but_copyTN->setVisible(false);
    }
    cmb_filterTN->setCurrentIndex(0);
}
void Tab_TN::fillTNTable() {
    int idx = cmb_filterTN->currentIndex();
    cmb_filterTN->clear();
    QSqlQuery query;
    query.prepare("SELECT tfx_wertungen.int_startnummer, tfx_teilnehmer.var_nachname || ', ' || tfx_teilnehmer.var_vorname || CASE WHEN tfx_wertungen.bol_ak THEN ' (AK)' ELSE '' END, "+_global::date("dat_geburtstag",2)+", CASE WHEN tfx_teilnehmer.int_geschlecht=0 THEN 'w' ELSE 'm' END, tfx_vereine.var_name, tfx_wettkaempfe.var_nummer, tfx_wertungen.var_riege, tfx_wertungen.int_wertungenid FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) INNER JOIN tfx_vereine USING (int_vereineid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE tfx_wettkaempfe.int_veranstaltungenid=? AND int_runde=? AND tfx_wertungen.int_gruppenid IS NULL AND tfx_wertungen.int_mannschaftenid IS NULL ORDER BY tfx_wettkaempfe.var_nummer, "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, tfx_teilnehmer.var_nachname, tfx_teilnehmer.var_vorname");
    query.bindValue( 0, this->event->getMainEventId() );
    query.bindValue( 1, this->event->getRound());
    query.exec();
    tn_model->setQuery(query);
    QHeaderView::ResizeMode resizeModeTN[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resizeTN[] = {40, 200, 40, 40, 200, 45, 45};
    QString headersTN[7] = {"StNr.", "Name", "Geb.", "m/w", "Verein", "WK", "Riege"};
    for (int i=0;i<7;i++) {
        cmb_filterTN->addItem(headersTN[i]);
        tn_model->setHeaderData(i, Qt::Horizontal, headersTN[i]);
        tn_table->horizontalHeader()->setSectionResizeMode(i, resizeModeTN[i]);
        tn_table->horizontalHeader()->resizeSection(i, resizeTN[i]);
    }
    tn_table->hideColumn(7);
    cmb_filterTN->setCurrentIndex(idx);
}

void Tab_TN::fillTNTable2() {
    int idx = cmb_filterTN->currentIndex();
    cmb_filterTN->clear();
    QSqlQuery query;
    query.prepare("SELECT tfx_mannschaften.int_startnummer, tfx_vereine.var_name, tfx_mannschaften.int_nummer || '. Mannschaft', tfx_wettkaempfe.var_nummer, tfx_mannschaften.var_riege, tfx_mannschaften.int_mannschaftenid FROM tfx_mannschaften INNER JOIN tfx_vereine USING (int_vereineid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_mannschaften.int_wettkaempfeid WHERE tfx_wettkaempfe.int_veranstaltungenid=? ORDER BY tfx_wettkaempfe.var_nummer, "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, tfx_mannschaften.int_nummer");
    query.bindValue( 0,this->event->getMainEventId() );
    query.exec();
    tn_model2->setQuery(query);
    QHeaderView::ResizeMode resizeModeTN[] = {QHeaderView::Fixed,
                                              QHeaderView::Stretch,
                                              QHeaderView::Stretch,
                                              QHeaderView::Fixed,
                                              QHeaderView::Fixed};
    int resizeTN[] = {40,
                      200,
                      200,
                      45,
                      45};
    QString headersTN[5] = {"StNr.",
                            "Verein",
                            "Mannschaft",
                            "WK",
                            "Riege"};
    for (int i=0;i<5;i++) {
        cmb_filterTN->addItem(headersTN[i]);
        tn_model2->setHeaderData(i, Qt::Horizontal, headersTN[i]);
        tn_table2->horizontalHeader()->setSectionResizeMode(i, resizeModeTN[i]);
        tn_table2->horizontalHeader()->resizeSection(i, resizeTN[i]);
    }
    tn_table2->hideColumn(5);
    cmb_filterTN->setCurrentIndex(idx);
}

void Tab_TN::fillTNTable3() {
    int idx = cmb_filterTN->currentIndex();
    cmb_filterTN->clear();
    QSqlQuery query;
    query.prepare("SELECT tfx_wertungen.int_startnummer, tfx_gruppen.var_name, tfx_vereine.var_name, tfx_wettkaempfe.var_nummer, tfx_wertungen.var_riege, tfx_gruppen.int_gruppenid FROM tfx_wertungen INNER JOIN tfx_gruppen USING (int_gruppenid) INNER JOIN tfx_vereine USING (int_vereineid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE tfx_wettkaempfe.int_veranstaltungenid=? AND int_runde=? ORDER BY tfx_wettkaempfe.var_nummer, "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, tfx_gruppen.var_name");
    query.bindValue( 0,this->event->getMainEventId() );
    query.bindValue( 1, this->event->getRound());
    query.exec();
    tn_model3->setQuery(query);
    QHeaderView::ResizeMode resizeModeTN[] = {QHeaderView::Fixed,
                                              QHeaderView::Stretch,
                                              QHeaderView::Stretch,
                                              QHeaderView::Fixed,
                                              QHeaderView::Fixed};
    int resizeTN[] = {40,
                      200,
                      200,
                      45,
                      45};
    QString headersTN[5] = {"StNr.",
                            "Gruppe",
                            "Verein",
                            "WK",
                            "Riege"};
    for (int i=0;i<5;i++) {
        cmb_filterTN->addItem(headersTN[i]);
        tn_model3->setHeaderData(i, Qt::Horizontal, headersTN[i]);
        tn_table3->horizontalHeader()->setSectionResizeMode(i, resizeModeTN[i]);
        tn_table3->horizontalHeader()->resizeSection(i, resizeTN[i]);
    }
    tn_table3->hideColumn(5);
    cmb_filterTN->setCurrentIndex(idx);
}

void Tab_TN::addTN() {
    if (sta_tables->currentIndex() == 0) {
        Tn_Dialog *tn = new Tn_Dialog(this->event, 0,this);
        if(tn->exec() == 1) {
            fillTNTable();
        }
    } else if (sta_tables->currentIndex() == 1) {
        Team_Dialog *tn = new Team_Dialog(this->event, 0,this);
        if(tn->exec() == 1) {
            fillTNTable2();
        }
    } else if (sta_tables->currentIndex() == 2) {
        Group_Dialog *tn = new Group_Dialog(this->event, 0,this);
        if(tn->exec() == 1) {
            fillTNTable3();
        }
    }
    _global::updateRgDis(this->event);
    tn_table->setFocus();
}

void Tab_TN::addCL() {
    Club_Dialog *cl = new Club_Dialog(this->event, this);
    if(cl->exec() == 1) {
        fillTNTable();
    }
}

void Tab_TN::editTN() {
    if (sta_tables->currentIndex() == 0) {
        if (tn_table->currentIndex().isValid()) {
            QModelIndex idx = tn_table->currentIndex();
            Tn_Dialog *tn = new Tn_Dialog(this->event, QVariant(tn_sort_model->data(tn_sort_model->index(tn_table->currentIndex().row(),7))).toInt(),this);
            if(tn->exec() == 1) {
                fillTNTable();
            }
            tn_table->setCurrentIndex(idx);
            tn_table->setFocus();
        }
    } else if (sta_tables->currentIndex() == 1) {
        if (tn_table2->currentIndex().isValid()) {
            QModelIndex idx = tn_table2->currentIndex();
            Team_Dialog *tn = new Team_Dialog(this->event, QVariant(tn_sort_model2->data(tn_sort_model2->index(tn_table2->currentIndex().row(),5))).toInt(),this);
            if(tn->exec() == 1) {
                fillTNTable2();
            }
            tn_table2->setCurrentIndex(idx);
            tn_table2->setFocus();
        }
    } else if (sta_tables->currentIndex() == 2) {
        if (tn_table3->currentIndex().isValid()) {
            QModelIndex idx = tn_table3->currentIndex();
            Group_Dialog *tn = new Group_Dialog(this->event, QVariant(tn_sort_model3->data(tn_sort_model3->index(tn_table3->currentIndex().row(),5))).toInt(),this);
            if(tn->exec() == 1) {
                fillTNTable3();
            }
            tn_table3->setCurrentIndex(idx);
            tn_table3->setFocus();
        }
    }
    _global::updateRgDis(this->event);
}

void Tab_TN::meldeTN() {
    Quali_Dialog *ml = new Quali_Dialog(this->event, QVariant(tn_sort_model->data(tn_sort_model->index(tn_table->currentIndex().row(),7))).toInt(),this);
    if(ml->exec() == 1) {}
    tn_table->setFocus();
}

void Tab_TN::delTN() {
    if (sta_tables->currentIndex() == 0) {
        if (tn_table->currentIndex().isValid()) {
            QMessageBox msg(QMessageBox::Question, "Teilnehmer löschen", "Wollen sie diesen Teilnehmer wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_wertungen WHERE int_wertungenid=?");
                query.bindValue( 0, QVariant(tn_sort_model->data(tn_sort_model->index(tn_table->currentIndex().row(),7))).toInt() );
                query.exec();
                fillTNTable();
            }
            tn_table->setFocus();
        }
    } else if (sta_tables->currentIndex() == 1) {
        if (tn_table2->currentIndex().isValid()) {
            QMessageBox msg(QMessageBox::Question, "Mannschaft löschen", "Wollen sie diese Mannschaft wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_mannschaften WHERE int_mannschaftenid=?");
                query.bindValue( 0, QVariant(tn_sort_model2->data(tn_sort_model2->index(tn_table2->currentIndex().row(),5))).toInt());
                query.exec();
                fillTNTable2();
            }
            tn_table2->setFocus();
        }
    } else if (sta_tables->currentIndex() == 2) {
        if (tn_table3->currentIndex().isValid()) {
            QMessageBox msg(QMessageBox::Question, "Gruppe löschen", "Wollen sie diese Gruppe wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_gruppen WHERE int_gruppenid=?");
                query.bindValue( 0, QVariant(tn_sort_model3->data(tn_sort_model3->index(tn_table3->currentIndex().row(),5))).toInt());
                query.exec();
                fillTNTable3();
            }
            tn_table3->setFocus();
        }
    }
    _global::updateRgDis(this->event);
}

void Tab_TN::updateMelde() {
    if (tn_table->currentIndex().isValid()) {
        QSqlQuery query;
        query.prepare("SELECT tfx_disziplinen.var_name, int_disziplinenid, int_wertungenid, var_maske FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_veranstaltungenid=? AND int_wertungenid=? AND bol_bahnen AND (int_disziplinenid IN (SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_disziplinenid=tfx_disziplinen.int_disziplinenid AND int_wertungenid=tfx_wertungen.int_wertungenid) OR (SELECT COUNT(*) FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=tfx_wertungen.int_wertungenid)=0)");
        query.bindValue(0,this->event->getId());
        query.bindValue(1,QVariant(tn_sort_model->data(tn_sort_model->index(tn_table->currentIndex().row(),7))).toInt());
        query.exec();
        if (_global::querySize(query) <= 0) {
            but_timeTN->setEnabled(false);
        } else {
            but_timeTN->setEnabled(true);
        }
    } else {
        but_timeTN->setEnabled(false);
    }
}

void Tab_TN::syncTN() {
    QMessageBox msg(QMessageBox::Question, "Mannschaften synchronisieren", "Wollen sie diese Mannschaft wirklich synchronisieren? Dabei werden alle vorhandenen Wertungen dieser Runde gelöscht.",QMessageBox::Ok | QMessageBox::Cancel);
    if(msg.exec() == QMessageBox::Ok) {
        QSqlQuery query;
        query.prepare("DELETE FROM tfx_wertungen WHERE int_wertungenid IN (SELECT int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=?) AND int_mannschaftenid IS NOT NULL");
        query.bindValue( 0, this->event->getMainEventId() );
        query.bindValue( 1, this->event->getRound());
        query.exec();
        query.prepare("DELETE FROM tfx_man_x_teilnehmer WHERE int_mannschaftenid IN (SELECT int_mannschaftenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=?) AND int_runde=?");
        query.bindValue( 0, this->event->getMainEventId() );
        query.bindValue( 1, 1);
        query.bindValue( 2, this->event->getRound());
        query.exec();
        QSqlQuery query2;
        query2.prepare("SELECT * FROM tfx_wertungen WHERE int_wertungenid IN (SELECT int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=?) AND int_mannschaftenid IS NOT NULL");
        query2.bindValue( 0, this->event->getMainEventId() );
        query2.bindValue( 1, 1);
        query2.exec();
        while (query2.next()) {
            if (query2.value(4).toInt()>0) {
                QSqlQuery query6;
                query6.prepare("INSERT INTO tfx_man_x_teilnehmer (int_mannschaftenid,int_teilnehmerid, int_runde) VALUES (?,?,?)");
                query6.bindValue(0,query2.value(4).toString());
                query6.bindValue(1,query2.value(2).toString());
                query6.bindValue(2,this->event->getRound());
                query6.exec();
            }
            QSqlQuery query7;
            query7.prepare("INSERT INTO tfx_wertungen (int_teilnehmerid,int_wettkaempfeid,var_riege,bol_ak,int_mannschaftenid,int_runde,int_startnummer,int_statusid) VALUES(?,?,?,?,?,?,?,?)");
            query7.bindValue( 0, query2.value(2).toString() );
            query7.bindValue( 1, query2.value(1).toString() );
            query7.bindValue( 2, query2.value(10).toString() );
            query7.bindValue( 3, query2.value(8).toBool() );
            query7.bindValue( 4, query2.value(4).toString() );
            query7.bindValue( 5, this->event->getRound() );
            query7.bindValue( 6, query2.value(7).toString() );
            query7.bindValue( 7, 1);
            query7.exec();
        }
        _global::updateRgDis(this->event);
        fillTNTable();
    }
}

void Tab_TN::updateTNFilterColumn(int index) {
    if (sta_tables->currentIndex() == 0) {
        tn_sort_model->setFilterKeyColumn(index);
    } else if (sta_tables->currentIndex() == 1) {
        tn_sort_model2->setFilterKeyColumn(index);
    } else if (sta_tables->currentIndex() == 2) {
        tn_sort_model3->setFilterKeyColumn(index);
    }
}

void Tab_TN::updateTNFilterText(QString text) {
    QRegExp expr(text);
    expr.setCaseSensitivity(Qt::CaseInsensitive);
    //expr.setPatternSyntax(QRegExp::Wildcard);
    if (sta_tables->currentIndex() == 0) {
        tn_sort_model->setFilterRegExp(expr);
    } else if (sta_tables->currentIndex() == 1) {
        tn_sort_model2->setFilterRegExp(expr);
    } else if (sta_tables->currentIndex() == 2) {
        tn_sort_model3->setFilterRegExp(expr);
    }
}
