#include "masterdatadialog.h"
#include "athletedialog.h"
#include "bankaccountdialog.h"
#include "clubdialog.h"
#include "countrydialog.h"
#include "disciplinedialog.h"
#include "disciplinegroupdialog.h"
#include "divisiondialog.h"
#include "formuladialog.h"
#include "penaltydialog.h"
#include "persondialog.h"
#include "regiondialog.h"
#include "sportdialog.h"
#include "src/global/header/_global.h"
#include "statedialog.h"
#include "statusdialog.h"
#include "ui_masterdatadialog.h"
#include "venuedialog.h"
#include <QActionGroup>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QToolBar>

MasterdataDialog::MasterdataDialog(QWidget *parent, int type)
    : QMainWindow(parent)
    , ui(new Ui::MasterdataDialog)
{
    ui->setupUi(this);
    QToolBar *tb = new QToolBar();
    tb->setAllowedAreas(Qt::LeftToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setOrientation(Qt::Vertical);
    tb->addAction(ui->act_sportler);
    tb->addAction(ui->act_vereine);
    tb->addAction(ui->act_disziplinen);
    tb->addAction(ui->act_sport);
    tb->addAction(ui->act_bereiche);
    tb->addAction(ui->act_personen);
    tb->addAction(ui->act_status);
    tb->addAction(ui->act_konten);
    tb->addAction(ui->act_orte);
    tb->addAction(ui->act_gaue);
    tb->addAction(ui->act_verbaende);
    tb->addAction(ui->act_countries);
    tb->addAction(ui->act_penalty);
    tb->addAction(ui->act_disgruppe);
    tb->addAction(ui->act_formel);
    ui->sidebar->layout()->addWidget(tb);
    QActionGroup *ag = new QActionGroup(this);
    ag->addAction(ui->act_sportler);
    ag->addAction(ui->act_vereine);
    ag->addAction(ui->act_disziplinen);
    ag->addAction(ui->act_sport);
    ag->addAction(ui->act_bereiche);
    ag->addAction(ui->act_personen);
    ag->addAction(ui->act_status);
    ag->addAction(ui->act_konten);
    ag->addAction(ui->act_orte);
    ag->addAction(ui->act_gaue);
    ag->addAction(ui->act_verbaende);
    ag->addAction(ui->act_countries);
    ag->addAction(ui->act_penalty);
    ag->addAction(ui->act_disgruppe);
    ag->addAction(ui->act_formel);
    db_model = new QSqlQueryModel();
    db_sort_model = new QSortFilterProxyModel();
    db_sort_model->setSourceModel(db_model);
    db_sort_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->db_table->setModel(db_sort_model);
    switch (type) {
    case 1:
        ui->act_sportler->setChecked(true);
        break;
    case 2:
        ui->act_vereine->setChecked(true);
        break;
    case 3:
        ui->act_disziplinen->setChecked(true);
        break;
    case 4:
        ui->act_sport->setChecked(true);
        break;
    case 5:
        ui->act_bereiche->setChecked(true);
        break;
    case 6:
        ui->act_personen->setChecked(true);
        break;
    case 7:
        ui->act_status->setChecked(true);
        break;
    case 8:
        ui->act_konten->setChecked(true);
        break;
    case 9:
        ui->act_orte->setChecked(true);
        break;
    case 10:
        ui->act_gaue->setChecked(true);
        break;
    case 11:
        ui->act_verbaende->setChecked(true);
        break;
    case 12:
        ui->act_countries->setChecked(true);
        break;
    case 13:
        ui->act_penalty->setChecked(true);
        break;
    case 14:
        ui->act_disgruppe->setChecked(true);
        break;
    case 15:
        ui->act_formel->setChecked(true);
        break;
    }
    connect(ui->cmb_filter, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilterColumn(int)));
    connect(ui->txt_filter, SIGNAL(textChanged(QString)), this, SLOT(updateFilterText(QString)));
    connect(ui->act_sportler, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_vereine, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_disziplinen, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_sport, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_bereiche, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_personen, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_status, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_konten, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_orte, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_gaue, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_verbaende, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_countries, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_penalty, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_disgruppe, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->act_formel, SIGNAL(triggered()), this, SLOT(loaddb()));
    connect(ui->but_add, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->but_edit, SIGNAL(clicked()), this, SLOT(edit()));
    connect(ui->but_del, SIGNAL(clicked()), this, SLOT(del()));
    connect(ui->db_table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit()));
    loaddb();
}

MasterdataDialog::~MasterdataDialog()
{
    delete ui;
}

