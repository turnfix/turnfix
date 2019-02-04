#include <QFileDialog>
#include <QSqlQuery>
#include <QTextStream>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QMessageBox>
#include "header/wdg_tab_dr.h"
#include "../global/header/_global.h"
#include "../global/header/settings.h"
#include "../global/header/result_calc.h"
#include "../export/ergebnisse/header/einzel.h"
#include "../export/ergebnisse/header/detail.h"
#include "../export/ergebnisse/header/runde.h"
#include "../export/ergebnisse/header/tabelle.h"
#include "../export/teilnehmerlisten/header/ergebnisbogen.h"
#include "../export/teilnehmerlisten/header/riege.h"
#include "../export/teilnehmerlisten/header/meldung.h"
#include "../export/teilnehmerlisten/header/bogen.h"
#include "../export/teilnehmerlisten/header/pass.h"
#include "../export/teilnehmerlisten/header/edvchecktn.h"
#include "../export/misc/header/ausschreibung.h"
#include "../export/misc/header/bericht.h"
#include "../export/misc/header/edvcheckriege.h"
#include "../export/misc/header/karte.h"
#include "../export/misc/header/meldematrix.h"
#include "../export/misc/header/urkunde.h"
#include "../export/misc/header/vereinsliste.h"
#include "../export/misc/header/zeitplan.h"
#include "../export/misc/header/medallienspiegel.h"

#include "../dialogs/select/header/dlg_select_wk.h"
#include "../dialogs/select/header/dlg_select_club.h"
#include "../dialogs/select/header/dlg_select_rg.h"
#include "../dialogs/select/header/dlg_select_dis.h"
#include "../dialogs/select/header/dlg_select_tn.h"
#include "../dialogs/select/header/dlg_select_detail.h"


