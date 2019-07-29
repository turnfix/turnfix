#include "masterdatadialog.h"
#include "athletedialog.h"
#include "bankaccountdialog.h"
#include "clubdialog.h"
#include "countrydialog.h"
#include "disciplinedialog.h"
#include "disciplinegroupdialog.h"
#include "divisiondialog.h"
#include "formuladialog.h"
#include "model/entity/athlete.h"
#include "model/entity/bankaccount.h"
#include "model/entity/club.h"
#include "model/entity/discipline.h"
#include "model/entity/division.h"
#include "model/entity/person.h"
#include "model/entity/venue.h"
#include "model/entitymanager.h"
#include "model/repository/formularepository.h"
#include "model/repository/sportrepository.h"
#include "model/view/disciplinemodel.h"
#include "model/view/formulamodel.h"
#include "model/view/sportmodel.h"
#include "penaltydialog.h"
#include "persondialog.h"
#include "regiondialog.h"
#include "sportdialog.h"
#include "statedialog.h"
#include "statusdialog.h"
#include "ui_masterdatadialog.h"
#include "venuedialog.h"
#include <QActionGroup>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QToolBar>

MasterdataDialog::MasterdataDialog(EntityManager *em, QWidget *parent, Type type)
    : QMainWindow(parent)
    , ui(new Ui::MasterdataDialog)
    , m_em(em)
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

    m_sortModel = new QSortFilterProxyModel();
    m_sortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->db_table->setModel(m_sortModel);

    switch (type) {
    case Type::AthleteData:
        ui->act_sportler->setChecked(true);
        break;
    case Type::ClubData:
        ui->act_vereine->setChecked(true);
        break;
    case Type::DisciplineData:
        ui->act_disziplinen->setChecked(true);
        break;
    case Type::SportData:
        ui->act_sport->setChecked(true);
        break;
    case Type::DivisionData:
        ui->act_bereiche->setChecked(true);
        break;
    case Type::PersonData:
        ui->act_personen->setChecked(true);
        break;
    case Type::StatusData:
        ui->act_status->setChecked(true);
        break;
    case Type::BankAccountData:
        ui->act_konten->setChecked(true);
        break;
    case Type::VenueData:
        ui->act_orte->setChecked(true);
        break;
    case Type::RegionData:
        ui->act_gaue->setChecked(true);
        break;
    case Type::StateData:
        ui->act_verbaende->setChecked(true);
        break;
    case Type::CountryData:
        ui->act_countries->setChecked(true);
        break;
    case Type::PenaltyData:
        ui->act_penalty->setChecked(true);
        break;
    case Type::DisciplineGroupData:
        ui->act_disgruppe->setChecked(true);
        break;
    case Type::FormulaData:
        ui->act_formel->setChecked(true);
        break;
    }

    connect(ui->cmb_filter, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilterColumn(int)));
    connect(ui->txt_filter, SIGNAL(textChanged(QString)), this, SLOT(updateFilterText(QString)));

    connect(ui->act_sportler, &QAction::triggered, this, [this]() {
        updateModel(Type::AthleteData);
    });
    connect(ui->act_vereine, &QAction::triggered, this, [this]() { updateModel(Type::ClubData); });
    connect(ui->act_disziplinen, &QAction::triggered, this, [this]() {
        updateModel(Type::DisciplineData);
    });
    connect(ui->act_sport, &QAction::triggered, this, [this]() { updateModel(Type::SportData); });
    connect(ui->act_bereiche, &QAction::triggered, this, [this]() {
        updateModel(Type::DivisionData);
    });
    connect(ui->act_personen, &QAction::triggered, this, [this]() {
        updateModel(Type::PersonData);
    });
    connect(ui->act_status, &QAction::triggered, this, [this]() { updateModel(Type::StatusData); });
    connect(ui->act_konten, &QAction::triggered, this, [this]() {
        updateModel(Type::BankAccountData);
    });
    connect(ui->act_orte, &QAction::triggered, this, [this]() { updateModel(Type::VenueData); });
    connect(ui->act_gaue, &QAction::triggered, this, [this]() { updateModel(Type::RegionData); });
    connect(ui->act_verbaende, &QAction::triggered, this, [this]() {
        updateModel(Type::StateData);
    });
    connect(ui->act_countries, &QAction::triggered, this, [this]() {
        updateModel(Type::CountryData);
    });
    connect(ui->act_penalty, &QAction::triggered, this, [this]() {
        updateModel(Type::PenaltyData);
    });
    connect(ui->act_disgruppe, &QAction::triggered, this, [this]() {
        updateModel(Type::DisciplineGroupData);
    });
    connect(ui->act_formel, &QAction::triggered, this, [this]() { updateModel(Type::FormulaData); });

    connect(ui->but_add, &QPushButton::clicked, this, &MasterdataDialog::add);
    connect(ui->but_edit, &QPushButton::clicked, this, &MasterdataDialog::edit);
    connect(ui->but_del, &QPushButton::clicked, this, &MasterdataDialog::del);
    connect(ui->db_table, &QTableView::doubleClicked, this, &MasterdataDialog::edit);

    updateModel(type);
}

