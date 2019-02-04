#include <QSqlQuery>
#include <QMessageBox>
#include <QToolBar>
#include <QSignalMapper>
#include "header/dlg_event.h"
#include "../database/header/dlg_db_person.h"
#include "../database/header/dlg_db_loc.h"
#include "../database/header/dlg_db_acc.h"
#include "../../global/header/_global.h"

Event_Dialog::Event_Dialog(int edit, QWidget* parent) : QDialog(parent) {
    editid=edit;
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    QToolBar *tb = new QToolBar();
    QActionGroup *ag = new QActionGroup(this);
    tb->setAllowedAreas(Qt::LeftToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setOrientation(Qt::Vertical);
    tb->addAction(act_event);
    ag->addAction(act_event);
    tb->addAction(act_aus);
    ag->addAction(act_aus);
    tb->addAction(act_round);
    ag->addAction(act_round);
    act_event->setChecked(true);
    sidebar->layout()->addWidget(tb);
    QSignalMapper *mapper = new QSignalMapper(this);
    mapper->setMapping(act_event, 0);
    mapper->setMapping(act_aus, 1);
    mapper->setMapping(act_round, 2);
    connect(act_event, SIGNAL(triggered()), mapper, SLOT(map()));
    connect(act_aus, SIGNAL(triggered()), mapper, SLOT(map()));
    connect(act_round, SIGNAL(triggered()), mapper, SLOT(map()));
    connect(mapper, SIGNAL(mapped(int)), stackedWidget, SLOT(setCurrentIndex(int)));

    dae_from->setDate(QDate::currentDate());
    dae_to->setDate(QDate::currentDate());
    dae_ms->setDate(QDate::currentDate());
    QSqlQuery query2("SELECT int_veranstaltungenid, dat_von, var_name FROM tfx_veranstaltungen ORDER BY dat_von DESC");
    cmb_mainround->addItem(tr("Dies ist der Hauptwettkampf"),_global::getWkNr());
    while (query2.next()) {
        cmb_mainround->addItem(QDate().fromString(query2.value(1).toString(),"yyyy-MM-dd").toString("dd.MM.yyyy") + " - " + query2.value(2).toString(),query2.value(0).toInt());
    }
    cmb_mainround->setCurrentIndex(0);
    connect(but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(but_addp, SIGNAL(clicked()), this, SLOT(addPerson()));
    connect(but_addp2, SIGNAL(clicked()), this, SLOT(addPerson()));
    connect(but_addort, SIGNAL(clicked()), this, SLOT(addLocation()));
    connect(but_addacc, SIGNAL(clicked()), this, SLOT(addAccount()));
    readLocations();
    readPersons();
    readAccounts();
    initData();
}

int Event_Dialog::getWknr() {
    return wknr;
}

void Event_Dialog::readLocations() {
    QString currtext = cmb_locations->currentText();
    cmb_locations->clear();
    QSqlQuery query("SELECT int_wettkampforteid, var_name || ', ' || var_ort FROM tfx_wettkampforte ORDER BY var_name, var_ort");
    while (query.next()) {
        cmb_locations->addItem(query.value(1).toString(),query.value(0).toInt());
    }
    cmb_locations->setCurrentIndex(cmb_locations->findText(currtext));
}

void Event_Dialog::readPersons() {
    QString currtext1 = cmb_persons1->currentText();
    QString currtext2 = cmb_persons2->currentText();
    cmb_persons1->clear();
    cmb_persons2->clear();
    cmb_persons1->addItem("");
    cmb_persons2->addItem("");
    QSqlQuery query("SELECT int_personenid, var_nachname || ', ' || var_vorname FROM tfx_personen ORDER BY var_nachname, var_vorname");
    while (query.next()) {
        cmb_persons1->addItem(query.value(1).toString(),query.value(0).toInt());
        cmb_persons2->addItem(query.value(1).toString(),query.value(0).toInt());
    }
    cmb_persons1->setCurrentIndex(cmb_persons1->findText(currtext1));
    cmb_persons2->setCurrentIndex(cmb_persons2->findText(currtext2));
}

void Event_Dialog::readAccounts() {
    QString currtext = cmb_account->currentText();
    cmb_account->clear();
    cmb_account->addItem("");
    QSqlQuery query("SELECT int_kontenid, var_name FROM tfx_konten ORDER BY var_name");
    while (query.next()) {
        cmb_account->addItem(query.value(1).toString(),query.value(0).toInt());
    }
    cmb_account->setCurrentIndex(cmb_account->findText(currtext));
}

void Event_Dialog::addLocation() {
    Db_Loc_Dialog *loc = new Db_Loc_Dialog(0,this);
    if(loc->exec() == 1) { readLocations(); }
}

void Event_Dialog::addPerson() {
    Db_Pe_Dialog *pe = new Db_Pe_Dialog(0,this);
    if(pe->exec() == 1) { readPersons(); }
}

void Event_Dialog::addAccount() {
    Db_Acc_Dialog *ac = new Db_Acc_Dialog(0,this);
    if(ac->exec() == 1) { readAccounts(); }
}

void Event_Dialog::initData() {
    if (editid != 0) {
        QSqlQuery query;
        query.prepare("SELECT tfx_veranstaltungen.var_name, dat_von, dat_bis, dat_meldeschluss, int_wettkampforteid, var_veranstalter, int_ansprechpartner, int_meldung_an, var_meldung_website, int_edv, int_helfer, int_kampfrichter, rel_meldegeld, bol_faellig_nichtantritt, bol_ummeldung_moeglich, bol_nachmeldung_moeglich, rel_nachmeldung, txt_meldung_an, txt_startberechtigung, txt_teilnahmebedingungen, txt_siegerauszeichnung, txt_kampfrichter, txt_hinweise, int_kontenid, int_runde, int_hauptwettkampf, bol_rundenwettkampf FROM tfx_veranstaltungen WHERE int_veranstaltungenid=?");
        query.bindValue(0,editid);
        query.exec();
        query.next();
        txt_event->setText(query.value(0).toString());
        dae_from->setDate(QDate().fromString(query.value(1).toString(),"yyyy-MM-dd"));
        dae_to->setDate(QDate().fromString(query.value(2).toString(),"yyyy-MM-dd"));
        dae_ms->setDate(QDate().fromString(query.value(3).toString(),"yyyy-MM-dd"));
        cmb_locations->setCurrentIndex(cmb_locations->findData(query.value(4).toInt()));
        txt_orga->setText(query.value(5).toString());
        cmb_persons1->setCurrentIndex(cmb_persons1->findData(query.value(6).toInt()));
        cmb_persons2->setCurrentIndex(cmb_persons2->findData(query.value(7).toInt()));
        txt_web->setText(query.value(8).toString());
        sbx_edv->setValue(query.value(9).toInt());
        sbx_ref->setValue(query.value(11).toInt());
        sbx_volu->setValue(query.value(10).toInt());
        dsb_money->setValue(query.value(12).toDouble());
        chk_nostart->setChecked(query.value(13).toBool());
        chk_rereg->setChecked(query.value(14).toBool());
        chk_postreg->setChecked(query.value(15).toBool());
        dsb_postmoney->setValue(query.value(16).toDouble());
        txt_regtext->setText(query.value(17).toString());
        txt_startproof->setText(query.value(18).toString());
        txt_requirements->setText(query.value(19).toString());
        txt_certificate->setText(query.value(20).toString());
        txt_ref->setText(query.value(21).toString());
        txt_misc->setText(query.value(22).toString());
        cmb_account->setCurrentIndex(cmb_account->findData(query.value(23).toInt()));
        sbx_round->setValue(query.value(24).toInt());
        cmb_mainround->setCurrentIndex(cmb_mainround->findData(query.value(25).toInt()));
        gbx_round->setChecked(query.value(26).toBool());
    }
}

void Event_Dialog::save() {
    if (cmb_locations->currentText().length() == 0) {
        QMessageBox msg(QMessageBox::Warning,tr("Keine Wettkampfort gewählt!"),tr("Bitte wählen sie einen Wettkampfort aus!"));
        msg.exec();
    } else {
        QSqlQuery query;
        if (editid == 0) {
            query.prepare("INSERT INTO tfx_veranstaltungen (int_wettkampforteid,int_meldung_an,int_ansprechpartner,int_kontenid,int_hauptwettkampf,var_name,int_runde,dat_von,dat_bis,dat_meldeschluss,bol_rundenwettkampf,var_veranstalter,int_edv,int_helfer,int_kampfrichter,var_meldung_website,var_verwendungszweck,rel_meldegeld,rel_nachmeldung,bol_faellig_nichtantritt,bol_ummeldung_moeglich,bol_nachmeldung_moeglich,txt_meldung_an,txt_startberechtigung,txt_teilnahmebedingungen,txt_siegerauszeichnung,txt_kampfrichter,txt_hinweise) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        } else {
            query.prepare("UPDATE tfx_veranstaltungen SET int_wettkampforteid=?,int_meldung_an=?,int_ansprechpartner=?,int_kontenid=?,int_hauptwettkampf=?,var_name=?,int_runde=?,dat_von=?,dat_bis=?,dat_meldeschluss=?,bol_rundenwettkampf=?,var_veranstalter=?,int_edv=?,int_helfer=?,int_kampfrichter=?,var_meldung_website=?,var_verwendungszweck=?,rel_meldegeld=?,rel_nachmeldung=?,bol_faellig_nichtantritt=?,bol_ummeldung_moeglich=?,bol_nachmeldung_moeglich=?,txt_meldung_an=?,txt_startberechtigung=?,txt_teilnahmebedingungen=?,txt_siegerauszeichnung=?,txt_kampfrichter=?,txt_hinweise=? WHERE int_veranstaltungenid=?");
            query.bindValue(28,editid);
        }
        query.bindValue(0,cmb_locations->itemData(cmb_locations->currentIndex()));
        query.bindValue(1,cmb_persons2->itemData(cmb_persons2->currentIndex()));
        query.bindValue(2,cmb_persons1->itemData(cmb_persons1->currentIndex()));
        query.bindValue(3,cmb_account->itemData(cmb_account->currentIndex()));
        if (cmb_mainround->itemData(cmb_mainround->currentIndex()).toInt()>0) {
            query.bindValue(4,cmb_mainround->itemData(cmb_mainround->currentIndex()));
        }
        query.bindValue(5,txt_event->text());
        query.bindValue(6,sbx_round->value());
        query.bindValue(7,dae_from->date().toString("yyyy-MM-dd"));
        query.bindValue(8,dae_to->date().toString("yyyy-MM-dd"));
        query.bindValue(9,dae_ms->date().toString("yyyy-MM-dd"));
        query.bindValue(10,gbx_round->isChecked());
        query.bindValue(11,txt_orga->text());
        query.bindValue(12,sbx_edv->value());
        query.bindValue(13,sbx_volu->value());
        query.bindValue(14,sbx_ref->value());
        query.bindValue(15,txt_web->text());
        query.bindValue(16,txt_zweck->text());
        query.bindValue(17,dsb_money->value());
        query.bindValue(18,dsb_postmoney->value());
        query.bindValue(19,chk_nostart->isChecked());
        query.bindValue(20,chk_rereg->isChecked());
        query.bindValue(21,chk_postreg->isChecked());
        query.bindValue(22,txt_regtext->toPlainText());
        query.bindValue(23,txt_startproof->toPlainText());
        query.bindValue(24,txt_requirements->toPlainText());
        query.bindValue(25,txt_certificate->toPlainText());
        query.bindValue(26,txt_ref->toPlainText());
        query.bindValue(27,txt_misc->toPlainText());
        query.exec();
        if (editid==0) {
            if (_global::getDBTyp() == 0) {
                QSqlQuery query2("SELECT last_value FROM tfx_veranstaltungen_int_veranstaltungenid_seq");
                query2.next();
                wknr = query2.value(0).toInt();
            } else {
                wknr = query.lastInsertId().toInt();
            }
        }
        done(1);
    }
}
