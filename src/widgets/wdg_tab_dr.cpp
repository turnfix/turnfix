#include <QFileDialog>
#include <QSqlQuery>
#include <QTextStream>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QMessageBox>
#include "export/results/individual.h"
#include "export/results/detail.h"
#include "export/results/round.h"
#include "export/results/table.h"
#include "export/results/results.h"
#include "export/participants/resultssheet.h"
#include "export/participants/squad.h"
#include "export/participants/registration.h"
#include "export/participants/judgessheet.h"
#include "export/participants/license.h"
#include "export/participants/itchecklist.h"
#include "export/misc/invitation.h"
#include "export/misc/summary.h"
#include "export/misc/itchecksquads.h"
#include "export/misc/card.h"
#include "export/misc/registrationmatrix.h"
#include "export/misc/certificate.h"
#include "export/misc/clublist.h"
#include "export/misc/timetable.h"
#include "export/misc/medalcount.h"
#include "model/settings/session.h"
#include "model/objects/competition.h"
#include "view/dialogs/selectclubdialog.h"
#include "view/dialogs/selectdisciplinedialog.h"

#include "header/wdg_tab_dr.h"
#include "../global/header/_global.h"
#include "../global/header/settings.h"
#include "../global/header/result_calc.h"
#include "../dialogs/select/header/dlg_select_wk.h"
#include "../dialogs/select/header/dlg_select_rg.h"
#include "../dialogs/select/header/dlg_select_tn.h"
#include "../dialogs/select/header/dlg_select_detail.h"


Tab_DR::Tab_DR(QWidget* parent) : QWidget(parent) {
    setupUi(this);

    this->event = Session::getInstance()->getEvent();

    connect(but_csvgesamt, SIGNAL(clicked()), this, SLOT(csvGesamt()));
    connect(but_csvrunde, SIGNAL(clicked()), this, SLOT(csvRunde()));
    connect(but_print, SIGNAL(clicked()), this, SLOT(startPrint()));
}

void Tab_DR::updateData() {
    QString urkunde = cmb_urkunde->currentText();
    QString headfoot = cmb_headfoot->currentText();
    QString cover = cmb_cover->currentText();
    cmb_urkunde->clear();
    cmb_headfoot->clear();
    cmb_cover->clear();
    cmb_headfoot->addItem("kein individualisiertes Layout",0);
    cmb_cover->addItem("kein Cover drucken",0);
    QSqlQuery getLayouts("SELECT int_layoutid, var_name FROM tfx_layouts ORDER BY var_name");
    while (getLayouts.next()) {
        cmb_urkunde->addItem(getLayouts.value(1).toString(),getLayouts.value(0).toInt());
        cmb_headfoot->addItem(getLayouts.value(1).toString(),getLayouts.value(0).toInt());
        cmb_cover->addItem(getLayouts.value(1).toString(),getLayouts.value(0).toInt());
    }
    if (cmb_urkunde->findText(urkunde)>=0) cmb_urkunde->setCurrentIndex(cmb_urkunde->findText(urkunde));
    if (cmb_headfoot->findText(headfoot)>=0) cmb_headfoot->setCurrentIndex(cmb_headfoot->findText(headfoot));
    if (cmb_cover->findText(cover)>=0) cmb_cover->setCurrentIndex(cmb_cover->findText(cover));
}

void Tab_DR::csvGesamt() {
    createCSV(0);
}

void Tab_DR::csvRunde() {
    createCSV(1);
}

