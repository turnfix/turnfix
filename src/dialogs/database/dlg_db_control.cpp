#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QActionGroup>
#include <QToolBar>
#include "header/dlg_db_control.h"
#include "header/dlg_db_dis.h"
#include "header/dlg_db_tn.h"
#include "header/dlg_db_club.h"
#include "header/dlg_db_person.h"
#include "header/dlg_db_loc.h"
#include "header/dlg_db_acc.h"
#include "header/dlg_db_penalty.h"
#include "header/dlg_db_sec.h"
#include "header/dlg_db_sport.h"
#include "header/dlg_db_status.h"
#include "header/dlg_db_gau.h"
#include "header/dlg_db_land.h"
#include "header/dlg_db_verband.h"
#include "header/dlg_db_disgroup.h"
#include "header/dlg_db_formel.h"
#include "../../global/header/_global.h"

Db_Control_Dialog::Db_Control_Dialog(QWidget* parent, int type) : QMainWindow(parent) {
    setupUi(this);
    QToolBar *tb = new QToolBar();
    tb->setAllowedAreas(Qt::LeftToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setOrientation(Qt::Vertical);
    tb->addAction(act_sportler);
    tb->addAction(act_vereine);
    tb->addAction(act_disziplinen);
    tb->addAction(act_sport);
    tb->addAction(act_bereiche);
    tb->addAction(act_personen);
    tb->addAction(act_status);
    tb->addAction(act_konten);
    tb->addAction(act_orte);
    tb->addAction(act_gaue);
    tb->addAction(act_verbaende);
    tb->addAction(act_countries);
    tb->addAction(act_penalty);
    tb->addAction(act_disgruppe);
    tb->addAction(act_formel);
    sidebar->layout()->addWidget(tb);
    QActionGroup *ag = new QActionGroup(this);
    ag->addAction(act_sportler);
    ag->addAction(act_vereine);
    ag->addAction(act_disziplinen);
    ag->addAction(act_sport);
    ag->addAction(act_bereiche);
    ag->addAction(act_personen);
    ag->addAction(act_status);
    ag->addAction(act_konten);
    ag->addAction(act_orte);
    ag->addAction(act_gaue);
    ag->addAction(act_verbaende);
    ag->addAction(act_countries);
    ag->addAction(act_penalty);
    ag->addAction(act_disgruppe);
    ag->addAction(act_formel);
    db_model = new QSqlQueryModel();
    db_sort_model = new QSortFilterProxyModel();
    db_sort_model->setSourceModel(db_model);
    db_sort_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    db_table->setModel(db_sort_model);
    switch (type) {
    case 1: act_sportler->setChecked(true); break;
    case 2: act_vereine->setChecked(true); break;
    case 3: act_disziplinen->setChecked(true); break;
    case 4: act_sport->setChecked(true); break;
    case 5: act_bereiche->setChecked(true); break;
    case 6: act_personen->setChecked(true); break;
    case 7: act_status->setChecked(true); break;
    case 8: act_konten->setChecked(true); break;
    case 9: act_orte->setChecked(true); break;
    case 10: act_gaue->setChecked(true); break;
    case 11: act_verbaende->setChecked(true); break;
    case 12: act_countries->setChecked(true); break;
    case 13: act_penalty->setChecked(true); break;
    case 14: act_disgruppe->setChecked(true); break;
    case 15: act_formel->setChecked(true); break;
    }
    connect(cmb_filter, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilterColumn(int)));
    connect(txt_filter, SIGNAL(textChanged(QString)), this, SLOT(updateFilterText(QString)));
    connect(act_sportler, SIGNAL(triggered()), this, SLOT (loaddb()));
    connect(act_vereine, SIGNAL(triggered()), this, SLOT (loaddb()));
    connect(act_disziplinen, SIGNAL(triggered()), this, SLOT (loaddb()));
    connect(act_sport, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(act_bereiche, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(act_personen, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(act_status, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(act_konten, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(act_orte, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(act_gaue, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(act_verbaende, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(act_countries, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(act_penalty, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(act_disgruppe, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(act_formel, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(but_add, SIGNAL(clicked()), this, SLOT(add()));
    connect(but_edit, SIGNAL(clicked()), this, SLOT(edit()));
    connect(but_del, SIGNAL(clicked()), this, SLOT(del()));
    connect(db_table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit()));
    loaddb();
}

void Db_Control_Dialog::loaddb() {
    if (act_sportler->isChecked()) {
        currtype = 1;
    } else if (act_vereine->isChecked()) {
        currtype = 2;
    } else if (act_disziplinen->isChecked()) {
        currtype = 3;
    } else if (act_sport->isChecked()) {
        currtype = 4;
    } else if (act_bereiche->isChecked()) {
        currtype = 5;
    } else if (act_personen->isChecked()) {
        currtype = 6;
    } else if (act_status->isChecked()) {
        currtype = 7;
    } else if (act_konten->isChecked()) {
        currtype = 8;
    } else if (act_orte->isChecked()) {
        currtype = 9;
    } else if (act_gaue->isChecked()) {
        currtype = 10;
    } else if (act_verbaende->isChecked()) {
        currtype = 11;
    } else if (act_countries->isChecked()) {
        currtype = 12;
    } else if (act_penalty->isChecked()) {
        currtype = 13;
    } else if (act_disgruppe->isChecked()) {
        currtype = 14;
    } else if (act_formel->isChecked()) {
        currtype = 15;
    }
    getData();
    //Vereine
    QString headers1[4] = {"ID","Name","Geb.","Verein"};
    QHeaderView::ResizeMode resizeMode1[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Stretch};
    int resize1[] = {40,200,60,200};
    //Athleten
    QString headers2[4] = {"ID","Verein","Ansprechpartner","Website"};
    QHeaderView::ResizeMode resizeMode2[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch, QHeaderView::Stretch};
    int resize2[] = {40,200,200,200};
    //Disziplinen
    QString headers3[4] = {"ID","Name","Sport","Bereich"};
    QHeaderView::ResizeMode resizeMode3[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resize3[] = {40,200,90,60};
    //Sportarten
    QString headers4[4] = {"ID","Name","",""};
    QHeaderView::ResizeMode resizeMode4[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resize4[] = {40,200,0,0};
    //Bereiche
    QString headers5[4] = {"ID","Name","",""};
    QHeaderView::ResizeMode resizeMode5[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resize5[] = {40,200,0,0};
    //Personen
    QString headers6[4] = {"ID","Name","Telefon","Email"};
    QHeaderView::ResizeMode resizeMode6[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch, QHeaderView::Stretch};
    int resize6[] = {40,200,200,200};
    //Status
    QString headers7[4] = {"ID","Name","",""};
    QHeaderView::ResizeMode resizeMode7[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resize7[] = {40,200,0,0};
    //Konten
    QString headers8[4] = {"ID","Name","Kontonummer","BLZ"};
    QHeaderView::ResizeMode resizeMode8[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch, QHeaderView::Stretch};
    int resize8[] = {40,200,200,200};
    //Wettkampforte
    QString headers9[4] = {"ID","Name","PLZ","Ort"};
    QHeaderView::ResizeMode resizeMode9[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Stretch};
    int resize9[] = {40,200,60,200};
    //Gaue
    QString headers10[4] = {"ID","Turngau/-kreis","Kürzel",""};
    QHeaderView::ResizeMode resizeMode10[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::Fixed};
    int resize10[] = {40,200,70,0};
    //Verbände
    QString headers11[4] = {"ID","Landesverband","Kürzel",""};
    QHeaderView::ResizeMode resizeMode11[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::Fixed};
    int resize11[] = {40,200,70,0};
    //Länder
    QString headers12[4] = {"ID","Land","Kürzel",""};
    QHeaderView::ResizeMode resizeMode12[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::Fixed};
    int resize12[] = {40,200,70,0};
    //Strafen
    QString headers13[4] = {"ID","Strafe","Abzug",""};
    QHeaderView::ResizeMode resizeMode13[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::Fixed};
    int resize13[] = {40,200,70,0};
    //Dis-Gruppen
    QString headers14[4] = {"ID","Name","Kommentar",""};
    QHeaderView::ResizeMode resizeMode14[] = {QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed};
    int resize14[] = {40,250,200,0};
    //Formeln
    QString headers15[4] = {"ID","Name","Formel","Typ"};
    QHeaderView::ResizeMode resizeMode15[] = {QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed};
    int resize15[] = {40,250,200,60};
    QString *headers;
    QHeaderView::ResizeMode *resizeMode;
    int *resize;
    switch (currtype) {
    default:
    case 1: headers = headers1; resizeMode = resizeMode1; resize = resize1; break;
    case 2: headers = headers2; resizeMode = resizeMode2; resize = resize2; break;
    case 3: headers = headers3; resizeMode = resizeMode3; resize = resize3; break;
    case 4: headers = headers4; resizeMode = resizeMode4; resize = resize4; break;
    case 5: headers = headers5; resizeMode = resizeMode5; resize = resize5; break;
    case 6: headers = headers6; resizeMode = resizeMode6; resize = resize6; break;
    case 7: headers = headers7; resizeMode = resizeMode7; resize = resize7; break;
    case 8: headers = headers8; resizeMode = resizeMode8; resize = resize8; break;
    case 9: headers = headers9; resizeMode = resizeMode9; resize = resize9; break;
    case 10: headers = headers10; resizeMode = resizeMode10; resize = resize10; break;
    case 11: headers = headers11; resizeMode = resizeMode11; resize = resize11; break;
    case 12: headers = headers12; resizeMode = resizeMode12; resize = resize12; break;
    case 13: headers = headers13; resizeMode = resizeMode13; resize = resize13; break;
    case 14: headers = headers14; resizeMode = resizeMode14; resize = resize14; break;
    case 15: headers = headers15; resizeMode = resizeMode15; resize = resize15; break;
    }
    cmb_filter->clear();
    for (int i=0;i<4;i++) {
        db_model->setHeaderData(i, Qt::Horizontal, headers[i]);
        if (headers[i].length() > 0) {
            cmb_filter->addItem(headers[i],i);
        }
        db_table->horizontalHeader()->setSectionResizeMode(i, resizeMode[i]);
        db_table->horizontalHeader()->resizeSection(i, resize[i]);
    }
    txt_filter->setText("");
}
void Db_Control_Dialog::getData() {
    QSqlQuery query;
    switch (currtype) {
    case 1: {
            query.prepare("SELECT int_teilnehmerid, var_nachname || ', ' || var_vorname, "+_global::date("dat_geburtstag",4)+", var_name FROM tfx_teilnehmer INNER JOIN tfx_vereine USING (int_vereineid) ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", var_name, var_nachname, var_vorname");

        } break;
    case 2: {
            query.prepare("SELECT int_vereineid, tfx_vereine.var_name, var_vorname || ' ' || var_nachname, var_website FROM tfx_vereine LEFT JOIN tfx_personen USING (int_personenid) ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", var_name");

        } break;
    case 3: query.prepare("SELECT int_disziplinenid, tfx_disziplinen.var_name, tfx_sport.var_name, CASE WHEN bol_m = 'true' AND bol_w = 'true' THEN 'm/w' ELSE CASE WHEN bol_m = 'true' THEN 'm' ELSE 'w' END END FROM tfx_disziplinen INNER JOIN tfx_sport USING (int_sportid) ORDER BY tfx_sport.var_name, tfx_disziplinen.var_name"); break;
    case 4: query.prepare("SELECT int_sportid, var_name, '', '' FROM tfx_sport ORDER BY var_name"); break;
    case 5: query.prepare("SELECT int_bereicheid, var_name, '', '' FROM tfx_bereiche ORDER BY var_name"); break;
    case 6: query.prepare("SELECT int_personenid, var_nachname || ', ' || var_vorname, var_telefon, var_email FROM tfx_personen ORDER BY var_nachname, var_vorname"); break;
    case 7: query.prepare("SELECT int_statusid, var_name, '', '' FROM tfx_status ORDER BY var_name"); break;
    case 8: query.prepare("SELECT int_kontenid, var_name, var_kontonummer, var_blz FROM tfx_konten ORDER BY var_name"); break;
    case 9: query.prepare("SELECT int_wettkampforteid, var_name, var_plz, var_ort FROM tfx_wettkampforte ORDER BY var_name, var_ort"); break;
    case 10: query.prepare("SELECT int_gaueid, var_name, var_kuerzel, '' FROM tfx_gaue ORDER BY var_name"); break;
    case 11: query.prepare("SELECT int_verbaendeid, var_name, var_kuerzel, '' FROM tfx_verbaende ORDER BY var_name"); break;
    case 12: query.prepare("SELECT int_laenderid, var_name, var_kuerzel, '' FROM tfx_laender ORDER BY var_name"); break;
    case 13: query.prepare("SELECT int_mannschaften_abzugid, var_name, rel_abzug, '' FROM tfx_mannschaften_abzug ORDER BY var_name"); break;
    case 14: query.prepare("SELECT int_disziplinen_gruppenid, var_name, txt_comment, '' FROM tfx_disziplinen_gruppen ORDER BY var_name"); break;
    case 15: query.prepare("SELECT int_formelid, var_name, var_formel, int_typ FROM tfx_formeln ORDER BY var_name"); break;
    }
    query.exec();
    db_model->setQuery(query);
    lbl_rowcount->setText(QString::number(db_sort_model->rowCount()));
}

void Db_Control_Dialog::add() {
    QDialog *dia;
    switch (currtype) {
    default:
    case 1: dia = new Db_Tn_Dialog(0,this); break;
    case 2: dia = new Db_Club_Dialog(0,this); break;
    case 3: dia = new Db_Dis_Dialog(0,this); break;
    case 4: dia = new Db_Sp_Dialog(0,this); break;
    case 5: dia = new Db_Sec_Dialog(0,this); break;
    case 6: dia = new Db_Pe_Dialog(0,this); break;
    case 7: dia = new Db_St_Dialog(0,this); break;
    case 8: dia = new Db_Acc_Dialog(0,this); break;
    case 9: dia = new Db_Loc_Dialog(0,this); break;
    case 10: dia = new Db_Gau_Dialog(0,this); break;
    case 11: dia = new Db_Verband_Dialog(0,this); break;
    case 12: dia = new Db_Land_Dialog(0,this); break;
    case 13: dia = new Db_Penalty_Dialog(0,this); break;
    case 14: dia = new Db_Disgroup_Dialog(0,this); break;
    case 15: dia = new Formel_Dialog(0,this); break;
    }
    if(dia->exec() == 1) {
        getData();
    }
}

void Db_Control_Dialog::edit() {
    QDialog *dia;
    int id = QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt();
    switch (currtype) {
    default:
    case 1: dia = new Db_Tn_Dialog(id,this); break;
    case 2: dia = new Db_Club_Dialog(id,this); break;
    case 3: dia = new Db_Dis_Dialog(id,this); break;
    case 4: dia = new Db_Sp_Dialog(id,this); break;
    case 5: dia = new Db_Sec_Dialog(id,this); break;
    case 6: dia = new Db_Pe_Dialog(id,this); break;
    case 7: dia = new Db_St_Dialog(id,this); break;
    case 8: dia = new Db_Acc_Dialog(id,this); break;
    case 9: dia = new Db_Loc_Dialog(id,this); break;
    case 10: dia = new Db_Gau_Dialog(id,this); break;
    case 11: dia = new Db_Verband_Dialog(id,this); break;
    case 12: dia = new Db_Land_Dialog(id,this); break;
    case 13: dia = new Db_Penalty_Dialog(id,this); break;
    case 14: dia = new Db_Disgroup_Dialog(id,this); break;
    case 15: dia = new Formel_Dialog(id,this); break;
    }
    if(dia->exec() == 1) {
        QModelIndex sel = db_table->currentIndex();
        getData();
        db_table->setCurrentIndex(sel);
    }
}

void Db_Control_Dialog::del() {
    switch (currtype) {
    case 1: {
            QMessageBox msg(QMessageBox::Question, "Teilnehmer löschen", "Wollen sie diesen Teilnehmer wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_teilnehmer WHERE int_teilnehmerid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Teilnehmer kann nicht gelöscht werden, da er noch einem Wettkampf zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 2: {
            QMessageBox msg(QMessageBox::Question, "Verein löschen", "Wollen sie diesen Verein wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_vereine WHERE int_vereineid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Verein kann nicht gelöscht werden, da er noch einem Teilnehmer zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 3: {
            QMessageBox msg(QMessageBox::Question, "Disziplin löschen", "Wollen sie diese Disziplin wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("SELECT * FROM tfx_wettkaempfe_x_disziplinen WHERE int_disziplinenid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (_global::querySize(query) == 0) {
                    query.prepare("DELETE FROM tfx_disziplinen_felder WHERE int_disziplinenid=?");
                    query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                    query.exec();
                    query.prepare("DELETE FROM tfx_disziplinen WHERE int_disziplinenid=?");
                    query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                    query.exec();
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                } else {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Diese Disziplin kann nicht gelöscht werden, da sie noch in einem Wettkampf verwendet wird!",QMessageBox::Ok);
                    msg.exec();
                }
            }
        }; break;
    case 4: {
            QMessageBox msg(QMessageBox::Question, "Sport löschen", "Wollen sie diesen Sport wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_sport WHERE int_sportid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Sport kann nicht gelöscht werden, da er noch zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 5: {
            QMessageBox msg(QMessageBox::Question, "Bereich löschen", "Wollen sie diesen Bereich wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_bereiche WHERE int_bereicheid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Bereich kann nicht gelöscht werden, da er noch zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 6: {
            QMessageBox msg(QMessageBox::Question, "Person löschen", "Wollen sie diese Person wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_personen WHERE int_personenid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Diese Person kann nicht gelöscht werden, da sie noch zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 7: {
            QMessageBox msg(QMessageBox::Question, "Status löschen", "Wollen sie diesen Status wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_status WHERE int_statusid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Status kann nicht gelöscht werden, da er noch zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 8: {
            QMessageBox msg(QMessageBox::Question, "Konto löschen", "Wollen sie dieses Konto wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_konten WHERE int_kontenid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieses Konto kann nicht gelöscht werden, da es noch einem Wettkampf zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 9: {
            QMessageBox msg(QMessageBox::Question, "Wettkampfort löschen", "Wollen sie diesen Wettkampfort wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_wettkampforte WHERE int_wettkampforteid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Wettkampfort kann nicht gelöscht werden, da er noch einem Wettkampf zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 10: {
            QMessageBox msg(QMessageBox::Question, "Turnkreis/-gau löschen", "Wollen sie diesen Turnkreis/-gau wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_gaue WHERE int_gaueid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Turnkreis/-gau kann nicht gelöscht werden, da er noch einem Verein zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 11: {
            QMessageBox msg(QMessageBox::Question, "Landesverband löschen", "Wollen sie diesen Landesverband wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_verbaende WHERE int_verbaendeid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Landesverband kann nicht gelöscht werden, da er noch einem Turnkreis/-gau zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 12: {
            QMessageBox msg(QMessageBox::Question, "Land löschen", "Wollen sie dieses Land wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_laender WHERE int_laenderid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieses Land kann nicht gelöscht werden, da er noch einem Landesverband zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 13: {
            QMessageBox msg(QMessageBox::Question, "Strafe löschen", "Wollen sie diese Strafe wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_mannschaften_abzug WHERE int_mannschaften_abzugid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Abzug kann nicht gelöscht werden, da er noch in Verwendung ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = db_table->currentIndex();
                    getData();
                    db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 14: {
            QMessageBox msg(QMessageBox::Question, "Disziplinengruppe löschen", "Wollen sie diese Disziplinengruppe wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_disziplinen_gruppen WHERE int_disziplinen_gruppenid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                QModelIndex sel = db_table->currentIndex();
                getData();
                db_table->setCurrentIndex(sel);
            }
        }; break;
    case 15: {
            QMessageBox msg(QMessageBox::Question, "Formel löschen", "Wollen sie diese Formel wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_formeln WHERE int_formelid=?");
                query.bindValue( 0, QVariant(db_sort_model->data(db_sort_model->index(db_table->currentIndex().row(),0))).toInt() );
                query.exec();
                QModelIndex sel = db_table->currentIndex();
                getData();
                db_table->setCurrentIndex(sel);
            }
        }; break;
    }
}

void Db_Control_Dialog::updateFilterColumn(int index) {
    db_sort_model->setFilterKeyColumn(index);
    lbl_rowcount->setText(QString::number(db_sort_model->rowCount()));
}

void Db_Control_Dialog::updateFilterText(QString text) {
    db_sort_model->setFilterRegExp(text);
    lbl_rowcount->setText(QString::number(db_sort_model->rowCount()));
}
