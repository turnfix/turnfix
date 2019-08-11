#include "competitiondialog.h"
#include "competitiondisciplinemodel.h"
#include "controls/doublespinboxdelegate.h"
#include "disciplineordermodel.h"
#include "masterdata/disciplinegroupmodel.h"
#include "masterdata/divisionmodel.h"
#include "model/entity/competition.h"
#include "model/entity/competitiondiscipline.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "ui_competitiondialog.h"
#include <QList>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QToolBar>

CompetitionDialog::CompetitionDialog(Competition *competition, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , m_competition(competition)
    , m_em(em)
    , ui(new Ui::CompetitionDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    auto tb = new QToolBar();
    auto ag = new QActionGroup(this);
    tb->setAllowedAreas(Qt::LeftToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setOrientation(Qt::Vertical);
    tb->addAction(ui->act_wk);
    ag->addAction(ui->act_wk);
    tb->addAction(ui->act_dis);
    ag->addAction(ui->act_dis);
    tb->addAction(ui->act_misc);
    ag->addAction(ui->act_misc);
    tb->addAction(ui->act_timetable);
    ag->addAction(ui->act_timetable);
    tb->addAction(ui->act_order);
    ag->addAction(ui->act_order);
    ui->act_wk->setChecked(true);
    ui->sidebar->layout()->addWidget(tb);

    connect(ui->act_wk, &QAction::triggered, [this]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->act_dis, &QAction::triggered, [this]() { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->act_misc, &QAction::triggered, [this]() { ui->stackedWidget->setCurrentIndex(2); });
    connect(ui->act_timetable, &QAction::triggered, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
    });
    connect(ui->act_order, &QAction::triggered, [this]() { ui->stackedWidget->setCurrentIndex(4); });

    auto divisionModel = new DivisionModel(m_em, this);
    divisionModel->fetchDivisions();
    ui->cmb_bereich->setModel(divisionModel);

    auto disciplineGroupModel = new DisciplineGroupModel(m_em, this);
    disciplineGroupModel->fetchGroups();
    ui->cmb_groups->setModel(disciplineGroupModel);

    m_competitionDisciplineModel = new CompetitionDisciplineModel(m_em, this);
    ui->tbl_disziplinen->setModel(m_competitionDisciplineModel);

    QList<QHeaderView::ResizeMode> resizeModes = {QHeaderView::Stretch,
                                                  QHeaderView::Stretch,
                                                  QHeaderView::Fixed,
                                                  QHeaderView::Stretch,
                                                  QHeaderView::Fixed,
                                                  QHeaderView::ResizeToContents};
    QList<int> sizes = {300, 250, 40, 100, 24, 50};
    for (int i = 0; i < 6; i++) {
        ui->tbl_disziplinen->horizontalHeader()->setSectionResizeMode(i, resizeModes.at(i));
        ui->tbl_disziplinen->horizontalHeader()->resizeSection(i, sizes.at(i));
    }
    ui->tbl_disziplinen->setItemDelegateForColumn(5, new DoubleSpinBoxDelegate);

    m_orderModel = new DisciplineOrderModel(m_em, this);
    ui->tbl_order->setModel(m_orderModel);

    connect(m_competitionDisciplineModel,
            &CompetitionDisciplineModel::disciplineAdded,
            m_orderModel,
            &DisciplineOrderModel::addDiscipline);
    connect(m_competitionDisciplineModel,
            &CompetitionDisciplineModel::disciplineRemoved,
            m_orderModel,
            &DisciplineOrderModel::removeDiscipline);
    connect(ui->chk_kuer,
            &QCheckBox::stateChanged,
            m_competitionDisciplineModel,
            &CompetitionDisciplineModel::updateGlobalFreeAndCompulsary);

    connect(ui->cmb_bereich, SIGNAL(currentIndexChanged(int)), this, SLOT(loadDisciplines()));
    connect(ui->cmb_typ, SIGNAL(currentIndexChanged(int)), this, SLOT(enableOptions(int)));
    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_up, SIGNAL(clicked()), this, SLOT(moveUp()));
    connect(ui->but_down, SIGNAL(clicked()), this, SLOT(moveDown()));
    connect(ui->but_mark, SIGNAL(clicked()), this, SLOT(markGroup()));
    connect(ui->but_orderup, SIGNAL(clicked()), this, SLOT(orderMoveUp()));
    connect(ui->but_orderdown, SIGNAL(clicked()), this, SLOT(orderMoveDown()));
    connect(ui->but_orderleft, SIGNAL(clicked()), this, SLOT(orderMoveLeft()));
    connect(ui->but_orderright, SIGNAL(clicked()), this, SLOT(orderMoveRight()));

    ui->cmb_bereich->setCurrentIndex(
        ui->cmb_bereich->findData(m_competition->divisionId(), TF::IdRole));

    loadDisciplines();
    enableOptions(0);

    ui->txt_wknr->setText(m_competition->number());
    ui->txt_wkbez->setText(m_competition->name());
    ui->sbx_quali->setValue(m_competition->qualifiers());
    ui->dae_wkj1->setDate(QDate().fromString(QString::number(m_competition->minYear()), "yyyy"));
    ui->dae_wkj2->setDate(QDate().fromString(QString::number(m_competition->maxYear()), "yyyy"));
    switch (m_competition->maxYear()) {
    case 1:
        ui->rab_ua->setChecked(true);
        break;
    case 2:
        ui->rab_uj->setChecked(true);
        break;
    case 3:
        ui->rab_ja->setChecked(true);
        break;
    }
    ui->cmb_typ->setCurrentIndex(m_competition->type());
    ui->sbx_wertungen->setValue(m_competition->scores());
    ui->chk_streich->setChecked(m_competition->dropScores());
    ui->chk_akjg->setChecked(m_competition->showAgeGroup());
    ui->chk_wahl->setChecked(m_competition->apparatusChoices());
    ui->sbx_durchgang->setValue(m_competition->round());
    ui->tie_start->setTime(m_competition->startTime());
    ui->sbx_bahn->setValue(m_competition->group());
    ui->tie_warmup->setTime(m_competition->warmUpTime());
    ui->chk_showinfo->setChecked(m_competition->showInfo());
    ui->chk_kuer->setChecked(m_competition->freeAndCompulsary());
    ui->chk_sortasc->setChecked(m_competition->sortAsc());
    ui->gbx_sort->setChecked(m_competition->manualSort());
    ui->chk_gpunkte->setChecked(m_competition->extraApparatusPoints());
    ui->sbx_anz->setValue(m_competition->numDropScores());
}

CompetitionDialog::~CompetitionDialog()
{
    delete ui;
}

void CompetitionDialog::enableOptions(int typ)
{
    if (typ == 1) {
        ui->sbx_wertungen->setEnabled(true);
        ui->chk_wahl->setEnabled(false);
        ui->chk_wahl->setChecked(false);
        ui->chk_gpunkte->setEnabled(true);
    } else {
        ui->sbx_wertungen->setEnabled(false);
        ui->sbx_wertungen->setValue(0);
        ui->chk_wahl->setEnabled(true);
        ui->chk_gpunkte->setEnabled(false);
    }
}

void CompetitionDialog::moveUp()
{
    auto index = ui->tbl_disziplinen->currentIndex();
    if (index.isValid()) {
        m_competitionDisciplineModel->moveUp(index);
    }
}

void CompetitionDialog::moveDown()
{
    auto index = ui->tbl_disziplinen->currentIndex();
    if (index.isValid()) {
        m_competitionDisciplineModel->moveDown(index);
    }
}

void CompetitionDialog::save()
{
    //    bool checked = false;
    //    for (int i = 0; i < model->rowCount(); i++) {
    //        if (model->item(i, 0)->checkState() == Qt::Checked) {
    //            checked = true;
    //            break;
    //        }
    //    }
    //    if (!checked) {
    //        int ret = QMessageBox::warning(
    //            this,
    //            tr("Keine Disziplinen selektiert"),
    //            tr("Wurden keine Disziplinen selektiert. Wollen sie trotzdem speichern?"),
    //            QMessageBox::Ok,
    //            QMessageBox::Cancel);
    //        if (ret != QMessageBox::Ok)
    //            return;
    //    }
    //    int j2 = 0;
    //    if (ui->rab_ua->isChecked()) {
    //        j2 = 1;
    //    } else if (ui->rab_uj->isChecked()) {
    //        j2 = 2;
    //    } else if (ui->rab_ja->isChecked()) {
    //        j2 = 3;
    //    } else {
    //        j2 = ui->dae_wkj2->date().year();
    //    }
    //    QSqlQuery query;
    //    if (editid == 0) {
    //        query.prepare("INSERT INTO tfx_wettkaempfe "
    //                      "(int_veranstaltungenid,int_bereicheid,int_typ,var_nummer,var_name,yer_von,"
    //                      "yer_bis,int_qualifikation,int_wertungen,bol_streichwertung,bol_ak_anzeigen,"
    //                      "bol_wahlwettkampf,int_durchgang,int_bahn,tim_startzeit,tim_einturnen,bol_"
    //                      "info_anzeigen,bol_kp,bol_sortasc,bol_mansort,bol_gerpkt,int_anz_streich) "
    //                      "VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    //    } else {
    //        query.prepare(
    //            "UPDATE tfx_wettkaempfe SET int_veranstaltungenid=?, int_bereicheid=?, int_typ=?, "
    //            "var_nummer=?, var_name=?, yer_von=?, yer_bis=?, int_qualifikation=?, int_wertungen=?, "
    //            "bol_streichwertung=?, bol_ak_anzeigen=?, bol_wahlwettkampf=?, int_durchgang=?, "
    //            "int_bahn=?, tim_startzeit=?, tim_einturnen=?, bol_info_anzeigen=?, bol_kp=?, "
    //            "bol_sortasc=?, bol_mansort=?, bol_gerpkt=?, int_anz_streich=? WHERE "
    //            "int_wettkaempfeid=?");
    //        query.bindValue(22, editid);
    //    }
    //    query.bindValue(0, this->m_event->mainEvent()->id());
    //    query.bindValue(1, ui->cmb_bereich->itemData(ui->cmb_bereich->currentIndex()));
    //    query.bindValue(2, ui->cmb_typ->currentIndex());
    //    query.bindValue(3, ui->txt_wknr->text());
    //    query.bindValue(4, ui->txt_wkbez->text());
    //    query.bindValue(5, ui->dae_wkj1->date().year());
    //    query.bindValue(6, j2);
    //    query.bindValue(7, ui->sbx_quali->value());
    //    query.bindValue(8, ui->sbx_wertungen->value());
    //    query.bindValue(9, ui->chk_streich->isChecked());
    //    query.bindValue(10, ui->chk_akjg->isChecked());
    //    query.bindValue(11, ui->chk_wahl->isChecked());
    //    query.bindValue(12, ui->sbx_durchgang->value());
    //    query.bindValue(13, ui->sbx_bahn->value());
    //    query.bindValue(14, ui->tie_start->time().toString("hh:mm:ss"));
    //    query.bindValue(15, ui->tie_warmup->time().toString("hh:mm:ss"));
    //    query.bindValue(16, ui->chk_showinfo->isChecked());
    //    query.bindValue(17, ui->chk_kuer->isChecked());
    //    query.bindValue(18, ui->chk_sortasc->isChecked());
    //    query.bindValue(19, ui->gbx_sort->isChecked());
    //    query.bindValue(20, ui->chk_gpunkte->isChecked());
    //    query.bindValue(21, ui->sbx_anz->value());
    //    query.exec();
    //    if (editid == 0) {
    //        if (_global::getDBTyp() == 0) {
    //            QSqlQuery query7("SELECT last_value FROM tfx_wettkaempfe_int_wettkaempfeid_seq");
    //            query7.next();
    //            editid = query7.value(0).toInt();
    //        } else {
    //            editid = query.lastInsertId().toInt();
    //        }
    //    }
    //    for (int i = 0; i < model->rowCount(); i++) {
    //        QSqlQuery query7;
    //        query7.prepare(
    //            "SELECT int_wettkaempfe_x_disziplinenid, int_disziplinenid, int_wettkaempfeid FROM "
    //            "tfx_wettkaempfe_x_disziplinen WHERE int_wettkaempfeid=? AND int_disziplinenid=?");
    //        query7.bindValue(0, editid);
    //        query7.bindValue(1, model->item(i, 8)->text());
    //        query7.exec();
    //        if (model->item(i, 0)->checkState() == Qt::Checked) {
    //            QSqlQuery query2;
    //            if (_global::querySize(query7) == 0) {
    //                query2.prepare("INSERT INTO tfx_wettkaempfe_x_disziplinen "
    //                               "(int_wettkaempfeid,int_disziplinenid,var_ausschreibung,int_"
    //                               "sortierung,bol_kp,rel_max) VALUES (?,?,?,?,?,?)");
    //            } else {
    //                query7.next();
    //                query2.prepare("UPDATE tfx_wettkaempfe_x_disziplinen SET int_wettkaempfeid=?, "
    //                               "int_disziplinenid=?, var_ausschreibung=?, int_sortierung=?, "
    //                               "bol_kp=?, rel_max=? WHERE int_wettkaempfe_x_disziplinenid=?");
    //                query2.bindValue(6, query7.value(0).toString());
    //            }
    //            query2.bindValue(0, editid);
    //            query2.bindValue(1, model->item(i, 8)->text());
    //            query2.bindValue(2, model->item(i, 5)->text());
    //            query2.bindValue(3, i);
    //            if (model->item(i, 6)->checkState() == Qt::Checked) {
    //                query2.bindValue(4, true);
    //            } else {
    //                query2.bindValue(4, false);
    //            }
    //            query2.bindValue(5, model->item(i, 7)->text().replace(",", ".").toDouble());
    //            query2.exec();
    //        } else {
    //            if (_global::querySize(query7) > 0) {
    //                query7.next();
    //                QSqlQuery query9;
    //                query9.prepare("DELETE FROM tfx_wettkaempfe_x_disziplinen WHERE "
    //                               "int_wettkaempfe_x_disziplinenid=?");
    //                query9.bindValue(0, query7.value(0).toString());
    //                query9.exec();
    //                query9.prepare("DELETE FROM tfx_quali_leistungen WHERE int_disziplinenid=? AND "
    //                               "int_wertungenid IN (SELECT int_wertungenid FROM tfx_wertungen "
    //                               "WHERE int_wettkaempfeid=?)");
    //                query9.bindValue(0, query7.value(1).toInt());
    //                query9.bindValue(1, query7.value(2).toInt());
    //                query9.exec();
    //                query9.prepare("DELETE FROM tfx_wertungen_details WHERE int_disziplinenid=? AND "
    //                               "int_wertungenid IN (SELECT int_wertungenid FROM tfx_wertungen "
    //                               "WHERE int_wettkaempfeid=?)");
    //                query9.bindValue(0, query7.value(1).toInt());
    //                query9.bindValue(1, query7.value(2).toInt());
    //                query9.exec();
    //            }
    //        }
    //    }
    //    if (ui->gbx_sort->isChecked()) {
    //        QSqlQuery query8;
    //        query8.prepare("SELECT int_wettkaempfe_x_disziplinenid, int_disziplinenid, "
    //                       "tfx_wettkaempfe_x_disziplinen.bol_kp, tfx_wettkaempfe.bol_kp FROM "
    //                       "tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_wettkaempfe USING "
    //                       "(int_wettkaempfeid) WHERE int_wettkaempfeid=?");
    //        query8.bindValue(0, editid);
    //        query8.exec();
    //        while (query8.next()) {
    //            int kpmax = 0;
    //            if (query8.value(2).toBool() || query8.value(3).toBool())
    //                kpmax = 1;
    //            for (int kp = 0; kp <= kpmax; kp++) {
    //                QSqlQuery query11;
    //                query11.prepare("SELECT int_wettkaempfe_disposid FROM tfx_wettkaempfe_dispos WHERE "
    //                                "int_wettkaempfe_x_disziplinenid=? AND int_kp=?");
    //                query11.bindValue(0, query8.value(0).toInt());
    //                query11.bindValue(1, kp);
    //                query11.exec();
    //                bool found = false;
    //                for (int i = 0; i < model2->rowCount(); i++) {
    //                    if (found)
    //                        break;
    //                    for (int j = 0; j < model2->columnCount(); j++) {
    //                        if (model2->item(i, j) == nullptr)
    //                            continue;
    //                        if (model2->item(i, j)->data().toInt() == query8.value(1).toInt()) {
    //                            if (kp == 0 || model2->item(i, j)->text().right(3) == "(K)") {
    //                                QSqlQuery query12;
    //                                if (_global::querySize(query11) == 0) {
    //                                    query12.prepare("INSERT INTO tfx_wettkaempfe_dispos "
    //                                                    "(int_wettkaempfe_x_disziplinenid,int_sortx,"
    //                                                    "int_sorty,int_kp) VALUES (?,?,?,?)");
    //                                } else {
    //                                    query11.next();
    //                                    query12.prepare(
    //                                        "UPDATE tfx_wettkaempfe_dispos SET "
    //                                        "int_wettkaempfe_x_disziplinenid=?, int_sortx=?, "
    //                                        "int_sorty=?, int_kp=? WHERE int_wettkaempfe_disposid=?");
    //                                    query12.bindValue(4, query11.value(0).toInt());
    //                                }
    //                                query12.bindValue(0, query8.value(0).toInt());
    //                                if (ui->cmb_typ->currentIndex() == 1) {
    //                                    query12.bindValue(1, model2->columnCount() - 1 - j);
    //                                } else {
    //                                    query12.bindValue(1, j);
    //                                }
    //                                query12.bindValue(2, i);
    //                                query12.bindValue(3, kp);
    //                                query12.exec();
    //                                found = true;
    //                                break;
    //                            }
    //                        }
    //                    }
    //                }
    //                if (!found) {
    //                    QSqlQuery query13;
    //                    query13.prepare("DELETE FROM tfx_wettkaempfe_dispos WHERE "
    //                                    "int_wettkaempfe_x_disziplinenid=?)");
    //                    query13.bindValue(0, query8.value(0).toInt());
    //                    query13.exec();
    //                }
    //            }
    //        }
    //    } else {
    //        QSqlQuery query10;
    //        query10.prepare("DELETE FROM tfx_wettkaempfe_dispos WHERE int_wettkaempfe_x_disziplinenid "
    //                        "IN (SELECT int_wettkaempfe_x_disziplinenid FROM "
    //                        "tfx_wettkaempfe_x_disziplinen WHERE int_wettkaempfeid=?)");
    //        query10.bindValue(0, editid);
    //        query10.exec();
    //    }
    done(1);
}

void CompetitionDialog::markGroup()
{
    //    QSqlQuery getRows;
    //    getRows.prepare("SELECT int_disziplinenid, int_pos FROM tfx_disgrp_x_disziplinen WHERE "
    //                    "int_disziplinen_gruppenid=? ORDER BY int_pos");
    //    getRows.bindValue(0, ui->cmb_groups->itemData(ui->cmb_groups->currentIndex()));
    //    getRows.exec();
    //    while (getRows.next()) {
    //        if (model->findItems(getRows.value(0).toString(), Qt::MatchExactly, 8).isEmpty())
    //            continue;
    //        int row = model->findItems(getRows.value(0).toString(), Qt::MatchExactly, 8).at(0)->row();
    //        model->insertRow(getRows.value(1).toInt(), model->takeRow(row));
    //        model->item(getRows.value(1).toInt(), 0)->setCheckState(Qt::Checked);
    //    }
}

void CompetitionDialog::loadDisciplines()
{
    QVariant currentDivision = ui->cmb_bereich->currentData();
    if (currentDivision.isValid()) {
        auto division = qvariant_cast<Division *>(currentDivision);
        m_competitionDisciplineModel->fetchDisciplines(m_competition,
                                                       division->women(),
                                                       division->men());
    } else {
        m_competitionDisciplineModel->reset();
    }
}

void CompetitionDialog::orderMoveUp()
{
    auto model = ui->tbl_order->selectionModel();
    if (model->selectedIndexes().isEmpty())
        return;

    QModelIndex selIdx = model->selectedIndexes().at(0);
    QModelIndex newIndex = m_orderModel->moveUp(selIdx);
    model->setCurrentIndex(newIndex, QItemSelectionModel::Select);
}

void CompetitionDialog::orderMoveDown()
{
    auto model = ui->tbl_order->selectionModel();
    if (model->selectedIndexes().isEmpty())
        return;

    QModelIndex selIdx = model->selectedIndexes().at(0);
    QModelIndex newIndex = m_orderModel->moveDown(selIdx);
    model->setCurrentIndex(newIndex, QItemSelectionModel::Select);
}

void CompetitionDialog::orderMoveLeft()
{
    auto model = ui->tbl_order->selectionModel();
    if (model->selectedIndexes().isEmpty())
        return;

    QModelIndex selIdx = model->selectedIndexes().at(0);
    QModelIndex newIndex = m_orderModel->moveLeft(selIdx);
    model->setCurrentIndex(newIndex, QItemSelectionModel::Select);
}

void CompetitionDialog::orderMoveRight()
{
    auto model = ui->tbl_order->selectionModel();
    if (model->selectedIndexes().isEmpty())
        return;

    QModelIndex selIdx = model->selectedIndexes().at(0);
    QModelIndex newIndex = m_orderModel->moveRight(selIdx);
    model->setCurrentIndex(newIndex, QItemSelectionModel::Select);
}