void Tab_DR::startPrint() {
    Print::setCoverID(cmb_cover->itemData(cmb_cover->currentIndex()).toInt());
    Print::setDetailInfo(cmb_detail->currentIndex());
    Print::setHeadFootID(cmb_headfoot->itemData(cmb_headfoot->currentIndex()).toInt());
    Print::setPaperSize(QPrinter::A4);
    Print::setOrientation(QPrinter::Portrait);
    switch(cmb_printtype->currentIndex()) {
    case 0: { //Ergebnisse
            Results::setnewPageWK(chk_res_pg->isChecked());
            switch (cmb_res_type->currentIndex()) {
            case 0: { //Einzel
                    ausdruck = new Individual(this->event); }
                break;
            case 1: { //Detail
                    Detail::setPrintAW(chk_res_aw->isChecked());
                    ausdruck = new Detail(this->event); }
                break;
            case 2: { //Rundenergebnisse
                    Round::setUseExtraScore(chk_res_sc->isChecked());
                    ausdruck = new Round(this->event); }
                break;
            case 3: { //Tabelle
                    ausdruck = new Table(this->event);
                    ausdruck->setTypeString("Tabelle");}
                break;
            }
            ausdruck->setSelectWK(chk_res_wk->isChecked());
            ausdruck->setSelectDetail(chk_res_detail->isChecked());
            ausdruck->setSelectClub(chk_res_cl->isChecked());
            ausdruck->setTypeString("Ergebnisse");
        }
        break;
    case 1: { //Riegen
            Squad::setNewPageEach(chk_rg_page->isChecked());
            Squad::setOrder(cmb_rg_sort->currentIndex());
            ausdruck = new Squad(this->event);
            ausdruck->setSelectRiege(chk_rg_select->isChecked());
            ausdruck->setTypeString("Riegen");
        }
        break;
    case 2: { //Meldeliste
            ausdruck = new Registration(this->event);
            ausdruck->setSelectClub(chk_ve_select->isChecked());
            ausdruck->setTypeString("Meldeliste");
        }
        break;
    case 3: { //Wettkampfbogen
            JudgesSheet::setTeammode(chk_bo_split->isChecked());
            ausdruck = new JudgesSheet(this->event);
            ausdruck->setSelectRiege(true);
            ausdruck->setSelectDis(true);
        }
        break;
    case 4: { //Wettkampfkarte
            Card::setPaperSize(QPrinter::A5);
            ausdruck = new Card(this->event);
            ausdruck->setSelectTN(true);
        }
        break;
    case 5: { //Urkunde
            Certificate::setEineUrkunde(chk_ur_single->isChecked());
            Certificate::setRundenErgebnisse(chk_ur_runde->isChecked());
            Certificate::setPlatzWertung(chk_ur_platz->isChecked());
            Certificate::setUrkundenID(cmb_urkunde->itemData(cmb_urkunde->currentIndex()).toInt());
            Certificate::setEinzelErgebnis(chk_ur_einzelwertung->isChecked());
            ausdruck = new Certificate(this->event);
            ausdruck->setSelectTN(true);
        }
        break;
    case 6: { //sonstiges
            switch (cmb_misc_typ->currentIndex()) {
            case 0: ausdruck = new Invitation(this->event); break;
            case 1: { //Zeitplan
                    Timetable::setOrientation(QPrinter::Landscape);
                    ausdruck = new Timetable(this->event);
                }
                break;
            case 2: { //Meldematrix
                    RegistrationMatrix::setOrientation(QPrinter::Landscape);
                    RegistrationMatrix::setTeamMode(false);
                    ausdruck = new RegistrationMatrix(this->event);
                }
                break;
            case 3: { //Mannschaftsmatrix
                    RegistrationMatrix::setOrientation(QPrinter::Landscape);
                    RegistrationMatrix::setTeamMode(true);
                    ausdruck = new RegistrationMatrix(this->event);
                }
                break;
            case 4: { //Startpässe
                    ausdruck = new License(this->event);
                    ausdruck->setSelectClub(true);
                    ausdruck->setTypeString("Startpässe");
                }
                break;
            case 5: ausdruck = new ClubList(this->event); break;
            case 6: ausdruck = new ITCheckSquads(this->event); break;
            case 7: { //EDV-Checkliste TN
                    ausdruck = new ITCheckList(this->event);
                    ausdruck->setTypeString("EDV-Checkliste");
                }
                break;
            case 8: ausdruck = new Summary(this->event); break;
            case 9: ausdruck = new MedalCount(this->event); break;
            case 10: {
                    ausdruck = new ResultsSheet(this->event);
                    ausdruck->setSelectRiege(true);
                    ausdruck->setSelectDis(true);
                }
                break;
            }
        }
        break;
    }
    ausdruck->setShowPreview(chk_preview->isChecked());
    ausdruck->setOutputType(cmb_output->currentIndex());
    connect(ausdruck,SIGNAL(requestDetailInfo()),this,SLOT(showDetailinfoDialog()),Qt::BlockingQueuedConnection);
    connect(ausdruck,SIGNAL(requestDisziplinen()),this,SLOT(showDisziplinenDialog()),Qt::BlockingQueuedConnection);
    connect(ausdruck,SIGNAL(requestRiegen()),this,SLOT(showRiegenDialog()),Qt::BlockingQueuedConnection);
    connect(ausdruck,SIGNAL(requestTN()),this,SLOT(showTNDialog()),Qt::BlockingQueuedConnection);
    connect(ausdruck,SIGNAL(requestVereine()),this,SLOT(showVereineDialog()),Qt::BlockingQueuedConnection);
    connect(ausdruck,SIGNAL(requestWKs()),this,SLOT(showWKDialog()),Qt::BlockingQueuedConnection);
    connect(ausdruck,SIGNAL(showPrintPreview(QPrinter*)),this,SLOT(showPrintPreview(QPrinter*)),Qt::BlockingQueuedConnection);
    //if (ausdruck->printPreview()) ausdruck->~Drucken();
    ausdruck->start();
}