MasterdataDialog::~MasterdataDialog()
{
    delete ui;
}

void MasterdataDialog::updateModel(Type type)
{
    m_currentType = type;

    QList<int> sizes;
    QList<QHeaderView::ResizeMode> resizeModes;

    switch (type) {
    case DisciplineData: {
        auto disciplineModel = new DisciplineModel(m_em, this);
        disciplineModel->fetchDisciplines();
        m_model = disciplineModel;
        resizeModes << QHeaderView::Stretch << QHeaderView::Fixed;
        sizes << 200 << 90;
        break;
    }
    case SportData: {
        auto sportModel = new SportModel(m_em, this);
        sportModel->fetchSports();
        m_model = sportModel;
        resizeModes << QHeaderView::Stretch;
        sizes << 200;
        break;
    }
    case FormulaData: {
        auto formulaModel = new FormulaModel(m_em, this);
        formulaModel->fetchFormulas();
        m_model = formulaModel;
        resizeModes << QHeaderView::Fixed << QHeaderView::Stretch << QHeaderView::Fixed;
        sizes << 250 << 200 << 60;
        break;
    }
    }

    m_sortModel->setSourceModel(m_model);

    for (int i = 0; i < resizeModes.length(); i++) {
        ui->db_table->horizontalHeader()->setSectionResizeMode(i, resizeModes.at(i));
    }

    for (int i = 0; i < sizes.length(); i++) {
        ui->db_table->horizontalHeader()->resizeSection(i, sizes.at(i));
    }

    ui->cmb_filter->clear();
    for (int i = 0; i < m_sortModel->columnCount(); i++) {
        ui->cmb_filter->addItem(m_sortModel->headerData(i, Qt::Horizontal).toString());
    }
    ui->txt_filter->setText("");

    // getData();
    //Vereine
    //    QString headers1[4] = {"ID","Name","Geb.","Verein"};
    //    QHeaderView::ResizeMode resizeMode1[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Stretch};
    //    int resize1[] = {40,200,60,200};
    //    //Athleten
    //    QString headers2[4] = {"ID","Verein","Ansprechpartner","Website"};
    //    QHeaderView::ResizeMode resizeMode2[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch, QHeaderView::Stretch};
    //    int resize2[] = {40,200,200,200};
    //    //Bereiche
    //    QString headers5[4] = {"ID","Name","",""};
    //    QHeaderView::ResizeMode resizeMode5[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    //    int resize5[] = {40,200,0,0};
    //    //Personen
    //    QString headers6[4] = {"ID","Name","Telefon","Email"};
    //    QHeaderView::ResizeMode resizeMode6[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch, QHeaderView::Stretch};
    //    int resize6[] = {40,200,200,200};
    //    //Status
    //    QString headers7[4] = {"ID","Name","",""};
    //    QHeaderView::ResizeMode resizeMode7[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    //    int resize7[] = {40,200,0,0};
    //    //Konten
    //    QString headers8[4] = {"ID","Name","Kontonummer","BLZ"};
    //    QHeaderView::ResizeMode resizeMode8[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch, QHeaderView::Stretch};
    //    int resize8[] = {40,200,200,200};
    //    //Wettkampforte
    //    QString headers9[4] = {"ID","Name","PLZ","Ort"};
    //    QHeaderView::ResizeMode resizeMode9[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Stretch};
    //    int resize9[] = {40,200,60,200};
    //    //Gaue
    //    QString headers10[4] = {"ID","Turngau/-kreis","Kürzel",""};
    //    QHeaderView::ResizeMode resizeMode10[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::Fixed};
    //    int resize10[] = {40,200,70,0};
    //    //Verbände
    //    QString headers11[4] = {"ID","Landesverband","Kürzel",""};
    //    QHeaderView::ResizeMode resizeMode11[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::Fixed};
    //    int resize11[] = {40,200,70,0};
    //    //Länder
    //    QString headers12[4] = {"ID","Land","Kürzel",""};
    //    QHeaderView::ResizeMode resizeMode12[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::Fixed};
    //    int resize12[] = {40,200,70,0};
    //    //Strafen
    //    QString headers13[4] = {"ID","Strafe","Abzug",""};
    //    QHeaderView::ResizeMode resizeMode13[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::ResizeToContents, QHeaderView::Fixed};
    //    int resize13[] = {40,200,70,0};
    //    //Dis-Gruppen
    //    QString headers14[4] = {"ID","Name","Kommentar",""};
    //    QHeaderView::ResizeMode resizeMode14[] = {QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed};
    //    int resize14[] = {40,250,200,0};
}
//void MasterdataDialog::getData()
//{
//    QSqlQuery query;
//    switch (m_currentType) {
//    case 1: {
//            query.prepare("SELECT int_teilnehmerid, var_nachname || ', ' || var_vorname, "+_global::date("dat_geburtstag",4)+", var_name FROM tfx_teilnehmer INNER JOIN tfx_vereine USING (int_vereineid) ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", var_name, var_nachname, var_vorname");