void MasterdataDialog::loaddb()
{
    if (ui->act_sportler->isChecked()) {
        currtype = 1;
    } else if (ui->act_vereine->isChecked()) {
        currtype = 2;
    } else if (ui->act_disziplinen->isChecked()) {
        currtype = 3;
    } else if (ui->act_sport->isChecked()) {
        currtype = 4;
    } else if (ui->act_bereiche->isChecked()) {
        currtype = 5;
    } else if (ui->act_personen->isChecked()) {
        currtype = 6;
    } else if (ui->act_status->isChecked()) {
        currtype = 7;
    } else if (ui->act_konten->isChecked()) {
        currtype = 8;
    } else if (ui->act_orte->isChecked()) {
        currtype = 9;
    } else if (ui->act_gaue->isChecked()) {
        currtype = 10;
    } else if (ui->act_verbaende->isChecked()) {
        currtype = 11;
    } else if (ui->act_countries->isChecked()) {
        currtype = 12;
    } else if (ui->act_penalty->isChecked()) {
        currtype = 13;
    } else if (ui->act_disgruppe->isChecked()) {
        currtype = 14;
    } else if (ui->act_formel->isChecked()) {
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
    ui->cmb_filter->clear();
    for (int i=0;i<4;i++) {
        db_model->setHeaderData(i, Qt::Horizontal, headers[i]);
        if (headers[i].length() > 0) {
            ui->cmb_filter->addItem(headers[i], i);
        }
        ui->db_table->horizontalHeader()->setSectionResizeMode(i, resizeMode[i]);
        ui->db_table->horizontalHeader()->resizeSection(i, resize[i]);
    }
    ui->txt_filter->setText("");
}
void MasterdataDialog::getData()
{
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
    ui->lbl_rowcount->setText(QString::number(db_sort_model->rowCount()));
}

void MasterdataDialog::add()
{
    QDialog *dia;
    switch (currtype) {
    default:
    case 1:
        dia = new AthleteDialog(0, this);
        break;
    case 2:
        dia = new ClubDialog(0, this);
        break;
    case 3:
        dia = new DisciplineDialog(0, this);
        break;
    case 4:
        dia = new SportDialog(0, this);
        break;
    case 5:
        dia = new DivisionDialog(0, this);
        break;
    case 6:
        dia = new PersonDialog(0, this);
        break;
    case 7:
        dia = new StatusDialog(0, this);
        break;
    case 8:
        dia = new BankAccountDialog(0, this);
        break;
    case 9:
        dia = new VenueDialog(0, this);
        break;
    case 10:
        dia = new RegionDialog(0, this);
        break;
    case 11:
        dia = new StateDialog(0, this);
        break;
    case 12:
        dia = new CountryDialog(0, this);
        break;
    case 13:
        dia = new PenaltyDialog(0, this);
        break;
    case 14:
        dia = new DisciplineGroupDialog(0, this);
        break;
    case 15:
        dia = new FormulaDialog(0, this);
        break;
    }
    if(dia->exec() == 1) {
        getData();
    }
}

void MasterdataDialog::edit()
{
    QDialog *dia;
    int id = QVariant(
                 db_sort_model->data(db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                 .toInt();
    switch (currtype) {
    default:
    case 1:
        dia = new AthleteDialog(id, this);
        break;
    case 2:
        dia = new ClubDialog(id, this);
        break;
    case 3:
        dia = new DisciplineDialog(id, this);
        break;
    case 4:
        dia = new SportDialog(id, this);
        break;
    case 5:
        dia = new DivisionDialog(id, this);
        break;
    case 6:
        dia = new PersonDialog(id, this);
        break;
    case 7:
        dia = new StatusDialog(id, this);
        break;
    case 8:
        dia = new BankAccountDialog(id, this);
        break;
    case 9:
        dia = new VenueDialog(id, this);
        break;
    case 10:
        dia = new RegionDialog(id, this);
        break;
    case 11:
        dia = new StateDialog(id, this);
        break;
    case 12:
        dia = new CountryDialog(id, this);
        break;
    case 13:
        dia = new PenaltyDialog(id, this);
        break;
    case 14:
        dia = new DisciplineGroupDialog(id, this);
        break;
    case 15:
        dia = new FormulaDialog(id, this);
        break;
    }
    if(dia->exec() == 1) {
        QModelIndex sel = ui->db_table->currentIndex();
        getData();
        ui->db_table->setCurrentIndex(sel);
    }
}

void MasterdataDialog::del()
{
    switch (currtype) {
    case 1: {
            QMessageBox msg(QMessageBox::Question, "Teilnehmer löschen", "Wollen sie diesen Teilnehmer wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_teilnehmer WHERE int_teilnehmerid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Teilnehmer kann nicht gelöscht werden, da er noch einem Wettkampf zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 2: {
            QMessageBox msg(QMessageBox::Question, "Verein löschen", "Wollen sie diesen Verein wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_vereine WHERE int_vereineid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Verein kann nicht gelöscht werden, da er noch einem Teilnehmer zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 3: {
            QMessageBox msg(QMessageBox::Question, "Disziplin löschen", "Wollen sie diese Disziplin wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("SELECT * FROM tfx_wettkaempfe_x_disziplinen WHERE int_disziplinenid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (_global::querySize(query) == 0) {
                    query.prepare("DELETE FROM tfx_disziplinen_felder WHERE int_disziplinenid=?");
                    query.bindValue(0,
                                    QVariant(
                                        db_sort_model->data(
                                            db_sort_model->index(ui->db_table->currentIndex().row(),
                                                                 0)))
                                        .toInt());
                    query.exec();
                    query.prepare("DELETE FROM tfx_disziplinen WHERE int_disziplinenid=?");
                    query.bindValue(0,
                                    QVariant(
                                        db_sort_model->data(
                                            db_sort_model->index(ui->db_table->currentIndex().row(),
                                                                 0)))
                                        .toInt());
                    query.exec();
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
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
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Sport kann nicht gelöscht werden, da er noch zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 5: {
            QMessageBox msg(QMessageBox::Question, "Bereich löschen", "Wollen sie diesen Bereich wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_bereiche WHERE int_bereicheid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Bereich kann nicht gelöscht werden, da er noch zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 6: {
            QMessageBox msg(QMessageBox::Question, "Person löschen", "Wollen sie diese Person wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_personen WHERE int_personenid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Diese Person kann nicht gelöscht werden, da sie noch zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 7: {
            QMessageBox msg(QMessageBox::Question, "Status löschen", "Wollen sie diesen Status wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_status WHERE int_statusid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Status kann nicht gelöscht werden, da er noch zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 8: {
            QMessageBox msg(QMessageBox::Question, "Konto löschen", "Wollen sie dieses Konto wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_konten WHERE int_kontenid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieses Konto kann nicht gelöscht werden, da es noch einem Wettkampf zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 9: {
            QMessageBox msg(QMessageBox::Question, "Wettkampfort löschen", "Wollen sie diesen Wettkampfort wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_wettkampforte WHERE int_wettkampforteid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Wettkampfort kann nicht gelöscht werden, da er noch einem Wettkampf zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 10: {
            QMessageBox msg(QMessageBox::Question, "Turnkreis/-gau löschen", "Wollen sie diesen Turnkreis/-gau wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_gaue WHERE int_gaueid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Turnkreis/-gau kann nicht gelöscht werden, da er noch einem Verein zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 11: {
            QMessageBox msg(QMessageBox::Question, "Landesverband löschen", "Wollen sie diesen Landesverband wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_verbaende WHERE int_verbaendeid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Landesverband kann nicht gelöscht werden, da er noch einem Turnkreis/-gau zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 12: {
            QMessageBox msg(QMessageBox::Question, "Land löschen", "Wollen sie dieses Land wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_laender WHERE int_laenderid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieses Land kann nicht gelöscht werden, da er noch einem Landesverband zugeordnet ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 13: {
            QMessageBox msg(QMessageBox::Question, "Strafe löschen", "Wollen sie diese Strafe wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_mannschaften_abzug WHERE int_mannschaften_abzugid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                if (query.numRowsAffected() == -1) {
                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Abzug kann nicht gelöscht werden, da er noch in Verwendung ist!",QMessageBox::Ok);
                    msg.exec();
                } else {
                    QModelIndex sel = ui->db_table->currentIndex();
                    getData();
                    ui->db_table->setCurrentIndex(sel);
                }
            }
        }; break;
    case 14: {
            QMessageBox msg(QMessageBox::Question, "Disziplinengruppe löschen", "Wollen sie diese Disziplinengruppe wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_disziplinen_gruppen WHERE int_disziplinen_gruppenid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                QModelIndex sel = ui->db_table->currentIndex();
                getData();
                ui->db_table->setCurrentIndex(sel);
            }
        }; break;
    case 15: {
            QMessageBox msg(QMessageBox::Question, "Formel löschen", "Wollen sie diese Formel wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_formeln WHERE int_formelid=?");
                query.bindValue(0,
                                QVariant(
                                    db_sort_model->data(
                                        db_sort_model->index(ui->db_table->currentIndex().row(), 0)))
                                    .toInt());
                query.exec();
                QModelIndex sel = ui->db_table->currentIndex();
                getData();
                ui->db_table->setCurrentIndex(sel);
            }
        }; break;
    }
}

void MasterdataDialog::updateFilterColumn(int index)
{
    db_sort_model->setFilterKeyColumn(index);
    ui->lbl_rowcount->setText(QString::number(db_sort_model->rowCount()));
}

void MasterdataDialog::updateFilterText(QString text)
{
    db_sort_model->setFilterRegExp(text);
    ui->lbl_rowcount->setText(QString::number(db_sort_model->rowCount()));
}
