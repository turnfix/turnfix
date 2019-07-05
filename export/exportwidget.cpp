#include "exportwidget.h"
#include "misc/card.h"
#include "misc/certificate.h"
#include "misc/clublist.h"
#include "misc/invitation.h"
#include "misc/itchecksquads.h"
#include "misc/medalcount.h"
#include "misc/registrationmatrix.h"
#include "misc/summary.h"
#include "misc/timetable.h"
#include "model/objects/competition.h"
#include "model/settings/session.h"
#include "participants/itchecklist.h"
#include "participants/judgessheet.h"
#include "participants/license.h"
#include "participants/registration.h"
#include "participants/resultssheet.h"
#include "participants/squad.h"
#include "results/detail.h"
#include "results/individual.h"
#include "results/results.h"
#include "results/round.h"
#include "results/table.h"
#include "selectclubdialog.h"
#include "selectcompetitiondialog.h"
#include "selectdetaildialog.h"
#include "selectdisciplinedialog.h"
#include "selectparticipantdialog.h"
#include "selectsubdivisiondialog.h"
#include "src/global/header/_global.h"
#include "src/global/header/result_calc.h"
#include "src/global/header/settings.h"
#include "ui_exportwidget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QSqlQuery>
#include <QTextStream>

ExportWidget::ExportWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExportWidget)
{
    ui->setupUi(this);

    this->event = Session::getInstance()->getEvent();

    connect(ui->but_csvgesamt, SIGNAL(clicked()), this, SLOT(csvGesamt()));
    connect(ui->but_csvrunde, SIGNAL(clicked()), this, SLOT(csvRunde()));
    connect(ui->but_print, SIGNAL(clicked()), this, SLOT(startPrint()));
}

ExportWidget::~ExportWidget()
{
    delete ui;
}

void ExportWidget::updateData()
{
    QString urkunde = ui->cmb_urkunde->currentText();
    QString headfoot = ui->cmb_headfoot->currentText();
    QString cover = ui->cmb_cover->currentText();
    ui->cmb_urkunde->clear();
    ui->cmb_headfoot->clear();
    ui->cmb_cover->clear();
    ui->cmb_headfoot->addItem("kein individualisiertes Layout", 0);
    ui->cmb_cover->addItem("kein Cover drucken", 0);
    QSqlQuery getLayouts("SELECT int_layoutid, var_name FROM tfx_layouts ORDER BY var_name");
    while (getLayouts.next()) {
        ui->cmb_urkunde->addItem(getLayouts.value(1).toString(), getLayouts.value(0).toInt());
        ui->cmb_headfoot->addItem(getLayouts.value(1).toString(), getLayouts.value(0).toInt());
        ui->cmb_cover->addItem(getLayouts.value(1).toString(), getLayouts.value(0).toInt());
    }
    if (ui->cmb_urkunde->findText(urkunde) >= 0)
        ui->cmb_urkunde->setCurrentIndex(ui->cmb_urkunde->findText(urkunde));
    if (ui->cmb_headfoot->findText(headfoot) >= 0)
        ui->cmb_headfoot->setCurrentIndex(ui->cmb_headfoot->findText(headfoot));
    if (ui->cmb_cover->findText(cover) >= 0)
        ui->cmb_cover->setCurrentIndex(ui->cmb_cover->findText(cover));
}

void ExportWidget::csvGesamt()
{
    createCSV(0);
}

void ExportWidget::csvRunde()
{
    createCSV(1);
}