//        } break;
//    case 2: {
//            query.prepare("SELECT int_vereineid, tfx_vereine.var_name, var_vorname || ' ' || var_nachname, var_website FROM tfx_vereine LEFT JOIN tfx_personen USING (int_personenid) ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", var_name");

//        } break;
//    case 3: query.prepare("SELECT int_disziplinenid, tfx_disziplinen.var_name, tfx_sport.var_name, CASE WHEN bol_m = 'true' AND bol_w = 'true' THEN 'm/w' ELSE CASE WHEN bol_m = 'true' THEN 'm' ELSE 'w' END END FROM tfx_disziplinen INNER JOIN tfx_sport USING (int_sportid) ORDER BY tfx_sport.var_name, tfx_disziplinen.var_name"); break;
//    case 4: query.prepare("SELECT int_sportid, var_name, '', '' FROM tfx_sport ORDER BY var_name"); break;
//    case 5: query.prepare("SELECT int_bereicheid, var_name, '', '' FROM tfx_bereiche ORDER BY var_name"); break;
//    case 6: query.prepare("SELECT int_personenid, var_nachname || ', ' || var_vorname, var_telefon, var_email FROM tfx_personen ORDER BY var_nachname, var_vorname"); break;
//    case 7: query.prepare("SELECT int_statusid, var_name, '', '' FROM tfx_status ORDER BY var_name"); break;
//    case 8: query.prepare("SELECT int_kontenid, var_name, var_kontonummer, var_blz FROM tfx_konten ORDER BY var_name"); break;
//    case 9: query.prepare("SELECT int_wettkampforteid, var_name, var_plz, var_ort FROM tfx_wettkampforte ORDER BY var_name, var_ort"); break;
//    case 10: query.prepare("SELECT int_gaueid, var_name, var_kuerzel, '' FROM tfx_gaue ORDER BY var_name"); break;
//    case 11: query.prepare("SELECT int_verbaendeid, var_name, var_kuerzel, '' FROM tfx_verbaende ORDER BY var_name"); break;
//    case 12: query.prepare("SELECT int_laenderid, var_name, var_kuerzel, '' FROM tfx_laender ORDER BY var_name"); break;
//    case 13: query.prepare("SELECT int_mannschaften_abzugid, var_name, rel_abzug, '' FROM tfx_mannschaften_abzug ORDER BY var_name"); break;
//    case 14: query.prepare("SELECT int_disziplinen_gruppenid, var_name, txt_comment, '' FROM tfx_disziplinen_gruppen ORDER BY var_name"); break;
//    case 15: query.prepare("SELECT int_formelid, var_name, var_formel, int_typ FROM tfx_formeln ORDER BY var_name"); break;
//    }
//    query.exec();
//    m_model->setQuery(query);
//    ui->lbl_rowcount->setText(QString::number(m_sortModel->rowCount()));
//}

