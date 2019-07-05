#include "competitiondialog.h"
#include "model/objects/event.h"
#include "src/global/header/_delegates.h"
#include "src/global/header/_global.h"
#include "src/global/header/settings.h"
#include "ui_competitiondialog.h"
#include <QList>
#include <QMessageBox>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QToolBar>

CompetitionDialog::CompetitionDialog(Event *event, int edit, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CompetitionDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    this->event = event;
    this->editid = edit;

    auto *tb = new QToolBar();
    auto *ag = new QActionGroup(this);
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

    model = new QStandardItemModel();
    model->setColumnCount(9);
    ui->tbl_disziplinen->setModel(model);
    QHeaderView::ResizeMode resizeMode[] = {QHeaderView::Fixed,
                                            QHeaderView::Fixed,
                                            QHeaderView::Stretch,
                                            QHeaderView::Stretch,
                                            QHeaderView::Fixed,
                                            QHeaderView::Stretch,
                                            QHeaderView::Fixed,
                                            QHeaderView::ResizeToContents,
                                            QHeaderView::Fixed};
    int resize[] = {24, 24, 300, 250, 40, 100, 24, 50, 0};
    model->setHeaderData(0, Qt::Horizontal, "");
    model->setHeaderData(1, Qt::Horizontal, "");
    model->setHeaderData(2, Qt::Horizontal, tr("Sport"));
    model->setHeaderData(3, Qt::Horizontal, tr("Disziplin"));
    model->setHeaderData(4, Qt::Horizontal, tr("m/w"));
    model->setHeaderData(5, Qt::Horizontal, tr("Ausschreibungstext"));
    model->setHeaderData(6, Qt::Horizontal, tr("KP"));
    model->setHeaderData(7, Qt::Horizontal, tr("max. Pkt."));
    for (int i = 0; i < 9; i++) {
        ui->tbl_disziplinen->horizontalHeader()->setSectionResizeMode(i, resizeMode[i]);
        ui->tbl_disziplinen->horizontalHeader()->resizeSection(i, resize[i]);
    }
    ui->tbl_disziplinen->setItemDelegateForColumn(7, new DsbxDelegate);
    enableOptions(0);
    QSqlQuery query;
    query.prepare("SELECT var_name, int_bereicheid FROM tfx_bereiche ORDER BY int_bereicheid");
    query.exec();
    while (query.next()) {
        ui->cmb_bereich->addItem(query.value(0).toString(), query.value(1).toInt());
    }
    query.prepare("SELECT var_name, int_disziplinen_gruppenid FROM tfx_disziplinen_gruppen ORDER "
                  "BY var_name");
    query.exec();
    while (query.next()) {
        ui->cmb_groups->addItem(query.value(0).toString(), query.value(1).toInt());
    }

    model2 = new QStandardItemModel();
    ui->tbl_order->setModel(model2);

    connect(ui->cmb_bereich, SIGNAL(currentIndexChanged(int)), this, SLOT(fillTable()));
    connect(ui->cmb_typ, SIGNAL(currentIndexChanged(int)), this, SLOT(enableOptions(int)));
    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_up, SIGNAL(clicked()), this, SLOT(moveUp()));
    connect(ui->but_down, SIGNAL(clicked()), this, SLOT(moveDown()));
    connect(ui->but_mark, SIGNAL(clicked()), this, SLOT(markGroup()));
    connect(ui->but_orderup, SIGNAL(clicked()), this, SLOT(orderMoveUp()));
    connect(ui->but_orderdown, SIGNAL(clicked()), this, SLOT(orderMoveDown()));
    connect(ui->but_orderleft, SIGNAL(clicked()), this, SLOT(orderMoveLeft()));
    connect(ui->but_orderright, SIGNAL(clicked()), this, SLOT(orderMoveRight()));
    connect(ui->tbl_disziplinen, SIGNAL(clicked(QModelIndex)), this, SLOT(fillTable2()));
    connect(ui->chk_kuer, SIGNAL(clicked()), this, SLOT(fillTable2()));
    initData();
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

