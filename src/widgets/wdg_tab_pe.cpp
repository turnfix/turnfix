#include <QSqlQuery>
#include <QKeyEvent>
#include "model/settings/session.h"
#include "model/objects/event.h"
#include "header/wdg_tab_pe.h"
#include "../dialogs/others/header/dlg_barcode.h"
#include "../dialogs/points/header/dlg_bogen.h"
#include "view/dialogs/scorecarddialog.h"
#include "../global/header/_global.h"

Tab_PE::Tab_PE(QWidget* parent) : QWidget(parent) {
    setupUi(this);

    this->event = Session::getInstance()->getEvent();

    connect(but_startbogen, SIGNAL(clicked()), this, SLOT(startBogen()));
    connect(but_startkarte, SIGNAL(clicked()), this, SLOT(startKarte()));
    connect(but_barcode, SIGNAL(clicked()), this, SLOT(startBarcode()));
    connect(cmb_squadno, SIGNAL(currentIndexChanged(QString)), this, SLOT(squadChange(QString)));
    this->installEventFilter(this);
    cmb_squadno->installEventFilter(this);
    cmb_apparatus->installEventFilter(this);
    sbx_squadno_2->installEventFilter(this);
    init();
}

void Tab_PE::init() {
    cmb_squadno->clear();
    QSqlQuery query2;
    query2.prepare("SELECT DISTINCT(var_riege) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=? ORDER BY var_riege");
    query2.bindValue(0, this->event->getMainEventId());
    query2.bindValue(1, this->event->getRound());
    query2.exec();
    while (query2.next()) {
        cmb_squadno->addItem(query2.value(0).toString(),query2.value(0).toString());
    }
}

void Tab_PE::startBogen() {
    bool kuer=false;
    if (cmb_apparatus->currentText().right(5) == "(Kür)") {
        kuer = true;
    }
    WK_Bogen *wk = new WK_Bogen(this->event, this);
    wk->init(cmb_squadno->currentText(), cmb_apparatus->itemData(cmb_apparatus->currentIndex()).toInt(),kuer);
    wk->exec();
}

void Tab_PE::startKarte() {
    ScoreCardDialog *wk = new ScoreCardDialog(this->event, this);
    QList< QList<int> > disids;
    QSqlQuery query9;
    query9.prepare("SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen INNER JOIN tfx_wertungen USING (int_wertungenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_startnummer=? AND int_veranstaltungenid=? AND int_runde=?");
    query9.bindValue(0,sbx_squadno_2->value());
    query9.bindValue(1, this->event->getMainEventId());
    query9.bindValue(2, this->event->getRound());
    query9.exec();
    QSqlQuery query2;
    if (_global::querySize(query9) == 0) {
        query2.prepare("SELECT tfx_disziplinen.int_disziplinenid, int_wertungenid, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen ON tfx_disziplinen.int_disziplinenid = tfx_wettkaempfe_x_disziplinen.int_disziplinenid INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_veranstaltungenid=? AND tfx_wertungen.int_startnummer=? AND tfx_wertungen.int_runde=? ORDER BY int_sortierung, kp");
    } else {
        query2.prepare("SELECT tfx_disziplinen.int_disziplinenid, int_wertungenid, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen ON tfx_disziplinen.int_disziplinenid = tfx_wettkaempfe_x_disziplinen.int_disziplinenid INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_veranstaltungenid=? AND tfx_wertungen.int_startnummer=? AND tfx_wertungen.int_runde=? AND tfx_disziplinen.int_disziplinenid IN (SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=tfx_wertungen.int_wertungenid) ORDER BY int_sortierung, kp");
    }
    query2.bindValue(0, this->event->getMainEventId());
    query2.bindValue(1, sbx_squadno_2->value());
    query2.bindValue(2, this->event->getRound());
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
    wk->init(sbx_squadno_2->value(), wertid, disids);
    wk->exec();
}

void Tab_PE::squadChange(QString squadno) {
    cmb_apparatus->clear();
    QSqlQuery query;
    query.prepare("SELECT DISTINCT int_disziplinenid, tfx_disziplinen.var_name, var_icon, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp FROM tfx_disziplinen INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_wertungen USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND var_riege=? ORDER BY tfx_disziplinen.var_name, kp");
    query.bindValue(0, this->event->getMainEventId());
    query.bindValue(1, squadno);
    query.exec();
    while (query.next()) {
        QString name = query.value(1).toString();
        if (query.value(3).toInt() == 1) {
            name += " (Pflicht)";
        }
        cmb_apparatus->addItem(QIcon(query.value(2).toString()),name,query.value(0).toInt());
        if (query.value(3).toInt() == 1) {
            cmb_apparatus->addItem(QIcon(query.value(2).toString()),query.value(1).toString()+" (Kür)",query.value(0).toInt());
        }
    }
}

void Tab_PE::startBarcode() {
    Barcode_Dialog *bar = new Barcode_Dialog(this);
    if (bar->exec()) {
        if (bar->getInput().contains("-")) {
            QStringList input = bar->getInput().split("-");
            input.removeLast();
            QString riege = input.join("-");
            cmb_squadno->setCurrentIndex(cmb_squadno->findText(riege,Qt::MatchFixedString));
            cmb_apparatus->setCurrentIndex(cmb_apparatus->findData(bar->getInput().split("-").last()));
            startBogen();
        } else {
            sbx_squadno_2->setValue(bar->getInput().toInt());
            startKarte();
        }
    }
}

bool Tab_PE::eventFilter(QObject *, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if (e->key() == Qt::Key_F6) {
            startBarcode();
            return true;
        }
    }
    return false;
}