void MasterdataDialog::add()
{
    switch (m_currentType) {
        //    case 1: {
        //        dia = new AthleteDialog(nullptr, m_em, this);
        //        break;
        //    case 2:
        //        dia = new ClubDialog(nullptr, m_em, this);
        //        break;
    case DisciplineData: {
        auto dialog = new DisciplineDialog(nullptr, m_em, this);
        auto model = static_cast<DisciplineModel *>(m_model);
        if (dialog->exec() == 1) {
            model->fetchDisciplines();
        }
        break;
    }
    case SportData: {
        auto dialog = new SportDialog(nullptr, m_em, this);
        auto model = static_cast<SportModel *>(m_model);
        if (dialog->exec() == 1) {
            model->fetchSports();
        }
        break;
    }
    case FormulaData: {
        auto dialog = new FormulaDialog(nullptr, m_em, this);
        auto model = static_cast<FormulaModel *>(m_model);
        if (dialog->exec() == 1) {
            model->fetchFormulas();
        }
        break;
    }
        //    case 5:
        //        dia = new DivisionDialog(nullptr, m_em, this);
        //        break;
        //    case 6:
        //        dia = new PersonDialog(nullptr, m_em, this);
        //        break;
        //    case 7:
        //        dia = new StatusDialog(0, this);
        //        break;
        //    case 8:
        //        dia = new BankAccountDialog(nullptr, m_em, this);
        //        break;
        //    case 9:
        //        dia = new VenueDialog(nullptr, m_em, this);
        //        break;
        //    case 10:
        //        dia = new RegionDialog(0, this);
        //        break;
        //    case 11:
        //        dia = new StateDialog(0, this);
        //        break;
        //    case 12:
        //        dia = new CountryDialog(0, this);
        //        break;
        //    case 13:
        //        dia = new PenaltyDialog(0, this);
        //        break;
        //    case 14:
        //        dia = new DisciplineGroupDialog(0, this);
        //        break;
        //    case 15:
        //        dia = new FormulaDialog(0, this);
        //        break;
    }
}

