#include "mainwindow.h"
#include "export/maildialog.h"
#include "model/objects/event.h"
#include "model/settings/session.h"
#include "src/dialogs/database/header/dlg_db_control.h"
#include "src/dialogs/others/header/dlg_about.h"
#include "src/dialogs/participants/header/dlg_pass.h"
#include "src/dialogs/wk/header/dlg_event.h"
#include "src/global/header/settings.h"
#include <QActionGroup>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QToolBar>

MainWindow::MainWindow() {
    event = Session::getInstance()->getEvent();

    _global::initFields();
    setupUi(this);
    showMaximized();
    initTabs();
    updateTables(0);

    connect(act_change_contest, SIGNAL(triggered()), this, SLOT(changeWK()));
    connect(act_preferences, SIGNAL(triggered()), this, SLOT(showWKConf()));
    connect(act_nos, SIGNAL(triggered()), this, SLOT(newNumbers()));
    connect(act_mail, SIGNAL(triggered()), this, SLOT(sendMLists()));
    connect(act_exit, SIGNAL(triggered()), this, SLOT(close()));
    connect(act_competitordatabase, SIGNAL(triggered()), this, SLOT(showTNDB()));
    connect(act_clubdatabase, SIGNAL(triggered()), this, SLOT(showVNDB()));
    connect(act_events, SIGNAL(triggered()), this, SLOT(showDisDB()));
    connect(act_pass, SIGNAL(triggered()), this, SLOT(editPass()));
    connect(act_license, SIGNAL(triggered()), this, SLOT(showAbout()));

    QToolBar *tb = new QToolBar();
    tb->setAllowedAreas(Qt::LeftToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setOrientation(Qt::Vertical);
    tb->addAction(act_WK);
    tb->addAction(act_TN);
    tb->addAction(act_RG);
    tb->addAction(act_SR);
    tb->addAction(act_PE);
    tb->addAction(act_ER);
    tb->addAction(act_DR);
    tb->addAction(act_ST);
    tb->addAction(act_ED);

    QActionGroup *menu = new QActionGroup(this);
    menu->addAction(act_WK);
    menu->addAction(act_TN);
    menu->addAction(act_RG);
    menu->addAction(act_SR);
    menu->addAction(act_PE);
    menu->addAction(act_ER);
    menu->addAction(act_DR);
    menu->addAction(act_ST);
    menu->addAction(act_ED);

    connect(act_WK, &QAction::triggered, [this](){
        sta_wdg->setCurrentIndex(0);
        updateTables(0);
    });
    connect(act_TN, &QAction::triggered, [this](){
        sta_wdg->setCurrentIndex(1);
        updateTables(1);
    });
    connect(act_RG, &QAction::triggered, [this](){
        sta_wdg->setCurrentIndex(2);
        updateTables(2);
    });
    connect(act_SR, &QAction::triggered, [this](){
        sta_wdg->setCurrentIndex(3);
        updateTables(3);
    });
    connect(act_PE, &QAction::triggered, [this](){
        sta_wdg->setCurrentIndex(4);
        updateTables(4);
    });
    connect(act_ER, &QAction::triggered, [this](){
        sta_wdg->setCurrentIndex(5);
        updateTables(5);
    });
    connect(act_DR, &QAction::triggered, [this](){
        sta_wdg->setCurrentIndex(6);
        updateTables(6);
    });
    connect(act_ST, &QAction::triggered, [this](){
        sta_wdg->setCurrentIndex(7);
        updateTables(7);
    });
    connect(act_ED, &QAction::triggered, [this](){
        sta_wdg->setCurrentIndex(8);
        updateTables(8);
    });

    sidebar->layout()->addWidget(tb);
    this->installEventFilter(this);
}

void MainWindow::initTabs() {
    QSqlQuery query;
    query.prepare("SELECT int_runde, int_hauptwettkampf, bol_rundenwettkampf, tfx_veranstaltungen.var_name, tfx_wettkampforte.var_name, var_ort FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) WHERE int_veranstaltungenid=? LIMIT 1");
    query.bindValue(0, event->getId());
    query.exec();
    query.next();

    lbl_wk->setText(query.value(3).toString());
    lbl_ort->setText(query.value(4).toString()+", "+query.value(5).toString());
}

void MainWindow::updateTables(int init) {
    switch (init) {
    case 0 : wk_tab->fillWKTable(); break;
    case 1 : tn_tab->loadBestView(); tn_tab->refresh(); break;
    case 2 : rg_tab->fillRETable2(); break;
    case 3 : sr_tab->init(); break;
    case 4 : pe_tab->init(); break;
    case 5 : er_tab->updateERList(); er_tab->fillERTable(); break;
    case 6 : dr_tab->updateData(); break;
    case 7 : st_tab->updateStatus(); break;
    }
}

void MainWindow::showAbout() {
    About_Dialog *about = new About_Dialog(this);
    about->exec();
}

void MainWindow::showWKConf() {
    Event_Dialog *nwkw = new Event_Dialog(this->event, this->event->getId(),this);
    nwkw->exec();
}

void MainWindow::newNumbers() {
    int ret = QMessageBox::question(this,tr("Startnummern neu vergeben?"),tr("Sollen wirklich neue Startnummern vergeben werden?"),QMessageBox::Ok,QMessageBox::Cancel);
    if (ret != QMessageBox::Ok) return;
    bool ok;
    int firstNumber = QInputDialog::getInt(this,tr("Startnummern vergeben"),tr("Wählen sie die erste Startnummer die vergeben werden soll."),1,1,99999,1,&ok);
    if (!ok) return;
    QSqlQuery query;
    query.prepare("SELECT int_wertungenid, var_nummer FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer USING (int_teilnehmerid) LEFT JOIN tfx_gruppen USING (int_gruppenid) INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid OR tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid WHERE int_veranstaltungenid=? AND int_runde=? ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, var_nummer, int_mannschaftenid, int_wertungenid");
    query.bindValue(0, this->event->getMainEventId());
    query.bindValue(1, this->event->getRound());
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
    query4.bindValue(0, this->event->getMainEventId());
    query4.exec();
    while (query4.next()) {
        QSqlQuery query3;
        query3.prepare("UPDATE tfx_mannschaften SET int_startnummer=? WHERE int_mannschaftenid=?");
        query3.bindValue(0,mstnr);
        query3.bindValue(1,query4.value(0).toInt());
        query3.exec();
        mstnr++;
    }
    tn_tab->refresh();
}

void MainWindow::showTNDB() {
    Db_Control_Dialog *dbctrl = new Db_Control_Dialog(this,1);
    dbctrl->show();
}

void MainWindow::showVNDB() {
    Db_Control_Dialog *dbctrl = new Db_Control_Dialog(this,2);
    dbctrl->show();
}

void MainWindow::showDisDB() {
    Db_Control_Dialog *dbctrl = new Db_Control_Dialog(this,3);
    dbctrl->show();
}

void MainWindow::editPass() {
    Pass_Dialog *pass = new Pass_Dialog(this->event, this);
    pass->exec();
}

void MainWindow::changeWK() {
    /*Login_Dialog *select = new Login_Dialog(this);
    select->autoLogin();
    if (select->exec() == 1) {
        initTabs();
        act_WK->trigger();
        updateTables(0);
    }*/
}

void MainWindow::sendMLists() {
    MailDialog *vn = new MailDialog(this->event, this);
    // TODO check
    // vn->setDetailInfo(dr_tab->ui->cmb_detail->currentIndex());
    vn->exec();
}

void MainWindow::closeEvent(QCloseEvent *) {
    Settings::saveAllSettings();
}
