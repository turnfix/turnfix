#include "participantswidget.h"
#include "groupdialog.h"
#include "individualdialog.h"
#include "model/entity/event.h"
#include "model/settings/session.h"
#include "multiparticipantsdialog.h"
#include "participantstablemodel.h"
#include "qualificationstandardsdialog.h"
#include "src/global/header/_global.h"
#include "teamdialog.h"
#include "ui_participantswidget.h"
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSqlQuery>

ParticipantsWidget::ParticipantsWidget(QWidget* parent) : QWidget(parent), ui(new Ui::ParticipantsWidget) {
    ui->setupUi(this);

    this->event = Session::getInstance()->getEvent();

    this->participantsModel = new ParticipantsTableModel(this->event);
    this->sortModel = new QSortFilterProxyModel();
    this->sortModel->setSourceModel(this->participantsModel);

    ui->participantsTable->setModel(this->sortModel);

    this->viewChanged(ui->cmb_typ->currentIndex());

    connect(ui->but_addTN, &QPushButton::clicked, this, &ParticipantsWidget::addTN);
    connect(ui->but_addCL, &QPushButton::clicked, this, &ParticipantsWidget::addCL);
    connect(ui->but_editTN, &QPushButton::clicked, this, &ParticipantsWidget::editTN);
    connect(ui->but_delTN, &QPushButton::clicked, this, &ParticipantsWidget::delTN);
    connect(ui->but_timeTN, &QPushButton::clicked, this, &ParticipantsWidget::meldeTN);
    connect(ui->but_copyTN, &QPushButton::clicked, this, &ParticipantsWidget::syncTN);
    connect(ui->cmb_filterTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ParticipantsWidget::updateTNFilterColumn);
    connect(ui->txt_filterTN, &QLineEdit::textChanged, this, &ParticipantsWidget::updateTNFilterText);
    connect(ui->cmb_typ, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ParticipantsWidget::viewChanged);
    connect(ui->participantsTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ParticipantsWidget::updateMelde);
    connect(ui->participantsTable, &QTableView::doubleClicked, this, &ParticipantsWidget::editTN);
}

ParticipantsWidget::~ParticipantsWidget()
{
    delete ui;
}

