#include "capturewidget.h"
#include "barcodedialog.h"
#include "model/entity/event.h"
#include "model/settings/session.h"
#include "resultssheetdialog.h"
#include "scorecarddialog.h"
#include "src/global/header/_global.h"
#include "ui_capturewidget.h"
#include <QKeyEvent>
#include <QSqlQuery>

CaptureWidget::CaptureWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CaptureWidget)
{
    ui->setupUi(this);

    this->m_event = Session::getInstance()->getEvent();

    connect(ui->but_startbogen, SIGNAL(clicked()), this, SLOT(startBogen()));
    connect(ui->but_startkarte, SIGNAL(clicked()), this, SLOT(startKarte()));
    connect(ui->but_barcode, SIGNAL(clicked()), this, SLOT(startBarcode()));
    connect(ui->cmb_squadno, SIGNAL(currentIndexChanged(QString)), this, SLOT(squadChange(QString)));
    this->installEventFilter(this);
    ui->cmb_squadno->installEventFilter(this);
    ui->cmb_apparatus->installEventFilter(this);
    ui->sbx_squadno_2->installEventFilter(this);
    init();
}

CaptureWidget::~CaptureWidget()
{
    delete ui;
}

void CaptureWidget::init()
{
    //    ui->cmb_squadno->clear();
    //    QSqlQuery query2;
    //    query2.prepare("SELECT DISTINCT(var_riege) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? ORDER BY var_riege");
    //    query2.bindValue(0, this->event->mainEvent()->id());
    //    query2.bindValue(1, this->event->round());
    //    query2.exec();
    //    while (query2.next()) {
    //        ui->cmb_squadno->addItem(query2.value(0).toString(), query2.value(0).toString());
    //    }
}

void CaptureWidget::startBogen()
{
    bool kuer=false;
    if (ui->cmb_apparatus->currentText().right(5) == "(Kür)") {
        kuer = true;
    }
    ResultsSheetDialog *wk = new ResultsSheetDialog(this->m_event, this);
    wk->init(ui->cmb_squadno->currentText(),
             ui->cmb_apparatus->itemData(ui->cmb_apparatus->currentIndex()).toInt(),
             kuer);
    wk->exec();
}

void CaptureWidget::startKarte()
{
    ScoreCardDialog *wk = new ScoreCardDialog(this->m_event, this);
    QList< QList<int> > disids;
    QSqlQuery query9;
    query9.prepare("SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen INNER JOIN tfx_wertungen USING (int_wertungenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_startnummer=? AND int_veranstaltungenid=? AND int_runde=?");
    query9.bindValue(0, ui->sbx_squadno_2->value());
    query9.bindValue(1, this->m_event->mainEvent()->id());
    query9.bindValue(2, this->m_event->round());
    query9.exec();
    QSqlQuery query2;
    if (_global::querySize(query9) == 0) {
        query2.prepare("SELECT tfx_disziplinen.int_disziplinenid, int_wertungenid, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen ON tfx_disziplinen.int_disziplinenid = tfx_wettkaempfe_x_disziplinen.int_disziplinenid INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_veranstaltungenid=? AND tfx_wertungen.int_startnummer=? AND tfx_wertungen.int_runde=? ORDER BY int_sortierung, kp");
    } else {
        query2.prepare("SELECT tfx_disziplinen.int_disziplinenid, int_wertungenid, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen ON tfx_disziplinen.int_disziplinenid = tfx_wettkaempfe_x_disziplinen.int_disziplinenid INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_veranstaltungenid=? AND tfx_wertungen.int_startnummer=? AND tfx_wertungen.int_runde=? AND tfx_disziplinen.int_disziplinenid IN (SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=tfx_wertungen.int_wertungenid) ORDER BY int_sortierung, kp");
    }
    query2.bindValue(0, this->m_event->mainEvent()->id());
    query2.bindValue(1, ui->sbx_squadno_2->value());
    query2.bindValue(2, this->m_event->round());
    query2.exec();
    int wertid;
    while (query2.next()) {
        for (int i=0;i<=query2.value(2).toInt();i++) {
            QList<int> lst;
            lst.append(query2.value(0).toInt());
            lst.append(i);
            disids.append(lst);
        }
        wertid = query2.value(1).toInt();
    }
    wk->init(ui->sbx_squadno_2->value(), wertid, disids);
    wk->exec();
}

void CaptureWidget::squadChange(QString squadno)
{
    ui->cmb_apparatus->clear();
    QSqlQuery query;
    query.prepare("SELECT DISTINCT int_disziplinenid, tfx_disziplinen.var_name, var_icon, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp FROM tfx_disziplinen INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_wertungen USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND var_riege=? ORDER BY tfx_disziplinen.var_name, kp");
    query.bindValue(0, this->m_event->mainEvent()->id());
    query.bindValue(1, squadno);
    query.exec();
    while (query.next()) {
        QString name = query.value(1).toString();
        if (query.value(3).toInt() == 1) {
            name += " (Pflicht)";
        }
        ui->cmb_apparatus->addItem(QIcon(query.value(2).toString()), name, query.value(0).toInt());
        if (query.value(3).toInt() == 1) {
            ui->cmb_apparatus->addItem(QIcon(query.value(2).toString()),
                                       query.value(1).toString() + " (Kür)",
                                       query.value(0).toInt());
        }
    }
}

void CaptureWidget::startBarcode()
{
    BarcodeDialog *bar = new BarcodeDialog(this);
    if (bar->exec()) {
        if (bar->getInput().contains("-")) {
            QStringList input = bar->getInput().split("-");
            input.removeLast();
            QString riege = input.join("-");
            ui->cmb_squadno->setCurrentIndex(ui->cmb_squadno->findText(riege, Qt::MatchFixedString));
            ui->cmb_apparatus->setCurrentIndex(
                ui->cmb_apparatus->findData(bar->getInput().split("-").last()));
            startBogen();
        } else {
            ui->sbx_squadno_2->setValue(bar->getInput().toInt());
            startKarte();
        }
    }
}

bool CaptureWidget::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if (e->key() == Qt::Key_F6) {
            startBarcode();
            return true;
        }
    }
    return false;
}