void CompetitionDialog::initData()
{
    fillTable();
    if (editid != 0) {
        QSqlQuery query;
        query.prepare("SELECT * FROM tfx_wettkaempfe WHERE int_wettkaempfeid=? LIMIT 1");
        query.bindValue(0, editid);
        query.exec();
        query.next();
        ui->txt_wknr->setText(query.value(4).toString());
        ui->txt_wkbez->setText(query.value(5).toString());
        ui->sbx_quali->setValue(query.value(8).toInt());
        ui->dae_wkj1->setDate(QDate().fromString(query.value(6).toString(), "yyyy"));
        ui->dae_wkj2->setDate(QDate().fromString(query.value(7).toString(), "yyyy"));
        switch (query.value(7).toInt()) {
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
        ui->cmb_bereich->setCurrentIndex(ui->cmb_bereich->findData(query.value(2).toInt()));
        ui->cmb_typ->setCurrentIndex(query.value(3).toInt());
        ui->sbx_wertungen->setValue(query.value(9).toInt());
        ui->chk_streich->setChecked(query.value(10).toBool());
        ui->chk_akjg->setChecked(query.value(11).toBool());
        ui->chk_wahl->setChecked(query.value(12).toBool());
        ui->sbx_durchgang->setValue(query.value(13).toInt());
        ui->tie_start->setTime(QTime().fromString(query.value(15).toString(), "hh:mm:ss"));
        ui->sbx_bahn->setValue(query.value(14).toInt());
        ui->tie_warmup->setTime(QTime().fromString(query.value(16).toString(), "hh:mm:ss"));
        ui->chk_showinfo->setChecked(query.value(17).toBool());
        ui->chk_kuer->setChecked(query.value(18).toBool());
        ui->chk_sortasc->setChecked(query.value(19).toBool());
        ui->gbx_sort->setChecked(query.value(20).toBool());
        ui->chk_gpunkte->setChecked(query.value(21).toBool());
        ui->sbx_anz->setValue(query.value(22).toInt());
        fillTable();
    }
}

void CompetitionDialog::moveUp()
{
    if (ui->tbl_disziplinen->currentIndex().isValid()
        && ui->tbl_disziplinen->currentIndex().row() > 0) {
        int insert = ui->tbl_disziplinen->currentIndex().row() - 1;
        model->insertRow(insert, model->takeRow(ui->tbl_disziplinen->currentIndex().row()));
        ui->tbl_disziplinen->selectRow(insert);
    }
}

void CompetitionDialog::moveDown()
{
    if (ui->tbl_disziplinen->currentIndex().isValid()
        && ui->tbl_disziplinen->currentIndex().row() < (model->rowCount() - 1)) {
        int insert = ui->tbl_disziplinen->currentIndex().row() + 1;
        model->insertRow(insert, model->takeRow(ui->tbl_disziplinen->currentIndex().row()));
        ui->tbl_disziplinen->selectRow(insert);
    }
}

void CompetitionDialog::save()
{
    bool checked = false;
    for (int i = 0; i < model->rowCount(); i++) {
        if (model->item(i, 0)->checkState() == Qt::Checked) {
            checked = true;
            break;
        }
    }
    if (!checked) {
        int ret = QMessageBox::warning(
            this,
            tr("Keine Disziplinen selektiert"),
            tr("Wurden keine Disziplinen selektiert. Wollen sie trotzdem speichern?"),
            QMessageBox::Ok,
            QMessageBox::Cancel);
        if (ret != QMessageBox::Ok)
            return;
    }
    int j2 = 0;
    if (ui->rab_ua->isChecked()) {
        j2 = 1;
    } else if (ui->rab_uj->isChecked()) {
        j2 = 2;
    } else if (ui->rab_ja->isChecked()) {
        j2 = 3;
    } else {
        j2 = ui->dae_wkj2->date().year();
    }
    QSqlQuery query;
    if (editid == 0) {
        query.prepare("INSERT INTO tfx_wettkaempfe "
                      "(int_veranstaltungenid,int_bereicheid,int_typ,var_nummer,var_name,yer_von,"
                      "yer_bis,int_qualifikation,int_wertungen,bol_streichwertung,bol_ak_anzeigen,"
                      "bol_wahlwettkampf,int_durchgang,int_bahn,tim_startzeit,tim_einturnen,bol_"
                      "info_anzeigen,bol_kp,bol_sortasc,bol_mansort,bol_gerpkt,int_anz_streich) "
                      "VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    } else {
        query.prepare(
            "UPDATE tfx_wettkaempfe SET int_veranstaltungenid=?, int_bereicheid=?, int_typ=?, "
            "var_nummer=?, var_name=?, yer_von=?, yer_bis=?, int_qualifikation=?, int_wertungen=?, "
            "bol_streichwertung=?, bol_ak_anzeigen=?, bol_wahlwettkampf=?, int_durchgang=?, "
            "int_bahn=?, tim_startzeit=?, tim_einturnen=?, bol_info_anzeigen=?, bol_kp=?, "
            "bol_sortasc=?, bol_mansort=?, bol_gerpkt=?, int_anz_streich=? WHERE "
            "int_wettkaempfeid=?");
        query.bindValue(22, editid);
    }
    query.bindValue(0, this->event->getMainEventId());
    query.bindValue(1, ui->cmb_bereich->itemData(ui->cmb_bereich->currentIndex()));
    query.bindValue(2, ui->cmb_typ->currentIndex());
    query.bindValue(3, ui->txt_wknr->text());
    query.bindValue(4, ui->txt_wkbez->text());
    query.bindValue(5, ui->dae_wkj1->date().year());
    query.bindValue(6, j2);
    query.bindValue(7, ui->sbx_quali->value());
    query.bindValue(8, ui->sbx_wertungen->value());
    query.bindValue(9, ui->chk_streich->isChecked());
    query.bindValue(10, ui->chk_akjg->isChecked());
    query.bindValue(11, ui->chk_wahl->isChecked());
    query.bindValue(12, ui->sbx_durchgang->value());
    query.bindValue(13, ui->sbx_bahn->value());
    query.bindValue(14, ui->tie_start->time().toString("hh:mm:ss"));
    query.bindValue(15, ui->tie_warmup->time().toString("hh:mm:ss"));
    query.bindValue(16, ui->chk_showinfo->isChecked());
    query.bindValue(17, ui->chk_kuer->isChecked());
    query.bindValue(18, ui->chk_sortasc->isChecked());
    query.bindValue(19, ui->gbx_sort->isChecked());
    query.bindValue(20, ui->chk_gpunkte->isChecked());
    query.bindValue(21, ui->sbx_anz->value());
    query.exec();
    if (editid == 0) {
        if (_global::getDBTyp() == 0) {
            QSqlQuery query7("SELECT last_value FROM tfx_wettkaempfe_int_wettkaempfeid_seq");
            query7.next();
            editid = query7.value(0).toInt();
        } else {
            editid = query.lastInsertId().toInt();
        }
    }
    for (int i = 0; i < model->rowCount(); i++) {
        QSqlQuery query7;
        query7.prepare(
            "SELECT int_wettkaempfe_x_disziplinenid, int_disziplinenid, int_wettkaempfeid FROM "
            "tfx_wettkaempfe_x_disziplinen WHERE int_wettkaempfeid=? AND int_disziplinenid=?");
        query7.bindValue(0, editid);
        query7.bindValue(1, model->item(i, 8)->text());
        query7.exec();
        if (model->item(i, 0)->checkState() == Qt::Checked) {
            QSqlQuery query2;
            if (_global::querySize(query7) == 0) {
                query2.prepare("INSERT INTO tfx_wettkaempfe_x_disziplinen "
                               "(int_wettkaempfeid,int_disziplinenid,var_ausschreibung,int_"
                               "sortierung,bol_kp,rel_max) VALUES (?,?,?,?,?,?)");
            } else {
                query7.next();
                query2.prepare("UPDATE tfx_wettkaempfe_x_disziplinen SET int_wettkaempfeid=?, "
                               "int_disziplinenid=?, var_ausschreibung=?, int_sortierung=?, "
                               "bol_kp=?, rel_max=? WHERE int_wettkaempfe_x_disziplinenid=?");
                query2.bindValue(6, query7.value(0).toString());
            }
            query2.bindValue(0, editid);
            query2.bindValue(1, model->item(i, 8)->text());
            query2.bindValue(2, model->item(i, 5)->text());
            query2.bindValue(3, i);
            if (model->item(i, 6)->checkState() == Qt::Checked) {
                query2.bindValue(4, true);
            } else {
                query2.bindValue(4, false);
            }
            query2.bindValue(5, model->item(i, 7)->text().replace(",", ".").toDouble());
            query2.exec();
        } else {
            if (_global::querySize(query7) > 0) {
                query7.next();
                QSqlQuery query9;
                query9.prepare("DELETE FROM tfx_wettkaempfe_x_disziplinen WHERE "
                               "int_wettkaempfe_x_disziplinenid=?");
                query9.bindValue(0, query7.value(0).toString());
                query9.exec();
                query9.prepare("DELETE FROM tfx_quali_leistungen WHERE int_disziplinenid=? AND "
                               "int_wertungenid IN (SELECT int_wertungenid FROM tfx_wertungen "
                               "WHERE int_wettkaempfeid=?)");
                query9.bindValue(0, query7.value(1).toInt());
                query9.bindValue(1, query7.value(2).toInt());
                query9.exec();
                query9.prepare("DELETE FROM tfx_wertungen_details WHERE int_disziplinenid=? AND "
                               "int_wertungenid IN (SELECT int_wertungenid FROM tfx_wertungen "
                               "WHERE int_wettkaempfeid=?)");
                query9.bindValue(0, query7.value(1).toInt());
                query9.bindValue(1, query7.value(2).toInt());
                query9.exec();
            }
        }
    }
    if (ui->gbx_sort->isChecked()) {
        QSqlQuery query8;
        query8.prepare("SELECT int_wettkaempfe_x_disziplinenid, int_disziplinenid, "
                       "tfx_wettkaempfe_x_disziplinen.bol_kp, tfx_wettkaempfe.bol_kp FROM "
                       "tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_wettkaempfe USING "
                       "(int_wettkaempfeid) WHERE int_wettkaempfeid=?");
        query8.bindValue(0, editid);
        query8.exec();
        while (query8.next()) {
            int kpmax = 0;
            if (query8.value(2).toBool() || query8.value(3).toBool())
                kpmax = 1;
            for (int kp = 0; kp <= kpmax; kp++) {
                QSqlQuery query11;
                query11.prepare("SELECT int_wettkaempfe_disposid FROM tfx_wettkaempfe_dispos WHERE "
                                "int_wettkaempfe_x_disziplinenid=? AND int_kp=?");
                query11.bindValue(0, query8.value(0).toInt());
                query11.bindValue(1, kp);
                query11.exec();
                bool found = false;
                for (int i = 0; i < model2->rowCount(); i++) {
                    if (found)
                        break;
                    for (int j = 0; j < model2->columnCount(); j++) {
                        if (model2->item(i, j) == nullptr)
                            continue;
                        if (model2->item(i, j)->data().toInt() == query8.value(1).toInt()) {
                            if (kp == 0 || model2->item(i, j)->text().right(3) == "(K)") {
                                QSqlQuery query12;
                                if (_global::querySize(query11) == 0) {
                                    query12.prepare("INSERT INTO tfx_wettkaempfe_dispos "
                                                    "(int_wettkaempfe_x_disziplinenid,int_sortx,"
                                                    "int_sorty,int_kp) VALUES (?,?,?,?)");
                                } else {
                                    query11.next();
                                    query12.prepare(
                                        "UPDATE tfx_wettkaempfe_dispos SET "
                                        "int_wettkaempfe_x_disziplinenid=?, int_sortx=?, "
                                        "int_sorty=?, int_kp=? WHERE int_wettkaempfe_disposid=?");
                                    query12.bindValue(4, query11.value(0).toInt());
                                }
                                query12.bindValue(0, query8.value(0).toInt());
                                if (ui->cmb_typ->currentIndex() == 1) {
                                    query12.bindValue(1, model2->columnCount() - 1 - j);
                                } else {
                                    query12.bindValue(1, j);
                                }
                                query12.bindValue(2, i);
                                query12.bindValue(3, kp);
                                query12.exec();
                                found = true;
                                break;
                            }
                        }
                    }
                }
                if (!found) {
                    QSqlQuery query13;
                    query13.prepare("DELETE FROM tfx_wettkaempfe_dispos WHERE "
                                    "int_wettkaempfe_x_disziplinenid=?)");
                    query13.bindValue(0, query8.value(0).toInt());
                    query13.exec();
                }
            }
        }
    } else {
        QSqlQuery query10;
        query10.prepare("DELETE FROM tfx_wettkaempfe_dispos WHERE int_wettkaempfe_x_disziplinenid "
                        "IN (SELECT int_wettkaempfe_x_disziplinenid FROM "
                        "tfx_wettkaempfe_x_disziplinen WHERE int_wettkaempfeid=?)");
        query10.bindValue(0, editid);
        query10.exec();
    }
    done(1);
}

void CompetitionDialog::fillTable()
{
    model->removeRows(0, model->rowCount());
    QSqlQuery bereich;
    bereich.prepare(
        "SELECT bol_maennlich, bol_weiblich FROM tfx_bereiche WHERE int_bereicheid=? LIMIT 1");
    bereich.bindValue(0, ui->cmb_bereich->itemData(ui->cmb_bereich->currentIndex()));
    bereich.exec();
    bereich.next();
    QSqlQuery query2;
    query2.prepare(
        "SELECT var_icon, tfx_disziplinen.int_disziplinenid, tfx_sport.var_name, "
        "tfx_disziplinen.var_name, CASE WHEN bol_m = 'true' AND bol_w = 'true' THEN 'm/w' ELSE "
        "CASE WHEN bol_m = 'true' THEN 'm' ELSE 'w' END END, var_kuerzel, CASE WHEN "
        "int_wettkaempfeid IS NULL THEN 'false' ELSE 'true' END, var_ausschreibung, bol_kp, "
        "rel_max FROM tfx_disziplinen INNER JOIN tfx_sport USING (int_sportid) LEFT JOIN "
        "tfx_wettkaempfe_x_disziplinen ON tfx_wettkaempfe_x_disziplinen.int_disziplinenid = "
        "tfx_disziplinen.int_disziplinenid AND int_wettkaempfeid=? WHERE (bol_m=? OR bol_w=?) "
        "ORDER BY CASE WHEN int_sortierung IS NULL THEN 99999 ELSE int_sortierung END, "
        "tfx_sport.var_name,tfx_disziplinen.var_name");
    query2.bindValue(0, editid);
    query2.bindValue(1, bereich.value(0).toBool());
    query2.bindValue(2, bereich.value(1).toBool());
    query2.exec();
    while (query2.next()) {
        auto *check = new QStandardItem();
        check->setCheckable(true);
        check->setEditable(false);
        if (query2.value(6).toBool())
            check->setCheckState(Qt::Checked);
        auto *icon = new QStandardItem();
        icon->setIcon(QIcon(query2.value(0).toString()));
        icon->setEditable(false);
        model->setItem(query2.at(), 0, check);
        model->setItem(query2.at(), 1, icon);
        auto *sport = new QStandardItem(query2.value(2).toString());
        sport->setEditable(false);
        model->setItem(query2.at(), 2, sport);
        auto *name = new QStandardItem(query2.value(3).toString());
        name->setEditable(false);
        model->setItem(query2.at(), 3, name);
        auto *sex = new QStandardItem(query2.value(4).toString());
        sex->setEditable(false);
        sex->setTextAlignment(Qt::AlignCenter);
        model->setItem(query2.at(), 4, sex);
        model->setItem(query2.at(), 5, new QStandardItem(query2.value(7).toString()));
        auto *kp = new QStandardItem();
        kp->setCheckable(true);
        kp->setEditable(false);
        if (query2.value(8).toBool())
            kp->setCheckState(Qt::Checked);
        model->setItem(query2.at(), 6, kp);
        auto *points = new QStandardItem("");
        if (query2.value(9).toDouble() > 0)
            points->setText(query2.value(9).toString());
        model->setItem(query2.at(), 7, points);
        model->setItem(query2.at(), 8, new QStandardItem(query2.value(1).toString()));
    }
    fillTable2();
}

void CompetitionDialog::fillTable2()
{
    model2->clear();
    int count = 0;
    for (int i = 0; i < model->rowCount(); i++) {
        if (model->item(i, 0)->checkState() == Qt::Checked) {
            count++;
        }
    }
    int col = 0;
    QList<QStandardItem *> puffer;
    for (int i = 0; i < model->rowCount(); i++) {
        if (model->item(i, 0)->checkState() == Qt::Checked) {
            QStandardItem *itm = new QStandardItem(model->item(i, 1)->icon(),
                                                   model->item(i, 3)->text());
            itm->setData(model->item(i, 8)->text());
            QStandardItem *itm2;
            if (model->item(i, 6)->checkState() == Qt::Checked || ui->chk_kuer->isChecked()) {
                itm->setText(model->item(i, 3)->text() + " (P)");
                itm2 = new QStandardItem(model->item(i, 1)->icon(),
                                         model->item(i, 3)->text() + " (K)");
                itm2->setData(model->item(i, 8)->text());
            }
            if (ui->gbx_sort->isChecked() && editid > 0) {
                QSqlQuery query;
                query.prepare(
                    "SELECT int_sortx, int_sorty FROM tfx_wettkaempfe_dispos INNER JOIN "
                    "tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfe_x_disziplinenid) WHERE "
                    "int_wettkaempfeid=? AND int_disziplinenid=? AND int_kp=0");
                query.bindValue(0, editid);
                query.bindValue(1, itm->data());
                query.exec();
                if (_global::querySize(query) == 0) {
                    puffer.append(itm);
                } else {
                    query.next();
                    int x;
                    if (ui->cmb_typ->currentIndex() == 1) {
                        x = count - 1 - query.value(0).toInt();
                    } else {
                        x = query.value(0).toInt();
                    }
                    model2->setItem(query.value(1).toInt(), x, itm);
                }
            } else {
                model2->setItem(0, col, itm);
                col++;
            }
            if (model->item(i, 6)->checkState() == Qt::Checked || ui->chk_kuer->isChecked()) {
                if (ui->gbx_sort->isChecked() && editid > 0) {
                    QSqlQuery query;
                    query.prepare(
                        "SELECT int_sortx, int_sorty FROM tfx_wettkaempfe_dispos INNER JOIN "
                        "tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfe_x_disziplinenid) "
                        "WHERE int_wettkaempfeid=? AND int_disziplinenid=? AND int_kp=1");
                    query.bindValue(0, editid);
                    query.bindValue(1, itm2->data());
                    query.exec();
                    if (_global::querySize(query) == 0) {
                        puffer.append(itm2);
                    } else {
                        query.next();
                        int x;
                        if (ui->cmb_typ->currentIndex() == 1) {
                            x = count - 1 - query.value(0).toInt();
                        } else {
                            x = query.value(0).toInt();
                        }
                        model2->setItem(query.value(1).toInt(), x, itm2);
                    }
                } else {
                    model2->setItem(0, col, itm2);
                    col++;
                }
            }
        }
    }
    col = model2->columnCount();
    for (auto i : puffer) {
        model2->setItem(0, col, i);
        col++;
    }
}

void CompetitionDialog::markGroup()
{
    QSqlQuery getRows;
    getRows.prepare("SELECT int_disziplinenid, int_pos FROM tfx_disgrp_x_disziplinen WHERE "
                    "int_disziplinen_gruppenid=? ORDER BY int_pos");
    getRows.bindValue(0, ui->cmb_groups->itemData(ui->cmb_groups->currentIndex()));
    getRows.exec();
    while (getRows.next()) {
        if (model->findItems(getRows.value(0).toString(), Qt::MatchExactly, 8).isEmpty())
            continue;
        int row = model->findItems(getRows.value(0).toString(), Qt::MatchExactly, 8).at(0)->row();
        model->insertRow(getRows.value(1).toInt(), model->takeRow(row));
        model->item(getRows.value(1).toInt(), 0)->setCheckState(Qt::Checked);
    }
}

void CompetitionDialog::orderMoveUp()
{
    if (ui->tbl_order->selectionModel()->selectedIndexes().isEmpty())
        return;
    QModelIndex selIdx = ui->tbl_order->selectionModel()->selectedIndexes().at(0);
    if (selIdx.row() == 0)
        return;
    QStandardItem *itm = model2->takeItem(selIdx.row(), selIdx.column());
    QStandardItem *itm2 = model2->takeItem(selIdx.row() - 1, selIdx.column());
    if (itm2 == nullptr)
        itm2 = new QStandardItem("");
    QModelIndex newIdx = model2->index(selIdx.row() - 1, selIdx.column());
    model2->setItem(selIdx.row(), selIdx.column(), itm2);
    model2->setItem(selIdx.row() - 1, selIdx.column(), itm);
    ui->tbl_order->selectionModel()->setCurrentIndex(newIdx, QItemSelectionModel::Select);
    bool check = true;
    for (int i = 0; i < model2->columnCount(); i++) {
        if (model2->item(selIdx.row(), i) == nullptr)
            continue;
        if (model2->item(selIdx.row(), i)->text() != "") {
            check = false;
            break;
        }
    }
    if (check)
        model2->removeRow(selIdx.row());
}

void CompetitionDialog::orderMoveDown()
{
    if (ui->tbl_order->selectionModel()->selectedIndexes().isEmpty())
        return;
    QModelIndex selIdx = ui->tbl_order->selectionModel()->selectedIndexes().at(0);
    if (selIdx.row() + 1 == model2->rowCount())
        model2->insertRow(model2->rowCount());
    QStandardItem *itm = model2->takeItem(selIdx.row(), selIdx.column());
    QStandardItem *itm2 = model2->takeItem(selIdx.row() + 1, selIdx.column());
    if (itm2 == nullptr)
        itm2 = new QStandardItem("");
    QModelIndex newIdx = model2->index(selIdx.row() + 1, selIdx.column());
    model2->setItem(selIdx.row(), selIdx.column(), itm2);
    model2->setItem(selIdx.row() + 1, selIdx.column(), itm);
    ui->tbl_order->selectionModel()->setCurrentIndex(newIdx, QItemSelectionModel::Select);
}

void CompetitionDialog::orderMoveLeft()
{
    if (ui->tbl_order->selectionModel()->selectedIndexes().isEmpty())
        return;
    QModelIndex selIdx = ui->tbl_order->selectionModel()->selectedIndexes().at(0);
    if (selIdx.column() == 0)
        return;
    QStandardItem *itm = model2->takeItem(selIdx.row(), selIdx.column());
    QStandardItem *itm2 = model2->takeItem(selIdx.row(), selIdx.column() - 1);
    if (itm2 == nullptr)
        itm2 = new QStandardItem("");
    QModelIndex newIdx = model2->index(selIdx.row(), selIdx.column() - 1);
    model2->setItem(selIdx.row(), selIdx.column(), itm2);
    model2->setItem(selIdx.row(), selIdx.column() - 1, itm);
    ui->tbl_order->selectionModel()->setCurrentIndex(newIdx, QItemSelectionModel::Select);
    bool check = true;
    for (int i = 0; i < model2->rowCount(); i++) {
        if (model2->item(i, selIdx.column()) == nullptr)
            continue;
        if (model2->item(i, selIdx.column())->text() != "") {
            check = false;
            break;
        }
    }
    if (check)
        model2->removeColumn(selIdx.column());
}

void CompetitionDialog::orderMoveRight()
{
    if (ui->tbl_order->selectionModel()->selectedIndexes().isEmpty())
        return;
    QModelIndex selIdx = ui->tbl_order->selectionModel()->selectedIndexes().at(0);
    if (selIdx.column() + 1 == model2->columnCount())
        model2->insertColumn(model2->columnCount());
    QStandardItem *itm = model2->takeItem(selIdx.row(), selIdx.column());
    QStandardItem *itm2 = model2->takeItem(selIdx.row(), selIdx.column() + 1);
    if (itm2 == nullptr)
        itm2 = new QStandardItem("");
    QModelIndex newIdx = model2->index(selIdx.row(), selIdx.column() + 1);
    model2->setItem(selIdx.row(), selIdx.column(), itm2);
    model2->setItem(selIdx.row(), selIdx.column() + 1, itm);
    ui->tbl_order->selectionModel()->setCurrentIndex(newIdx, QItemSelectionModel::Select);
}
