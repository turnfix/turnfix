#include "mainwindow.h"
#include "aboutdialog.h"
#include "eventdialog.h"
#include "export/maildialog.h"
#include "masterdata/masterdatadialog.h"
#include "model/entity/event.h"
#include "participants/licensenumberdialog.h"
#include "ui_mainwindow.h"
#include <QActionGroup>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QToolBar>

MainWindow::MainWindow(EntityManager *em, Event *event)
    : QMainWindow()
    , ui(new Ui::MainWindow)
    , m_em(em)
{
    m_event = event;

    _global::initFields();
    ui->setupUi(this);
    showMaximized();
    // updateTables(0);

    initEvent();

    connect(ui->act_change_contest, SIGNAL(triggered()), this, SLOT(changeWK()));
    connect(ui->act_preferences, SIGNAL(triggered()), this, SLOT(showEventDialog()));
    connect(ui->act_nos, SIGNAL(triggered()), this, SLOT(newNumbers()));
    connect(ui->act_mail, SIGNAL(triggered()), this, SLOT(sendMLists()));
    connect(ui->act_exit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->act_competitordatabase, SIGNAL(triggered()), this, SLOT(showTNDB()));
    connect(ui->act_clubdatabase, SIGNAL(triggered()), this, SLOT(showVNDB()));
    connect(ui->act_events, SIGNAL(triggered()), this, SLOT(showDisDB()));
    connect(ui->act_pass, SIGNAL(triggered()), this, SLOT(editPass()));
    connect(ui->act_license, SIGNAL(triggered()), this, SLOT(showAbout()));

    QToolBar *tb = new QToolBar();
    tb->setAllowedAreas(Qt::LeftToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setOrientation(Qt::Vertical);
    tb->addAction(ui->act_WK);
    tb->addAction(ui->act_TN);
    tb->addAction(ui->act_RG);
    tb->addAction(ui->act_SR);
    tb->addAction(ui->act_PE);
    tb->addAction(ui->act_ER);
    tb->addAction(ui->act_DR);
    tb->addAction(ui->act_ST);
    tb->addAction(ui->act_ED);

    QActionGroup *menu = new QActionGroup(this);
    menu->addAction(ui->act_WK);
    menu->addAction(ui->act_TN);
    menu->addAction(ui->act_RG);
    menu->addAction(ui->act_SR);
    menu->addAction(ui->act_PE);
    menu->addAction(ui->act_ER);
    menu->addAction(ui->act_DR);
    menu->addAction(ui->act_ST);
    menu->addAction(ui->act_ED);

    connect(ui->act_WK, &QAction::triggered, [this]() {
        ui->sta_wdg->setCurrentIndex(0);
        updateTables(0);
    });
    connect(ui->act_TN, &QAction::triggered, [this]() {
        ui->sta_wdg->setCurrentIndex(1);
        updateTables(1);
    });
    connect(ui->act_RG, &QAction::triggered, [this]() {
        ui->sta_wdg->setCurrentIndex(2);
        updateTables(2);
    });
    connect(ui->act_SR, &QAction::triggered, [this]() {
        ui->sta_wdg->setCurrentIndex(3);
        updateTables(3);
    });
    connect(ui->act_PE, &QAction::triggered, [this]() {
        ui->sta_wdg->setCurrentIndex(4);
        updateTables(4);
    });
    connect(ui->act_ER, &QAction::triggered, [this]() {
        ui->sta_wdg->setCurrentIndex(5);
        updateTables(5);
    });
    connect(ui->act_DR, &QAction::triggered, [this]() {
        ui->sta_wdg->setCurrentIndex(6);
        updateTables(6);
    });
    connect(ui->act_ST, &QAction::triggered, [this]() {
        ui->sta_wdg->setCurrentIndex(7);
        updateTables(7);
    });
    connect(ui->act_ED, &QAction::triggered, [this]() {
        ui->sta_wdg->setCurrentIndex(8);
        updateTables(8);
    });

    ui->sidebar->layout()->addWidget(tb);
    installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTables(int init)
{
    //    switch (init) {
    //    case 1 : tn_tab->loadBestView(); tn_tab->refresh(); break;
    //    case 2 : rg_tab->fillRETable2(); break;
    //    case 3 : sr_tab->init(); break;
    //    case 4 : pe_tab->init(); break;
    //    case 5 : er_tab->updateERList(); er_tab->fillERTable(); break;
    //    case 6 : dr_tab->updateData(); break;
    //    case 7 : st_tab->updateStatus(); break;
    //    }
}

void MainWindow::showAbout() {
    AboutDialog *about = new AboutDialog(this);
    about->exec();
}

void MainWindow::showEventDialog()
{
    EventDialog *nwkw = new EventDialog(m_event, m_em, this);
    if (nwkw->exec() == 1) {
        initEvent();
    }
}

void MainWindow::newNumbers() {
    int ret = QMessageBox::question(this,
                                    tr("Startnummern neu vergeben?"),
                                    tr("Sollen wirklich neue Startnummern vergeben werden?"),
                                    QMessageBox::Ok,
                                    QMessageBox::Cancel);
    if (ret != QMessageBox::Ok) return;
    bool ok;
    int firstNumber
        = QInputDialog::getInt(this,
                               tr("Startnummern vergeben"),
                               tr("Wählen sie die erste Startnummer die vergeben werden soll."),
                               1,
                               1,
                               99999,
                               1,
                               &ok);
    if (!ok) return;
    QSqlQuery query;
    query.prepare("SELECT int_wertungenid, var_nummer FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer USING (int_teilnehmerid) LEFT JOIN tfx_gruppen USING (int_gruppenid) INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid OR tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid WHERE int_veranstaltungenid=? AND int_runde=? ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, var_nummer, int_mannschaftenid, int_wertungenid");
    query.bindValue(0, m_event->mainEvent()->id());
    query.bindValue(1, m_event->round());
    query.exec();
    int stnr=firstNumber;
    int mstnr=1;
    while (query.next()) {
        QSqlQuery query2;
        query2.prepare("UPDATE tfx_wertungen SET int_startnummer=? WHERE int_wertungenid=?");
        query2.bindValue(0,stnr);
        query2.bindValue(1,query.value(0).toInt());
        query2.exec();
        stnr++;
    }
    QSqlQuery query4;
    query4.prepare("SELECT int_mannschaftenid FROM tfx_mannschaften INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_vereine USING (int_vereineid) WHERE int_veranstaltungenid=? ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_wettkaempfe.var_nummer, tfx_mannschaften.int_nummer");
    query4.bindValue(0, m_event->mainEvent()->id());
    query4.exec();
    while (query4.next()) {
        QSqlQuery query3;
        query3.prepare("UPDATE tfx_mannschaften SET int_startnummer=? WHERE int_mannschaftenid=?");
        query3.bindValue(0,mstnr);
        query3.bindValue(1,query4.value(0).toInt());
        query3.exec();
        mstnr++;
    }
    ui->tn_tab->refresh();
}

void MainWindow::showTNDB() {
    MasterdataDialog *dbctrl = new MasterdataDialog(m_em, this, MasterdataDialog::Type::AthleteData);
    dbctrl->show();
}

void MainWindow::showVNDB() {
    MasterdataDialog *dbctrl = new MasterdataDialog(m_em, this, MasterdataDialog::Type::ClubData);
    dbctrl->show();
}

void MainWindow::showDisDB() {
    MasterdataDialog *dbctrl = new MasterdataDialog(m_em,
                                                    this,
                                                    MasterdataDialog::Type::DisciplineData);
    dbctrl->show();
}

void MainWindow::editPass() {
    LicenseNumberDialog *pass = new LicenseNumberDialog(m_event, this);
    pass->exec();
}

void MainWindow::changeWK() {
    /*Login_Dialog *select = new Login_Dialog(this);
    select->autoLogin();
    if (select->exec() == 1) {
        initEvent();
        act_WK->trigger();
        updateTables(0);
    }*/
}

void MainWindow::sendMLists() {
    MailDialog *vn = new MailDialog(m_event, this);
    // TODO check
    // vn->setDetailInfo(dr_tab->ui->cmb_detail->currentIndex());
    vn->exec();
}

void MainWindow::initEvent()
{
    ui->lbl_wk->setText(m_event->name());
    ui->lbl_ort->setText(m_event->venue()->nameAndCity());
    ui->competitionsWidget->setup(m_event, m_em);
}