void ParticipantsWidget::viewChanged(int index)
{
    ParticipantsTableModel::Type type = static_cast<ParticipantsTableModel::Type>(index);
    bool isIndiviual = type == ParticipantsTableModel::Type::Individual;
    bool isTeam = type == ParticipantsTableModel::Type::Team;

    this->participantsModel->updateType(type);

    ui->but_timeTN->setEnabled(isIndiviual);
    ui->but_addCL->setEnabled(isIndiviual);
    ui->but_copyTN->setVisible(isTeam && this->event->multiRoundEvent() && this->event->round() > 1);
    ui->cmb_filterTN->clear();

    QHeaderView::ResizeMode resizeModeIndividual[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resizeIndividual[] = {40, 200, 40, 40, 200, 45, 45};

    QHeaderView::ResizeMode resizeModeTeam[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resizeTeam[] = {40, 200, 200, 45, 45};

    QHeaderView::ResizeMode resizeModeGroup[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resizeGroup[] = {40, 200, 200, 45, 45};

    int length;
    switch (type) {
        case ParticipantsTableModel::Type::Individual: length = 7; break;
        default: length = 5; break;
    }

    for (int i=0;i<length;i++) {
        ui->cmb_filterTN->addItem(this->participantsModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).value<QString>());
        int resize;
        QHeaderView::ResizeMode mode;
        switch (ui->cmb_typ->currentIndex()) {
        case 0:
            resize = resizeIndividual[i];
            mode = resizeModeIndividual[i];
            break;
        case 1:
            resize = resizeTeam[i];
            mode = resizeModeTeam[i];
            break;
        case 2:
            resize = resizeGroup[i];
            mode = resizeModeGroup[i];
            break;
        }
        ui->participantsTable->horizontalHeader()->setSectionResizeMode(i, mode);
        ui->participantsTable->horizontalHeader()->resizeSection(i, resize);
    }
}

void ParticipantsWidget::refresh()
{
    this->participantsModel->loadData();
}

void ParticipantsWidget::loadBestView() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(CASE WHEN int_typ=0 THEN 1 END), COUNT(CASE WHEN int_typ=1 THEN 1 END), COUNT(CASE WHEN int_typ=2 THEN 1 END) FROM tfx_wettkaempfe WHERE int_veranstaltungenid=?");
    query.bindValue(0, this->event->mainEventId());
    query.exec();
    query.next();
    if (query.value(1).toInt()>query.value(0).toInt()) {
        if (query.value(2).toInt()>query.value(0).toInt()) {
            ui->cmb_typ->setCurrentIndex(2);
        } else {
            ui->cmb_typ->setCurrentIndex(1);
        }
    } else if (query.value(2).toInt()>query.value(0).toInt()) {
        ui->cmb_typ->setCurrentIndex(2);
    } else {
        ui->cmb_typ->setCurrentIndex(0);
    }
}

void ParticipantsWidget::addTN() {
    QDialog *dialog;
    switch(ui->cmb_typ->currentIndex()) {
    case 0:
        dialog = new IndividualDialog(this->event, 0,this);
        break;
    case 1:
        dialog = new TeamDialog(this->event, 0,this);
        break;
    case 2:
        dialog = new GroupDialog(this->event, 0,this);
        break;
    }
    if(dialog->exec() == 1) {
        this->participantsModel->loadData();
    }

    _global::updateRgDis(this->event);
    ui->participantsTable->setFocus();
}

void ParticipantsWidget::addCL() {
    MultiParticipantsDialog *cl = new MultiParticipantsDialog(this->event, this);
    if(cl->exec() == 1) {
        this->participantsModel->loadData();
    }
}

void ParticipantsWidget::editTN() {
    QModelIndex idx = ui->participantsTable->currentIndex();
    QDialog *dialog;

    if (ui->cmb_typ->currentIndex() == 0) {
        if (ui->participantsTable->currentIndex().isValid()) {
            dialog = new IndividualDialog(this->event, QVariant(sortModel->data(sortModel->index(ui->participantsTable->currentIndex().row(),7))).toInt(),this);
        }
    } else if (ui->cmb_typ->currentIndex() == 1) {
        if (ui->participantsTable->currentIndex().isValid()) {
            dialog = new TeamDialog(this->event, QVariant(sortModel->data(sortModel->index(ui->participantsTable->currentIndex().row(),5))).toInt(),this);
        }
    } else if (ui->cmb_typ->currentIndex() == 2) {
        if (ui->participantsTable->currentIndex().isValid()) {
            dialog = new GroupDialog(this->event, QVariant(sortModel->data(sortModel->index(ui->participantsTable->currentIndex().row(),5))).toInt(),this);
        }
    }

    if(dialog->exec() == 1) {
        this->participantsModel->loadData();
    }

    ui->participantsTable->setCurrentIndex(idx);
    ui->participantsTable->setFocus();

    _global::updateRgDis(this->event);
}

void ParticipantsWidget::meldeTN() {
    if (ui->participantsTable->currentIndex().isValid()) {
        QualificationStandardsDialog *ml = new QualificationStandardsDialog(this->event, QVariant(sortModel->data(sortModel->index(ui->participantsTable->currentIndex().row(),7))).toInt(),this);
        if(ml->exec() == 1) {}
        ui->participantsTable->setFocus();
    } else {
        QMessageBox::information(this, "Ungültiger Eintrag", "Bitte selektiere eine Zeile in der Liste");
    }
}

void ParticipantsWidget::delTN() {
    if (ui->cmb_typ->currentIndex() == 0) {
        if (ui->participantsTable->currentIndex().isValid()) {
            QMessageBox msg(QMessageBox::Question, "Teilnehmer löschen", "Wollen sie diesen Teilnehmer wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_wertungen WHERE int_wertungenid=?");
                query.bindValue( 0, QVariant(sortModel->data(sortModel->index(ui->participantsTable->currentIndex().row(),7))).toInt() );
                query.exec();
                this->participantsModel->loadData();
            }
            ui->participantsTable->setFocus();
        }
    } else if (ui->cmb_typ->currentIndex() == 1) {
        if (ui->participantsTable->currentIndex().isValid()) {
            QMessageBox msg(QMessageBox::Question, "Mannschaft löschen", "Wollen sie diese Mannschaft wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_mannschaften WHERE int_mannschaftenid=?");
                query.bindValue( 0, QVariant(sortModel->data(sortModel->index(ui->participantsTable->currentIndex().row(),5))).toInt());
                query.exec();
                this->participantsModel->loadData();
            }
            ui->participantsTable->setFocus();
        }
    } else if (ui->cmb_typ->currentIndex() == 2) {
        if (ui->participantsTable->currentIndex().isValid()) {
            QMessageBox msg(QMessageBox::Question, "Gruppe löschen", "Wollen sie diese Gruppe wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_gruppen WHERE int_gruppenid=?");
                query.bindValue( 0, QVariant(sortModel->data(sortModel->index(ui->participantsTable->currentIndex().row(),5))).toInt());
                query.exec();
                this->participantsModel->loadData();
            }
            ui->participantsTable->setFocus();
        }
    }
    _global::updateRgDis(this->event);
}

