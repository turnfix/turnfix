#include "eventdialog.h"
#include "masterdata/bankaccountdialog.h"
#include "masterdata/persondialog.h"
#include "masterdata/venuedialog.h"
#include "model/objects/event.h"
#include "src/global/header/_global.h"
#include "ui_eventdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QToolBar>

EventDialog::EventDialog(Event *event, int edit, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EventDialog)
{
    editid = edit;
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    this->event = event;

    QToolBar *tb = new QToolBar();
    QActionGroup *ag = new QActionGroup(this);
    tb->setAllowedAreas(Qt::LeftToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setOrientation(Qt::Vertical);
    tb->addAction(ui->act_event);
    ag->addAction(ui->act_event);
    tb->addAction(ui->act_aus);
    ag->addAction(ui->act_aus);
    tb->addAction(ui->act_round);
    ag->addAction(ui->act_round);
    ui->act_event->setChecked(true);
    ui->sidebar->layout()->addWidget(tb);

    connect(ui->act_event, &QAction::triggered, [this]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->act_aus, &QAction::triggered, [this]() { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->act_round, &QAction::triggered, [this]() { ui->stackedWidget->setCurrentIndex(2); });

    ui->dae_from->setDate(QDate::currentDate());
    ui->dae_to->setDate(QDate::currentDate());
    ui->dae_ms->setDate(QDate::currentDate());
    QSqlQuery query2("SELECT int_veranstaltungenid, dat_von, var_name FROM tfx_veranstaltungen ORDER BY dat_von DESC");
    ui->cmb_mainround->addItem(tr("Dies ist der Hauptwettkampf"), this->event->getId());
    while (query2.next()) {
        ui->cmb_mainround->addItem(QDate().fromString(query2.value(1).toString(), "yyyy-MM-dd")
                                           .toString("dd.MM.yyyy")
                                       + " - " + query2.value(2).toString(),
                                   query2.value(0).toInt());
    }
    ui->cmb_mainround->setCurrentIndex(0);
    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->but_addp, SIGNAL(clicked()), this, SLOT(addPerson()));
    connect(ui->but_addp2, SIGNAL(clicked()), this, SLOT(addPerson()));
    connect(ui->but_addort, SIGNAL(clicked()), this, SLOT(addLocation()));
    connect(ui->but_addacc, SIGNAL(clicked()), this, SLOT(addAccount()));
    readLocations();
    readPersons();
    readAccounts();
    initData();
}

EventDialog::~EventDialog()
{
    delete ui;
}

int EventDialog::getWknr()
{
    return wknr;
}

void EventDialog::readLocations()
{
    QString currtext = ui->cmb_locations->currentText();
    ui->cmb_locations->clear();
    QSqlQuery query("SELECT int_wettkampforteid, var_name || ', ' || var_ort FROM tfx_wettkampforte ORDER BY var_name, var_ort");
    while (query.next()) {
        ui->cmb_locations->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    ui->cmb_locations->setCurrentIndex(ui->cmb_locations->findText(currtext));
}

void EventDialog::readPersons()
{
    QString currtext1 = ui->cmb_persons1->currentText();
    QString currtext2 = ui->cmb_persons2->currentText();
    ui->cmb_persons1->clear();
    ui->cmb_persons2->clear();
    ui->cmb_persons1->addItem("");
    ui->cmb_persons2->addItem("");
    QSqlQuery query("SELECT int_personenid, var_nachname || ', ' || var_vorname FROM tfx_personen ORDER BY var_nachname, var_vorname");
    while (query.next()) {
        ui->cmb_persons1->addItem(query.value(1).toString(), query.value(0).toInt());
        ui->cmb_persons2->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    ui->cmb_persons1->setCurrentIndex(ui->cmb_persons1->findText(currtext1));
    ui->cmb_persons2->setCurrentIndex(ui->cmb_persons2->findText(currtext2));
}

void EventDialog::readAccounts()
{
    QString currtext = ui->cmb_account->currentText();
    ui->cmb_account->clear();
    ui->cmb_account->addItem("");
    QSqlQuery query("SELECT int_kontenid, var_name FROM tfx_konten ORDER BY var_name");
    while (query.next()) {
        ui->cmb_account->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    ui->cmb_account->setCurrentIndex(ui->cmb_account->findText(currtext));
}

void EventDialog::addLocation()
{
    VenueDialog *loc = new VenueDialog(0, this);
    if(loc->exec() == 1) { readLocations(); }
}

void EventDialog::addPerson()
{
    PersonDialog *pe = new PersonDialog(0, this);
    if(pe->exec() == 1) { readPersons(); }
}

void EventDialog::addAccount()
{
    BankAccountDialog *ac = new BankAccountDialog(0, this);
    if(ac->exec() == 1) { readAccounts(); }
}

void EventDialog::initData()
{
    if (editid != 0) {
        QSqlQuery query;
        query.prepare("SELECT tfx_veranstaltungen.var_name, dat_von, dat_bis, dat_meldeschluss, int_wettkampforteid, var_veranstalter, int_ansprechpartner, int_meldung_an, var_meldung_website, int_edv, int_helfer, int_kampfrichter, rel_meldegeld, bol_faellig_nichtantritt, bol_ummeldung_moeglich, bol_nachmeldung_moeglich, rel_nachmeldung, txt_meldung_an, txt_startberechtigung, txt_teilnahmebedingungen, txt_siegerauszeichnung, txt_kampfrichter, txt_hinweise, int_kontenid, int_runde, int_hauptwettkampf, bol_rundenwettkampf FROM tfx_veranstaltungen WHERE int_veranstaltungenid=?");
        query.bindValue(0,editid);
        query.exec();
        query.next();
        ui->txt_event->setText(query.value(0).toString());
        ui->dae_from->setDate(QDate().fromString(query.value(1).toString(), "yyyy-MM-dd"));
        ui->dae_to->setDate(QDate().fromString(query.value(2).toString(), "yyyy-MM-dd"));
        ui->dae_ms->setDate(QDate().fromString(query.value(3).toString(), "yyyy-MM-dd"));
        ui->cmb_locations->setCurrentIndex(ui->cmb_locations->findData(query.value(4).toInt()));
        ui->txt_orga->setText(query.value(5).toString());
        ui->cmb_persons1->setCurrentIndex(ui->cmb_persons1->findData(query.value(6).toInt()));
        ui->cmb_persons2->setCurrentIndex(ui->cmb_persons2->findData(query.value(7).toInt()));
        ui->txt_web->setText(query.value(8).toString());
        ui->sbx_edv->setValue(query.value(9).toInt());
        ui->sbx_ref->setValue(query.value(11).toInt());
        ui->sbx_volu->setValue(query.value(10).toInt());
        ui->dsb_money->setValue(query.value(12).toDouble());
        ui->chk_nostart->setChecked(query.value(13).toBool());
        ui->chk_rereg->setChecked(query.value(14).toBool());
        ui->chk_postreg->setChecked(query.value(15).toBool());
        ui->dsb_postmoney->setValue(query.value(16).toDouble());
        ui->txt_regtext->setText(query.value(17).toString());
        ui->txt_startproof->setText(query.value(18).toString());
        ui->txt_requirements->setText(query.value(19).toString());
        ui->txt_certificate->setText(query.value(20).toString());
        ui->txt_ref->setText(query.value(21).toString());
        ui->txt_misc->setText(query.value(22).toString());
        ui->cmb_account->setCurrentIndex(ui->cmb_account->findData(query.value(23).toInt()));
        ui->sbx_round->setValue(query.value(24).toInt());
        ui->cmb_mainround->setCurrentIndex(ui->cmb_mainround->findData(query.value(25).toInt()));
        ui->gbx_round->setChecked(query.value(26).toBool());
    }
}

void EventDialog::save()
{
    if (ui->cmb_locations->currentText().length() == 0) {
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
        query.bindValue(0, ui->cmb_locations->itemData(ui->cmb_locations->currentIndex()));
        query.bindValue(1, ui->cmb_persons2->itemData(ui->cmb_persons2->currentIndex()));
        query.bindValue(2, ui->cmb_persons1->itemData(ui->cmb_persons1->currentIndex()));
        query.bindValue(3, ui->cmb_account->itemData(ui->cmb_account->currentIndex()));
        if (ui->cmb_mainround->itemData(ui->cmb_mainround->currentIndex()).toInt() > 0) {
            query.bindValue(4, ui->cmb_mainround->itemData(ui->cmb_mainround->currentIndex()));
        }
        query.bindValue(5, ui->txt_event->text());
        query.bindValue(6, ui->sbx_round->value());
        query.bindValue(7, ui->dae_from->date().toString("yyyy-MM-dd"));
        query.bindValue(8, ui->dae_to->date().toString("yyyy-MM-dd"));
        query.bindValue(9, ui->dae_ms->date().toString("yyyy-MM-dd"));
        query.bindValue(10, ui->gbx_round->isChecked());
        query.bindValue(11, ui->txt_orga->text());
        query.bindValue(12, ui->sbx_edv->value());
        query.bindValue(13, ui->sbx_volu->value());
        query.bindValue(14, ui->sbx_ref->value());
        query.bindValue(15, ui->txt_web->text());
        query.bindValue(16, ui->txt_zweck->text());
        query.bindValue(17, ui->dsb_money->value());
        query.bindValue(18, ui->dsb_postmoney->value());
        query.bindValue(19, ui->chk_nostart->isChecked());
        query.bindValue(20, ui->chk_rereg->isChecked());
        query.bindValue(21, ui->chk_postreg->isChecked());
        query.bindValue(22, ui->txt_regtext->toPlainText());
        query.bindValue(23, ui->txt_startproof->toPlainText());
        query.bindValue(24, ui->txt_requirements->toPlainText());
        query.bindValue(25, ui->txt_certificate->toPlainText());
        query.bindValue(26, ui->txt_ref->toPlainText());
        query.bindValue(27, ui->txt_misc->toPlainText());
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