void MasterdataDialog::edit()
{
    QVariant obj = m_sortModel->data(ui->db_table->currentIndex(), Qt::UserRole);

    switch (m_currentType) {
        //    case 1: {
        //        dia = new AthleteDialog(nullptr, m_em, this);
        //        break;
        //    case 2:
        //        dia = new ClubDialog(nullptr, m_em, this);
        //        break;
    case DisciplineData: {
        auto discipline = qvariant_cast<Discipline *>(obj);
        auto dialog = new DisciplineDialog(discipline, m_em, this);
        auto model = static_cast<DisciplineModel *>(m_model);
        if (dialog->exec() == 1) {
            model->fetchDisciplines();
        }
        break;
    }
    case SportData: {
        auto sport = qvariant_cast<Sport *>(obj);
        auto dialog = new SportDialog(sport, m_em, this);
        auto model = static_cast<SportModel *>(m_model);
        if (dialog->exec() == 1) {
            model->fetchSports();
        }
        break;
    }
    case FormulaData: {
        auto formula = qvariant_cast<Formula *>(obj);
        auto dialog = new FormulaDialog(formula, m_em, this);
        auto model = static_cast<FormulaModel *>(m_model);
        if (dialog->exec() == 1) {
            model->fetchFormulas();
        }
        break;
    }
    }
}