void ExportWidget::startPrint()
{
    Print::setCoverID(ui->cmb_cover->itemData(ui->cmb_cover->currentIndex()).toInt());
    Print::setDetailInfo(ui->cmb_detail->currentIndex());
    Print::setHeadFootID(ui->cmb_headfoot->itemData(ui->cmb_headfoot->currentIndex()).toInt());
    Print::setPaperSize(QPrinter::A4);
    Print::setOrientation(QPrinter::Portrait);
    switch (ui->cmb_printtype->currentIndex()) {
    case 0: { //Ergebnisse
        Results::setnewPageWK(ui->chk_res_pg->isChecked());
        switch (ui->cmb_res_type->currentIndex()) {
        case 0: { //Einzel
            ausdruck = new Individual(this->event);
        } break;
        case 1: { //Detail
            Detail::setPrintAW(ui->chk_res_aw->isChecked());
            ausdruck = new Detail(this->event);
        } break;
        case 2: { //Rundenergebnisse
            Round::setUseExtraScore(ui->chk_res_sc->isChecked());
            ausdruck = new Round(this->event);
        } break;
        case 3: { //Tabelle
            ausdruck = new Table(this->event);
            ausdruck->setTypeString("Tabelle");
        } break;
        }
        ausdruck->setSelectWK(ui->chk_res_wk->isChecked());
        ausdruck->setSelectDetail(ui->chk_res_detail->isChecked());
        ausdruck->setSelectClub(ui->chk_res_cl->isChecked());
        ausdruck->setTypeString("Ergebnisse");
    } break;
    case 1: { //Riegen
        Squad::setNewPageEach(ui->chk_rg_page->isChecked());
        Squad::setOrder(ui->cmb_rg_sort->currentIndex());
        ausdruck = new Squad(this->event);
        ausdruck->setSelectRiege(ui->chk_rg_select->isChecked());
        ausdruck->setTypeString("Riegen");
    } break;
    case 2: { //Meldeliste
            ausdruck = new Registration(this->event);
            ausdruck->setSelectClub(ui->chk_ve_select->isChecked());
            ausdruck->setTypeString("Meldeliste");
    } break;
    case 3: { //Wettkampfbogen
        JudgesSheet::setTeammode(ui->chk_bo_split->isChecked());
        ausdruck = new JudgesSheet(this->event);
        ausdruck->setSelectRiege(true);
        ausdruck->setSelectDis(true);
    } break;
    case 4: { //Wettkampfkarte
            Card::setPaperSize(QPrinter::A5);
            ausdruck = new Card(this->event);
            ausdruck->setSelectTN(true);
    } break;
    case 5: { //Urkunde
        Certificate::setEineUrkunde(ui->chk_ur_single->isChecked());
        Certificate::setRundenErgebnisse(ui->chk_ur_runde->isChecked());
        Certificate::setPlatzWertung(ui->chk_ur_platz->isChecked());
        Certificate::setUrkundenID(
            ui->cmb_urkunde->itemData(ui->cmb_urkunde->currentIndex()).toInt());
        Certificate::setEinzelErgebnis(ui->chk_ur_einzelwertung->isChecked());
        ausdruck = new Certificate(this->event);
        ausdruck->setSelectTN(true);
    } break;
    case 6: { //sonstiges
        switch (ui->cmb_misc_typ->currentIndex()) {
        case 0:
            ausdruck = new Invitation(this->event);
            break;
        case 1: { //Zeitplan
            Timetable::setOrientation(QPrinter::Landscape);
            ausdruck = new Timetable(this->event);
        } break;
        case 2: { //Meldematrix
            RegistrationMatrix::setOrientation(QPrinter::Landscape);
            RegistrationMatrix::setTeamMode(false);
            ausdruck = new RegistrationMatrix(this->event);
        } break;
        case 3: { //Mannschaftsmatrix
            RegistrationMatrix::setOrientation(QPrinter::Landscape);
            RegistrationMatrix::setTeamMode(true);
            ausdruck = new RegistrationMatrix(this->event);
        } break;
        case 4: { //Startpässe
            ausdruck = new License(this->event);
            ausdruck->setSelectClub(true);
            ausdruck->setTypeString("Startpässe");
        } break;
        case 5:
            ausdruck = new ClubList(this->event);
            break;
        case 6:
            ausdruck = new ITCheckSquads(this->event);
            break;
        case 7: { //EDV-Checkliste TN
            ausdruck = new ITCheckList(this->event);
            ausdruck->setTypeString("EDV-Checkliste");
        } break;
        case 8:
            ausdruck = new Summary(this->event);
            break;
        case 9:
            ausdruck = new MedalCount(this->event);
            break;
        case 10: {
            ausdruck = new ResultsSheet(this->event);
            ausdruck->setSelectRiege(true);
            ausdruck->setSelectDis(true);
        } break;
        }
    } break;
    }
    ausdruck->setShowPreview(ui->chk_preview->isChecked());
    ausdruck->setOutputType(ui->cmb_output->currentIndex());
    connect(ausdruck,
            SIGNAL(requestDetailInfo()),
            this,
            SLOT(showDetailinfoDialog()),
            Qt::BlockingQueuedConnection);
    connect(ausdruck,
            SIGNAL(requestDisziplinen()),
            this,
            SLOT(showDisziplinenDialog()),
            Qt::BlockingQueuedConnection);
    connect(ausdruck,
            SIGNAL(requestRiegen()),
            this,
            SLOT(showRiegenDialog()),
            Qt::BlockingQueuedConnection);
    connect(ausdruck, SIGNAL(requestTN()), this, SLOT(showTNDialog()), Qt::BlockingQueuedConnection);
    connect(ausdruck,
            SIGNAL(requestVereine()),
            this,
            SLOT(showVereineDialog()),
            Qt::BlockingQueuedConnection);
    connect(ausdruck, SIGNAL(requestWKs()), this, SLOT(showWKDialog()), Qt::BlockingQueuedConnection);
    connect(ausdruck,
            SIGNAL(showPrintPreview(QPrinter *)),
            this,
            SLOT(showPrintPreview(QPrinter *)),
            Qt::BlockingQueuedConnection);
    //if (ausdruck->printPreview()) ausdruck->~Drucken();
    ausdruck->start();
}

