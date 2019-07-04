#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include "model/settings/session.h"
#include "model/objects/event.h"
#include "participantswidget.h"
#include "participantstablemodel.h"
#include "individualdialog.h"
#include "teamdialog.h"
#include "groupdialog.h"
#include "clubdialog.h"
#include "qualificationstandardsdialog.h"
#include "src/global/header/_global.h"

#include <QDebug>

ParticipantsWidget::ParticipantsWidget(QWidget* parent) : QWidget(parent) {
    setupUi(this);

    this->event = Session::getInstance()->getEvent();

    this->participantsModel = new ParticipantsTableModel(this->event);
    this->sortModel = new QSortFilterProxyModel();
    this->sortModel->setSourceModel(this->participantsModel);
    this->participantsTable->setModel(this->sortModel);

    connect(but_addTN, SIGNAL(clicked()), this, SLOT(addTN()));
    connect(but_addCL, SIGNAL(clicked()), this, SLOT(addCL()));
    connect(but_editTN, SIGNAL(clicked()), this, SLOT(editTN()));
    connect(but_delTN, SIGNAL(clicked()), this, SLOT(delTN()));
    connect(but_timeTN, SIGNAL(clicked()), this, SLOT(meldeTN()));
    connect(but_copyTN, SIGNAL(clicked()), this, SLOT(syncTN()));
    connect(cmb_filterTN, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTNFilterColumn(int)));
    connect(txt_filterTN, SIGNAL(textChanged(QString)), this, SLOT(updateTNFilterText(QString)));
    connect(cmb_typ, SIGNAL(currentIndexChanged(int)), this, SLOT(refresh()));
    connect(participantsTable->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateMelde()));
    connect(participantsTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editTN()));
}