void MasterdataDialog::del()
{
    QVariant obj = m_sortModel->data(ui->db_table->currentIndex(), Qt::UserRole);
    switch (m_currentType) {
    case 1: {
        //            QMessageBox msg(QMessageBox::Question, "Teilnehmer löschen", "Wollen sie diesen Teilnehmer wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("DELETE FROM tfx_teilnehmer WHERE int_teilnehmerid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                if (query.numRowsAffected() == -1) {
        //                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Teilnehmer kann nicht gelöscht werden, da er noch einem Wettkampf zugeordnet ist!",QMessageBox::Ok);
        //                    msg.exec();
        //                } else {
        //                    QModelIndex sel = ui->db_table->currentIndex();
        //                    getData();
        //                    ui->db_table->setCurrentIndex(sel);
        //                }
        //            }
    }; break;
    case 2: {
        //            QMessageBox msg(QMessageBox::Question, "Verein löschen", "Wollen sie diesen Verein wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("DELETE FROM tfx_vereine WHERE int_vereineid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                if (query.numRowsAffected() == -1) {
        //                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Verein kann nicht gelöscht werden, da er noch einem Teilnehmer zugeordnet ist!",QMessageBox::Ok);
        //                    msg.exec();
        //                } else {
        //                    QModelIndex sel = ui->db_table->currentIndex();
        //                    getData();
        //                    ui->db_table->setCurrentIndex(sel);
        //                }
        //            }
    }; break;
    case 3: {
        //            QMessageBox msg(QMessageBox::Question, "Disziplin löschen", "Wollen sie diese Disziplin wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("SELECT * FROM tfx_wettkaempfe_x_disziplinen WHERE int_disziplinenid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                if (_global::querySize(query) == 0) {
        //                    query.prepare("DELETE FROM tfx_disziplinen_felder WHERE int_disziplinenid=?");
        //                    query.bindValue(0,
        //                                    QVariant(
        //                                        m_sortModel->data(
        //                                            m_sortModel->index(ui->db_table->currentIndex().row(),
        //                                                               0)))
        //                                        .toInt());
        //                    query.exec();
        //                    query.prepare("DELETE FROM tfx_disziplinen WHERE int_disziplinenid=?");
        //                    query.bindValue(0,
        //                                    QVariant(
        //                                        m_sortModel->data(
        //                                            m_sortModel->index(ui->db_table->currentIndex().row(),
        //                                                               0)))
        //                                        .toInt());
        //                    query.exec();
        //                    QModelIndex sel = ui->db_table->currentIndex();
        //                    getData();
        //                    ui->db_table->setCurrentIndex(sel);
        //                } else {
        //                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Diese Disziplin kann nicht gelöscht werden, da sie noch in einem Wettkampf verwendet wird!",QMessageBox::Ok);
        //                    msg.exec();
        //                }
        //            }
    }; break;
    case SportData: {
        QMessageBox msg(QMessageBox::Question,
                        tr("Sport löschen"),
                        tr("Wollen sie diesen Sport wirklich löschen?"),
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            auto sport = qvariant_cast<Sport *>(obj);
            if (m_em->sportRepository()->remove(sport)) {
                static_cast<SportModel *>(m_model)->fetchSports();
            } else {
                QMessageBox
                    msg(QMessageBox::Information,
                        "Fehler!",
                        "Dieser Sport kann nicht gelöscht werden, da er noch zugeordnet ist!",
                        QMessageBox::Ok);
                msg.exec();
            }
        }
    }; break;
    case 5: {
        //            QMessageBox msg(QMessageBox::Question, "Bereich löschen", "Wollen sie diesen Bereich wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("DELETE FROM tfx_bereiche WHERE int_bereicheid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                if (query.numRowsAffected() == -1) {
        //                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Bereich kann nicht gelöscht werden, da er noch zugeordnet ist!",QMessageBox::Ok);
        //                    msg.exec();
        //                } else {
        //                    QModelIndex sel = ui->db_table->currentIndex();
        //                    getData();
        //                    ui->db_table->setCurrentIndex(sel);
        //                }
        //            }
    }; break;
    case 6: {
        //            QMessageBox msg(QMessageBox::Question, "Person löschen", "Wollen sie diese Person wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("DELETE FROM tfx_personen WHERE int_personenid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                if (query.numRowsAffected() == -1) {
        //                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Diese Person kann nicht gelöscht werden, da sie noch zugeordnet ist!",QMessageBox::Ok);
        //                    msg.exec();
        //                } else {
        //                    QModelIndex sel = ui->db_table->currentIndex();
        //                    getData();
        //                    ui->db_table->setCurrentIndex(sel);
        //                }
        //            }
    }; break;
    case 7: {
        //            QMessageBox msg(QMessageBox::Question, "Status löschen", "Wollen sie diesen Status wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("DELETE FROM tfx_status WHERE int_statusid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                if (query.numRowsAffected() == -1) {
        //                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Status kann nicht gelöscht werden, da er noch zugeordnet ist!",QMessageBox::Ok);
        //                    msg.exec();
        //                } else {
        //                    QModelIndex sel = ui->db_table->currentIndex();
        //                    getData();
        //                    ui->db_table->setCurrentIndex(sel);
        //                }
        //            }
    }; break;
    case 8: {
        //            QMessageBox msg(QMessageBox::Question, "Konto löschen", "Wollen sie dieses Konto wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("DELETE FROM tfx_konten WHERE int_kontenid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                if (query.numRowsAffected() == -1) {
        //                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieses Konto kann nicht gelöscht werden, da es noch einem Wettkampf zugeordnet ist!",QMessageBox::Ok);
        //                    msg.exec();
        //                } else {
        //                    QModelIndex sel = ui->db_table->currentIndex();
        //                    getData();
        //                    ui->db_table->setCurrentIndex(sel);
        //                }
        //            }
    }; break;
    case 9: {
        //            QMessageBox msg(QMessageBox::Question, "Wettkampfort löschen", "Wollen sie diesen Wettkampfort wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("DELETE FROM tfx_wettkampforte WHERE int_wettkampforteid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                if (query.numRowsAffected() == -1) {
        //                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Wettkampfort kann nicht gelöscht werden, da er noch einem Wettkampf zugeordnet ist!",QMessageBox::Ok);
        //                    msg.exec();
        //                } else {
        //                    QModelIndex sel = ui->db_table->currentIndex();
        //                    getData();
        //                    ui->db_table->setCurrentIndex(sel);
        //                }
        //            }
    }; break;
    case 10: {
        //            QMessageBox msg(QMessageBox::Question, "Turnkreis/-gau löschen", "Wollen sie diesen Turnkreis/-gau wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("DELETE FROM tfx_gaue WHERE int_gaueid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                if (query.numRowsAffected() == -1) {
        //                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Turnkreis/-gau kann nicht gelöscht werden, da er noch einem Verein zugeordnet ist!",QMessageBox::Ok);
        //                    msg.exec();
        //                } else {
        //                    QModelIndex sel = ui->db_table->currentIndex();
        //                    getData();
        //                    ui->db_table->setCurrentIndex(sel);
        //                }
        //            }
    }; break;
    case 11: {
        //            QMessageBox msg(QMessageBox::Question, "Landesverband löschen", "Wollen sie diesen Landesverband wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("DELETE FROM tfx_verbaende WHERE int_verbaendeid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                if (query.numRowsAffected() == -1) {
        //                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Landesverband kann nicht gelöscht werden, da er noch einem Turnkreis/-gau zugeordnet ist!",QMessageBox::Ok);
        //                    msg.exec();
        //                } else {
        //                    QModelIndex sel = ui->db_table->currentIndex();
        //                    getData();
        //                    ui->db_table->setCurrentIndex(sel);
        //                }
        //            }
    }; break;
    case 12: {
        //            QMessageBox msg(QMessageBox::Question, "Land löschen", "Wollen sie dieses Land wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("DELETE FROM tfx_laender WHERE int_laenderid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                if (query.numRowsAffected() == -1) {
        //                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieses Land kann nicht gelöscht werden, da er noch einem Landesverband zugeordnet ist!",QMessageBox::Ok);
        //                    msg.exec();
        //                } else {
        //                    QModelIndex sel = ui->db_table->currentIndex();
        //                    getData();
        //                    ui->db_table->setCurrentIndex(sel);
        //                }
        //            }
    }; break;
    case 13: {
        //            QMessageBox msg(QMessageBox::Question, "Strafe löschen", "Wollen sie diese Strafe wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("DELETE FROM tfx_mannschaften_abzug WHERE int_mannschaften_abzugid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                if (query.numRowsAffected() == -1) {
        //                    QMessageBox msg(QMessageBox::Information, "Fehler!", "Dieser Abzug kann nicht gelöscht werden, da er noch in Verwendung ist!",QMessageBox::Ok);
        //                    msg.exec();
        //                } else {
        //                    QModelIndex sel = ui->db_table->currentIndex();
        //                    getData();
        //                    ui->db_table->setCurrentIndex(sel);
        //                }
        //            }
    }; break;
    case 14: {
        //            QMessageBox msg(QMessageBox::Question, "Disziplinengruppe löschen", "Wollen sie diese Disziplinengruppe wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
        //            if(msg.exec() == QMessageBox::Ok) {
        //                QSqlQuery query;
        //                query.prepare("DELETE FROM tfx_disziplinen_gruppen WHERE int_disziplinen_gruppenid=?");
        //                query.bindValue(0,
        //                                QVariant(
        //                                    m_sortModel->data(
        //                                        m_sortModel->index(ui->db_table->currentIndex().row(), 0)))
        //                                    .toInt());
        //                query.exec();
        //                QModelIndex sel = ui->db_table->currentIndex();
        //                getData();
        //                ui->db_table->setCurrentIndex(sel);
        //            }
    }; break;
    case FormulaData: {
        QMessageBox msg(QMessageBox::Question,
                        "Formel löschen",
                        "Wollen sie diese Formel wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            auto formula = qvariant_cast<Formula *>(obj);
            if (m_em->formulaRepository()->remove(formula)) {
                static_cast<FormulaModel *>(m_model)->fetchFormulas();
            } else {
                QMessageBox
                    msg(QMessageBox::Information,
                        "Fehler!",
                        "Diese Formel kann nicht gelöscht werden, da sie noch zugeordnet ist!",
                        QMessageBox::Ok);
                msg.exec();
            }
        }
    }; break;
    }
}

void MasterdataDialog::updateFilterColumn(int index)
{
    m_sortModel->setFilterKeyColumn(index);
    ui->lbl_rowcount->setText(QString::number(m_sortModel->rowCount()));
}

void MasterdataDialog::updateFilterText(QString text)
{
    m_sortModel->setFilterRegExp(text);
    ui->lbl_rowcount->setText(QString::number(m_sortModel->rowCount()));
}