void ExportWidget::createCSV(int mode)
{
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

void ExportWidget::showDetailinfoDialog()
{
    SelectDetailDialog *dt = new SelectDetailDialog(this->event, ui->cmb_detail->currentIndex());
    if (dt->exec() == 1) {
        QString detailQuery;
        switch (ui->cmb_detail->currentIndex()) {
        case 1: detailQuery = " AND int_gaueid="+QString::number(dt->getId()); break;
        case 2: detailQuery = " AND int_verbaendeid="+QString::number(dt->getId()); break;
        case 3: detailQuery = " AND int_laenderid="+QString::number(dt->getId()); break;
        }
        ausdruck->setDetailQuery(detailQuery);
    } else {
        ausdruck->setFinish(true);
    }
}

void ExportWidget::showVereineDialog()
{
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

void ExportWidget::showWKDialog()
{
    SelectCompetitionDialog *wk = new SelectCompetitionDialog(this->event);
    if (wk->exec()) {
        ausdruck->setSelectedWKs(QStringList(wk->getWk()));
    } else {
        ausdruck->setFinish(true);
    }
}

void ExportWidget::showRiegenDialog()
{
    SelectSubdivisionDialog *rg = new SelectSubdivisionDialog(this->event);
    if (rg->exec() == 1) {
        ausdruck->setSelectedRiegen(rg->getRg());
    } else {
        ausdruck->setFinish(true);
    }
}

void ExportWidget::showTNDialog()
{
    SelectParticipantDialog *tn = new SelectParticipantDialog(this->event);
    if (tn->exec() == 1) {
        ausdruck->setSelectedTN(tn->getTnList());
        ausdruck->setSelectedTNWK(tn->getTnWk());
        ausdruck->setTeilnehmerNumbers(tn->getTnList());
    } else {
        ausdruck->setFinish(true);
    }
}

void ExportWidget::showDisziplinenDialog()
{
    SelectDisciplineDialog *dis = new SelectDisciplineDialog(this->event);
    if (dis->exec() == 1) {
        ausdruck->setSelectedDisziplinen(dis->getDis());
    } else {
        ausdruck->setFinish(true);
    }
}

void ExportWidget::showPrintPreview(QPrinter *printer)
{
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
