#include "athletedialog.h"
#include "src/global/header/_global.h"
#include "ui_athletedialog.h"
#include <QSqlQuery>

AthleteDialog::AthleteDialog(int tid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AthleteDialog)
{
    tnid = tid;
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    ui->cmb_sex->addItem("weiblich", 0);
    ui->cmb_sex->addItem("männlich", 1);
    ui->cmb_club->addItem("", 0);
    QSqlQuery query;
    query.prepare("SELECT int_vereineid, var_name FROM tfx_vereine ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+" ASC");
    query.exec();
    while (query.next()) {
        ui->cmb_club->addItem(query.value(1).toString(), query.value(0).toInt());
    }

    connect(ui->but_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->chk_dat, SIGNAL(stateChanged(int)), this, SLOT(changeDat()));

    if (tid != 0) {
        QSqlQuery query;
        query.prepare("SELECT var_nachname || ', ' || var_vorname, dat_geburtstag, tfx_vereine.var_name, int_geschlecht, int_startpassnummer, bool_nur_jahr FROM tfx_teilnehmer INNER JOIN tfx_vereine USING (int_vereineid) WHERE int_teilnehmerid=?");
        query.bindValue(0,tid);
        query.exec();
        query.next();
        ui->txt_name->setText(query.value(0).toString());
        ui->dae_year->setDate(QDate().fromString(query.value(1).toString(), "yyyy-MM-dd"));
        ui->cmb_club->setEditText(query.value(2).toString());
        ui->cmb_sex->setCurrentIndex(query.value(3).toInt());
        ui->txt_id->setText(query.value(4).toString());
        ui->chk_dat->setChecked(!query.value(5).toBool());
    }
}

AthleteDialog::~AthleteDialog()
{
    delete ui;
}

void AthleteDialog::changeDat()
{
    if (ui->chk_dat->isChecked()) {
        ui->dae_year->setDisplayFormat("dd.MM.yyyy");
        ui->dae_year->setCalendarPopup(true);
    } else {
        ui->dae_year->setDisplayFormat("yyyy");
        ui->dae_year->setCalendarPopup(false);
    }
}

void AthleteDialog::save()
{
    if (ui->txt_id->text() == "")
        ui->txt_id->setText("0");
    int vid=0;
    QSqlQuery query3;
    query3.prepare("SELECT int_vereineid FROM tfx_vereine WHERE var_name = ?");
    query3.bindValue(0, ui->cmb_club->currentText());
    query3.exec();
    if (_global::querySize(query3) > 0) {
        query3.next();
        vid = query3.value(0).toInt();
    } else {
        QSqlQuery query4;
        query4.prepare("INSERT INTO tfx_vereine (var_name) VALUES (?)");
        query4.bindValue(0, ui->cmb_club->currentText());
        query4.exec();
        if (_global::getDBTyp()==0) {
            QSqlQuery query5("SELECT last_value FROM tfx_vereine_int_vereineid_seq");
            query5.next();
            vid = query5.value(0).toInt();
        } else {
            vid = query4.lastInsertId().toInt();
        }
    }
    QSqlQuery query6;
    if (tnid == 0) {
        query6.prepare("INSERT INTO tfx_teilnehmer (int_vereineid,var_vorname,var_nachname,int_geschlecht,dat_geburtstag,int_startpassnummer,bool_nur_jahr) VALUES (?,?,?,?,?,?,?)");
    } else {
        query6.prepare("UPDATE tfx_teilnehmer SET int_vereineid=?, var_vorname=?, var_nachname=?, int_geschlecht=?, dat_geburtstag=?, int_startpassnummer=?, bool_nur_jahr=? WHERE int_teilnehmerid=?");
        query6.bindValue(7,tnid);
    }
    query6.bindValue(0,vid);
    query6.bindValue(1, _global::nameSplit(ui->txt_name->text()).at(0));
    query6.bindValue(2, _global::nameSplit(ui->txt_name->text()).at(1));
    query6.bindValue(3, ui->cmb_sex->currentIndex());
    query6.bindValue(4, ui->dae_year->date().toString("yyyy-MM-dd"));
    query6.bindValue(5, ui->txt_id->text());
    query6.bindValue(6, !ui->chk_dat->isChecked());
    query6.exec();
    done(1);
}

void AthleteDialog::setVerein(QString verein)
{
    ui->cmb_club->setEditText(verein);
}