void ParticipantsWidget::loadBestView() {
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

void ParticipantsWidget::refresh() {
    switch(cmb_typ->currentIndex()) {
    case 0:
        fillTable();
        if (participantsTable->currentIndex().isValid()) {
            but_timeTN->setEnabled(true);
        } else {
            but_timeTN->setEnabled(false);
        }
        but_addCL->setEnabled(true);
        but_copyTN->setVisible(false);
        break;
    case 1:
        fillTable();
        but_timeTN->setEnabled(false);
        but_addCL->setEnabled(false);
        if (!this->event->isMultiRoundEvent()) {
            but_copyTN->setVisible(false);
        } else if (this->event->getRound() > 1) {
            but_copyTN->setVisible(true);
        }
        break;
    case 2:
        fillTable();
        but_timeTN->setEnabled(false);
        but_addCL->setEnabled(false);
        but_copyTN->setVisible(false);
        break;
    }

    cmb_filterTN->setCurrentIndex(0);
}

void ParticipantsWidget::fillTable() {
    ParticipantsTableModel::Type type;
    switch (cmb_typ->currentIndex()) {
        case 0: type = ParticipantsTableModel::Type::Individual; break;
        case 1: type = ParticipantsTableModel::Type::Team; break;
        case 2: type = ParticipantsTableModel::Type::Group; break;
    }

    participantsModel->updateType(type);

    int idx = cmb_filterTN->currentIndex();
    cmb_filterTN->clear();

    QHeaderView::ResizeMode resizeModeIndividual[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resizeIndividual[] = {40, 200, 40, 40, 200, 45, 45};

    QHeaderView::ResizeMode resizeModeTeam[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resizeTeam[] = {40, 200, 200, 45, 45};

    QHeaderView::ResizeMode resizeModeGroup[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resizeGroup[] = {40, 200, 200, 45, 45};

    int length;
    switch (cmb_typ->currentIndex()) {
        case 0: length = 7; break;
        default: length = 5; break;
    }

    for (int i=0;i<length;i++) {
        // cmb_filterTN->addItem(headersTN[i]);
        int resize;
        QHeaderView::ResizeMode mode;
        switch (cmb_typ->currentIndex()) {
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
        participantsTable->horizontalHeader()->setSectionResizeMode(i, mode);
        participantsTable->horizontalHeader()->resizeSection(i, resize);
    }
    cmb_filterTN->setCurrentIndex(idx);
}

void ParticipantsWidget::addTN() {
    QDialog *dialog;
    switch(cmb_typ->currentIndex()) {
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
        fillTable();
    }

    _global::updateRgDis(this->event);
    participantsTable->setFocus();
}

void ParticipantsWidget::addCL() {
    ClubDialog *cl = new ClubDialog(this->event, this);
    if(cl->exec() == 1) {
        fillTable();
    }
}

void ParticipantsWidget::editTN() {
    QModelIndex idx = participantsTable->currentIndex();
    QDialog *dialog;

    if (cmb_typ->currentIndex() == 0) {
        if (participantsTable->currentIndex().isValid()) {
            dialog = new IndividualDialog(this->event, QVariant(sortModel->data(sortModel->index(participantsTable->currentIndex().row(),7))).toInt(),this);
        }
    } else if (cmb_typ->currentIndex() == 1) {
        if (participantsTable->currentIndex().isValid()) {
            dialog = new TeamDialog(this->event, QVariant(sortModel->data(sortModel->index(participantsTable->currentIndex().row(),5))).toInt(),this);
        }
    } else if (cmb_typ->currentIndex() == 2) {
        if (participantsTable->currentIndex().isValid()) {
            dialog = new GroupDialog(this->event, QVariant(sortModel->data(sortModel->index(participantsTable->currentIndex().row(),5))).toInt(),this);
        }
    }

    if(dialog->exec() == 1) {
        fillTable();
    }

    participantsTable->setCurrentIndex(idx);
    participantsTable->setFocus();

    _global::updateRgDis(this->event);
}

void ParticipantsWidget::meldeTN() {
    QualificationStandardsDialog *ml = new QualificationStandardsDialog(this->event, QVariant(sortModel->data(sortModel->index(participantsTable->currentIndex().row(),7))).toInt(),this);
    if(ml->exec() == 1) {}
    participantsTable->setFocus();
}

void ParticipantsWidget::delTN() {
    if (cmb_typ->currentIndex() == 0) {
        if (participantsTable->currentIndex().isValid()) {
            QMessageBox msg(QMessageBox::Question, "Teilnehmer löschen", "Wollen sie diesen Teilnehmer wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_wertungen WHERE int_wertungenid=?");
                query.bindValue( 0, QVariant(sortModel->data(sortModel->index(participantsTable->currentIndex().row(),7))).toInt() );
                query.exec();
                fillTable();
            }
            participantsTable->setFocus();
        }
    } else if (cmb_typ->currentIndex() == 1) {
        if (participantsTable->currentIndex().isValid()) {
            QMessageBox msg(QMessageBox::Question, "Mannschaft löschen", "Wollen sie diese Mannschaft wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_mannschaften WHERE int_mannschaftenid=?");
                query.bindValue( 0, QVariant(sortModel->data(sortModel->index(participantsTable->currentIndex().row(),5))).toInt());
                query.exec();
                fillTable();
            }
            participantsTable->setFocus();
        }
    } else if (cmb_typ->currentIndex() == 2) {
        if (participantsTable->currentIndex().isValid()) {
            QMessageBox msg(QMessageBox::Question, "Gruppe löschen", "Wollen sie diese Gruppe wirklich löschen?",QMessageBox::Ok | QMessageBox::Cancel);
            if(msg.exec() == QMessageBox::Ok) {
                QSqlQuery query;
                query.prepare("DELETE FROM tfx_gruppen WHERE int_gruppenid=?");
                query.bindValue( 0, QVariant(sortModel->data(sortModel->index(participantsTable->currentIndex().row(),5))).toInt());
                query.exec();
                fillTable();
            }
            participantsTable->setFocus();
        }
    }
    _global::updateRgDis(this->event);
}

void ParticipantsWidget::updateMelde() {
    if (participantsTable->currentIndex().isValid()) {
        QSqlQuery query;
        query.prepare("SELECT tfx_disziplinen.var_name, int_disziplinenid, int_wertungenid, var_maske FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_veranstaltungenid=? AND int_wertungenid=? AND bol_bahnen AND (int_disziplinenid IN (SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_disziplinenid=tfx_disziplinen.int_disziplinenid AND int_wertungenid=tfx_wertungen.int_wertungenid) OR (SELECT COUNT(*) FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=tfx_wertungen.int_wertungenid)=0)");
        query.bindValue(0,this->event->getId());
        query.bindValue(1,QVariant(sortModel->data(sortModel->index(participantsTable->currentIndex().row(),7))).toInt());
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

void ParticipantsWidget::syncTN() {
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
        fillTable();
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
