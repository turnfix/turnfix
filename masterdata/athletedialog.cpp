#include "athletedialog.h"
#include "model/entity/athlete.h"
#include "model/entity/club.h"
#include "model/view/clubmodel.h"
#include "ui_athletedialog.h"
#include <QSqlQuery>

AthleteDialog::AthleteDialog(Athlete *athlete, EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AthleteDialog)
    , m_em(em)
    , m_athlete(athlete)
    , m_clubModel(new ClubModel(em))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    ui->cmb_sex->addItem(tr("weiblich"), Athlete::Gender::Female);
    ui->cmb_sex->addItem("männlich", Athlete::Gender::Male);
    ui->cmb_club->addItem("", 0);

    m_clubModel->fetchClubs();
    ui->cmb_club->setModel(m_clubModel);

    connect(ui->but_save, &QPushButton::clicked, this, &AthleteDialog::save);
    connect(ui->chk_dat, &QCheckBox::stateChanged, this, &AthleteDialog::changeDat);

    if (m_athlete == nullptr) {
        m_athlete = new Athlete();
    }

    ui->txt_name->setText(m_athlete->firstName());
    ui->dae_year->setDate(m_athlete->dateOfBirth());
    ui->cmb_club->setCurrentIndex(ui->cmb_club->findData(m_athlete->clubId()));
    ui->cmb_sex->setCurrentIndex(m_athlete->gender());
    ui->txt_id->setText(m_athlete->license());
    ui->chk_dat->setChecked(!m_athlete->yearOfBirthOnly());
}

AthleteDialog::AthleteDialog(Athlete *athlete, Club *club, EntityManager *em, QWidget *parent)
    : AthleteDialog(athlete, em, parent)
{
    ui->cmb_club->setEditText(club->name());
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
    //    if (ui->txt_id->text() == "")
    //        ui->txt_id->setText("0");
    //    int vid=0;
    //    QSqlQuery query3;
    //    query3.prepare("SELECT int_vereineid FROM tfx_vereine WHERE var_name = ?");
    //    query3.bindValue(0, ui->cmb_club->currentText());
    //    query3.exec();
    //    if (_global::querySize(query3) > 0) {
    //        query3.next();
    //        vid = query3.value(0).toInt();
    //    } else {
    //        QSqlQuery query4;
    //        query4.prepare("INSERT INTO tfx_vereine (var_name) VALUES (?)");
    //        query4.bindValue(0, ui->cmb_club->currentText());
    //        query4.exec();
    //        if (_global::getDBTyp()==0) {
    //            QSqlQuery query5("SELECT last_value FROM tfx_vereine_int_vereineid_seq");
    //            query5.next();
    //            vid = query5.value(0).toInt();
    //        } else {
    //            vid = query4.lastInsertId().toInt();
    //        }
    //    }
    //    QSqlQuery query6;
    //    if (tnid == 0) {
    //        query6.prepare("INSERT INTO tfx_teilnehmer (int_vereineid,var_vorname,var_nachname,int_geschlecht,dat_geburtstag,int_startpassnummer,bool_nur_jahr) VALUES (?,?,?,?,?,?,?)");
    //    } else {
    //        query6.prepare("UPDATE tfx_teilnehmer SET int_vereineid=?, var_vorname=?, var_nachname=?, int_geschlecht=?, dat_geburtstag=?, int_startpassnummer=?, bool_nur_jahr=? WHERE int_teilnehmerid=?");
    //        query6.bindValue(7,tnid);
    //    }
    //    query6.bindValue(0,vid);
    //    query6.bindValue(1, _global::nameSplit(ui->txt_name->text()).at(0));
    //    query6.bindValue(2, _global::nameSplit(ui->txt_name->text()).at(1));
    //    query6.bindValue(3, ui->cmb_sex->currentIndex());
    //    query6.bindValue(4, ui->dae_year->date().toString("yyyy-MM-dd"));
    //    query6.bindValue(5, ui->txt_id->text());
    //    query6.bindValue(6, !ui->chk_dat->isChecked());
    //    query6.exec();
    done(1);
}