Tab_DR::Tab_DR(QWidget* parent) : QWidget(parent) {
    setupUi(this);
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
    Drucken::setCoverID(cmb_cover->itemData(cmb_cover->currentIndex()).toInt());
    Drucken::setDetailInfo(cmb_detail->currentIndex());
    Drucken::setHeadFootID(cmb_headfoot->itemData(cmb_headfoot->currentIndex()).toInt());
    Drucken::setPaperSize(QPrinter::A4);
    Drucken::setOrientation(QPrinter::Portrait);
    switch(cmb_printtype->currentIndex()) {
    case 0: { //Ergebnisse
            Ergebnisse::setnewPageWK(chk_res_pg->isChecked());
            switch (cmb_res_type->currentIndex()) {
            case 0: { //Einzel
                    ausdruck = new Einzel; }
                break;
            case 1: { //Detail
                    Detail::setPrintAW(chk_res_aw->isChecked());
                    ausdruck = new Detail; }
                break;
            case 2: { //Rundenergebnisse
                    Runde::setUseExtraScore(chk_res_sc->isChecked());
                    ausdruck = new Runde; }
                break;
            case 3: { //Tabelle
                    ausdruck = new Tabelle;
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
            Riege::setNewPageEach(chk_rg_page->isChecked());
            Riege::setOrder(cmb_rg_sort->currentIndex());
            ausdruck = new Riege;
            ausdruck->setSelectRiege(chk_rg_select->isChecked());
            ausdruck->setTypeString("Riegen");
        }
        break;
    case 2: { //Meldeliste
            ausdruck = new Meldung;
            ausdruck->setSelectClub(chk_ve_select->isChecked());
            ausdruck->setTypeString("Meldeliste");
        }
        break;
    case 3: { //Wettkampfbogen
            Bogen::setTeammode(chk_bo_split->isChecked());
            ausdruck = new Bogen;
            ausdruck->setSelectRiege(true);
            ausdruck->setSelectDis(true);
        }
        break;
    case 4: { //Wettkampfkarte
            Karte::setPaperSize(QPrinter::A5);
            ausdruck = new Karte;
            ausdruck->setSelectTN(true);
        }
        break;
    case 5: { //Urkunde
            Urkunde::setEineUrkunde(chk_ur_single->isChecked());
            Urkunde::setRundenErgebnisse(chk_ur_runde->isChecked());
            Urkunde::setPlatzWertung(chk_ur_platz->isChecked());
            Urkunde::setUrkundenID(cmb_urkunde->itemData(cmb_urkunde->currentIndex()).toInt());
            Urkunde::setEinzelErgebnis(chk_ur_einzelwertung->isChecked());
            ausdruck = new Urkunde;
            ausdruck->setSelectTN(true);
        }
        break;
    case 6: { //sonstiges
            switch (cmb_misc_typ->currentIndex()) {
            case 0: ausdruck = new Ausschreibung; break;
            case 1: { //Zeitplan
                    Zeitplan::setOrientation(QPrinter::Landscape);
                    ausdruck = new Zeitplan;
                }
                break;
            case 2: { //Meldematrix
                    MeldeMatrix::setOrientation(QPrinter::Landscape);
                    MeldeMatrix::setTeamMode(false);
                    ausdruck = new MeldeMatrix;
                }
                break;
            case 3: { //Mannschaftsmatrix
                    MeldeMatrix::setOrientation(QPrinter::Landscape);
                    MeldeMatrix::setTeamMode(true);
                    ausdruck = new MeldeMatrix;
                }
                break;
            case 4: { //Startpässe
                    ausdruck = new Pass;
                    ausdruck->setSelectClub(true);
                    ausdruck->setTypeString("Startpässe");
                }
                break;
            case 5: ausdruck = new Vereinsliste; break;
            case 6: ausdruck = new EDVCheckRiege; break;
            case 7: { //EDV-Checkliste TN
                    ausdruck = new EDVCheckTN;
                    ausdruck->setTypeString("EDV-Checkliste");
                }
                break;
            case 8: ausdruck = new Bericht; break;
            case 9: ausdruck = new Medallienspiegel; break;
            case 10: {
                    ausdruck = new ErgebnisBogen;
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
        query5.bindValue(0,_global::checkHWK());
        query5.exec();
        while (query5.next()) {
            stream << "Platz;";
            QList<QStringList> rlist;
            if (mode == 0) {
                if (_global::checkTyp(query5.value(0).toString()) == 1) {
                    stream << "Verein;Mannschaft;";
                } else {
                    stream << "Name;Verein;Jg.;";
                }
                rlist = Result_Calc::resultArrayNew(query5.value(0).toString());
                QSqlQuery dis;
                dis.prepare("SELECT tfx_disziplinen.var_name, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN generate_series(0,1) ELSE 0 END as kp FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND var_nummer=? ORDER BY int_sortierung, kp");
                dis.bindValue(0,_global::checkHWK());
                dis.bindValue(1,query5.value(0).toString());
                dis.exec();
                while (dis.next()) {
                    stream << dis.value(0).toString()+";";
                }
                stream << "Gesamt;";
            } else {
                stream << "Verein;Mannschaft;";
                rlist = Result_Calc::roundResultArrayNew(query5.value(0).toString());
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
                        stream << _global::wkBez(query5.value(0).toString()) + "\n";
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
    Select_Detail_Dialog *dt = new Select_Detail_Dialog(cmb_detail->currentIndex());
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
    Select_Club_Dialog *vn = new Select_Club_Dialog();
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
    Select_Wk_Dialog *wk = new Select_Wk_Dialog();
    if (wk->exec()) {
        ausdruck->setSelectedWKs(QStringList(wk->getWk()));
    } else {
        ausdruck->setFinish(true);
    }
}

void Tab_DR::showRiegenDialog() {
    Select_Rg_Dialog *rg = new Select_Rg_Dialog();
    if (rg->exec() == 1) {
        ausdruck->setSelectedRiegen(rg->getRg());
    } else {
        ausdruck->setFinish(true);
    }
}

void Tab_DR::showTNDialog() {
    Select_Tn_Dialog *tn = new Select_Tn_Dialog();
    if (tn->exec() == 1) {
        ausdruck->setSelectedTN(tn->getTnList());
        ausdruck->setSelectedTNWK(tn->getTnWk());
        ausdruck->setTeilnehmerNumbers(tn->getTnList());
    } else {
        ausdruck->setFinish(true);
    }
}

void Tab_DR::showDisziplinenDialog() {
    Select_Dis_Dialog *dis = new Select_Dis_Dialog();
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