void Tab_DR::createCSV(int mode) {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    QStringList filters;
    filters << "CSV-Datei (*.csv)";
    dialog.setNameFilters(filters);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("csv");
    if(dialog.exec()) {
        QFile file(dialog.selectedFiles().at(0));
        file.open(QIODevice::WriteOnly);
        QTextStream stream(&file);
        QSqlQuery query5;
        query5.prepare("SELECT var_nummer, var_name FROM tfx_wettkaempfe WHERE int_veranstaltungenid=?  AND (SELECT COUNT(*) FROM tfx_wertungen WHERE int_wettkaempfeid=tfx_wettkaempfe.int_wettkaempfeid) > 0 ORDER BY var_nummer");
        query5.bindValue(0, this->event->getMainEventId());
        query5.exec();
        while (query5.next()) {
            stream << "Platz;";
            Competition *competition = Competition::getByNumber(this->event, query5.value(0).toString());
            QList<QStringList> rlist;
            if (mode == 0) {
                if (competition->getType() == 1) {
                    stream << "Verein;Mannschaft;";
                } else {
                    stream << "Name;Verein;Jg.;";
                }
                rlist = Result_Calc::resultArrayNew(competition);
                QSqlQuery dis;
                dis.prepare("SELECT tfx_disziplinen.var_name, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN generate_series(0,1) ELSE 0 END as kp FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND var_nummer=? ORDER BY int_sortierung, kp");
                dis.bindValue(0, this->event->getMainEventId());
                dis.bindValue(1, query5.value(0).toString());
                dis.exec();
                while (dis.next()) {
                    stream << dis.value(0).toString()+";";
                }
                stream << "Gesamt;";
            } else {
                stream << "Verein;Mannschaft;";
                rlist = Result_Calc::roundResultArrayNew(competition);
                for (int i=0;i<(rlist.at(0).size()-6)/2;i++) {
                    stream << "Pkt. "+QString::number(i+1)+". R.;Pl. "+QString::number(i+1)+". R.;";
                }
                stream << "Pkt. Ges.;Pl. Ges.;";
            }
            stream << "WKNr.;WK-Bezeichnung;Jahrgang\n";
            for (int i=0;i<rlist.size();i++) {
                for (int j=0;j<=rlist.at(i).size()+1;j++) {
                    if (j == rlist.at(i).size() - 1) {
                        stream << query5.value(0).toString() + ";";
                    } else if (j == rlist.at(i).size()) {
                        stream << query5.value(1).toString() + ";";
                    } else if (j == rlist.at(i).size()+1) {
                        stream << _global::wkBez(this->event, query5.value(0).toString()) + "\n";
                    } else {
                        stream << rlist.at(i).at(j).toLatin1() + ";";
                    }
                }
            }
        }
        file.close();
    }
}

void Tab_DR::showDetailinfoDialog() {
    Select_Detail_Dialog *dt = new Select_Detail_Dialog(this->event, cmb_detail->currentIndex());
    if (dt->exec() == 1) {
        QString detailQuery;
        switch (cmb_detail->currentIndex()) {
        case 1: detailQuery = " AND int_gaueid="+QString::number(dt->getId()); break;
        case 2: detailQuery = " AND int_verbaendeid="+QString::number(dt->getId()); break;
        case 3: detailQuery = " AND int_laenderid="+QString::number(dt->getId()); break;
        }
        ausdruck->setDetailQuery(detailQuery);
    } else {
        ausdruck->setFinish(true);
    }
}

void Tab_DR::showVereineDialog() {
    QList<int> selectedClubs;
    SelectClubDialog *vn = new SelectClubDialog(this->event);
    if (vn->exec() == 1) {
        QStringList vereine = vn->returnVereine();
        for (int i=0;i<vereine.size();i++) {
            selectedClubs.append(vereine.at(i).toInt());
        }
        ausdruck->setSelectedClubs(selectedClubs);
    } else {
        ausdruck->setFinish(true);
    }
}

void Tab_DR::showWKDialog() {
    Select_Wk_Dialog *wk = new Select_Wk_Dialog(this->event);
    if (wk->exec()) {
        ausdruck->setSelectedWKs(QStringList(wk->getWk()));
    } else {
        ausdruck->setFinish(true);
    }
}

void Tab_DR::showRiegenDialog() {
    Select_Rg_Dialog *rg = new Select_Rg_Dialog(this->event);
    if (rg->exec() == 1) {
        ausdruck->setSelectedRiegen(rg->getRg());
    } else {
        ausdruck->setFinish(true);
    }
}

void Tab_DR::showTNDialog() {
    Select_Tn_Dialog *tn = new Select_Tn_Dialog(this->event);
    if (tn->exec() == 1) {
        ausdruck->setSelectedTN(tn->getTnList());
        ausdruck->setSelectedTNWK(tn->getTnWk());
        ausdruck->setTeilnehmerNumbers(tn->getTnList());
    } else {
        ausdruck->setFinish(true);
    }
}

void Tab_DR::showDisziplinenDialog() {
    SelectDisciplineDialog *dis = new SelectDisciplineDialog(this->event);
    if (dis->exec() == 1) {
        ausdruck->setSelectedDisziplinen(dis->getDis());
    } else {
        ausdruck->setFinish(true);
    }
}

void Tab_DR::showPrintPreview(QPrinter* printer) {
    if (ausdruck->getOutputType() != 2) {
        dialog = new QPrintPreviewDialog(printer);
        dialog->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
        dialog->setWindowTitle("TurnFix - Drucken");
        dialog->setWindowIcon(QIcon(":/appicons/icon.png"));
        dialog->printer()->setPaperSize(ausdruck->getPaperSize());
        connect(dialog, SIGNAL(paintRequested(QPrinter*)), ausdruck,SLOT(print(QPrinter*)));
    }
    if (dialog->exec() == 0) {
        ausdruck->quit();
    }
}