void ParticipantsWidget::updateMelde() {
    if (ui->participantsTable->currentIndex().isValid()) {
        QSqlQuery query;
        query.prepare("SELECT tfx_disziplinen.var_name, int_disziplinenid, int_wertungenid, var_maske FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_veranstaltungenid=? AND int_wertungenid=? AND bol_bahnen AND (int_disziplinenid IN (SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_disziplinenid=tfx_disziplinen.int_disziplinenid AND int_wertungenid=tfx_wertungen.int_wertungenid) OR (SELECT COUNT(*) FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=tfx_wertungen.int_wertungenid)=0)");
        query.bindValue(0,this->event->id());
        query.bindValue(1,QVariant(sortModel->data(sortModel->index(ui->participantsTable->currentIndex().row(),7))).toInt());
        query.exec();
        if (_global::querySize(query) <= 0) {
            ui->but_timeTN->setEnabled(false);
        } else {
            ui->but_timeTN->setEnabled(true);
        }
    } else {
        ui->but_timeTN->setEnabled(false);
    }
}

void ParticipantsWidget::syncTN() {
    QMessageBox msg(QMessageBox::Question, "Mannschaften synchronisieren", "Wollen sie diese Mannschaft wirklich synchronisieren? Dabei werden alle vorhandenen Wertungen dieser Runde gelöscht.",QMessageBox::Ok | QMessageBox::Cancel);
    if(msg.exec() == QMessageBox::Ok) {
        QSqlQuery query;
        query.prepare("DELETE FROM tfx_wertungen WHERE int_wertungenid IN (SELECT int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=?) AND int_mannschaftenid IS NOT NULL");
        query.bindValue( 0, this->event->mainEventId() );
        query.bindValue( 1, this->event->round());
        query.exec();
        query.prepare("DELETE FROM tfx_man_x_teilnehmer WHERE int_mannschaftenid IN (SELECT int_mannschaftenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=?) AND int_runde=?");
        query.bindValue( 0, this->event->mainEventId() );
        query.bindValue( 1, 1);
        query.bindValue( 2, this->event->round());
        query.exec();
        QSqlQuery query2;
        query2.prepare("SELECT * FROM tfx_wertungen WHERE int_wertungenid IN (SELECT int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=?) AND int_mannschaftenid IS NOT NULL");
        query2.bindValue( 0, this->event->mainEventId() );
        query2.bindValue( 1, 1);
        query2.exec();
        while (query2.next()) {
            if (query2.value(4).toInt()>0) {
                QSqlQuery query6;
                query6.prepare("INSERT INTO tfx_man_x_teilnehmer (int_mannschaftenid,int_teilnehmerid, int_runde) VALUES (?,?,?)");
                query6.bindValue(0,query2.value(4).toString());
                query6.bindValue(1,query2.value(2).toString());
                query6.bindValue(2,this->event->round());
                query6.exec();
            }
            QSqlQuery query7;
            query7.prepare("INSERT INTO tfx_wertungen (int_teilnehmerid,int_wettkaempfeid,var_riege,bol_ak,int_mannschaftenid,int_runde,int_startnummer,int_statusid) VALUES(?,?,?,?,?,?,?,?)");
            query7.bindValue( 0, query2.value(2).toString() );
            query7.bindValue( 1, query2.value(1).toString() );
            query7.bindValue( 2, query2.value(10).toString() );
            query7.bindValue( 3, query2.value(8).toBool() );
            query7.bindValue( 4, query2.value(4).toString() );
            query7.bindValue( 5, this->event->round() );
            query7.bindValue( 6, query2.value(7).toString() );
            query7.bindValue( 7, 1);
            query7.exec();
        }
        _global::updateRgDis(this->event);
        this->participantsModel->loadData();
    }
}

void ParticipantsWidget::updateTNFilterColumn(int index) {
    sortModel->setFilterKeyColumn(index);
}

void ParticipantsWidget::updateTNFilterText(QString text) {
    QRegExp expr(text);
    expr.setCaseSensitivity(Qt::CaseInsensitive);
    sortModel->setFilterRegExp(expr);
}
