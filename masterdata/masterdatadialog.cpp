#include "masterdatadialog.h"
#include "athletedialog.h"
#include "athletemodel.h"
#include "bankaccountdialog.h"
#include "bankaccountmodel.h"
#include "clubdialog.h"
#include "clubmodel.h"
#include "countrydialog.h"
#include "countrymodel.h"
#include "disciplinedialog.h"
#include "disciplinegroupdialog.h"
#include "disciplinegroupmodel.h"
#include "disciplinemodel.h"
#include "divisiondialog.h"
#include "divisionmodel.h"
#include "formuladialog.h"
#include "formulamodel.h"
#include "model/entitymanager.h"
#include "model/repository/athleterepository.h"
#include "model/repository/bankaccountrepository.h"
#include "model/repository/clubrepository.h"
#include "model/repository/countryrepository.h"
#include "model/repository/disciplinegrouprepository.h"
#include "model/repository/disciplinerepository.h"
#include "model/repository/divisionrepository.h"
#include "model/repository/formularepository.h"
#include "model/repository/penaltyrepository.h"
#include "model/repository/personrepository.h"
#include "model/repository/regionrepository.h"
#include "model/repository/sportrepository.h"
#include "model/repository/staterepository.h"
#include "model/repository/statusrepository.h"
#include "model/repository/venuerepository.h"
#include "penaltydialog.h"
#include "penaltymodel.h"
#include "persondialog.h"
#include "personmodel.h"
#include "regiondialog.h"
#include "regionmodel.h"
#include "sportdialog.h"
#include "sportmodel.h"
#include "statedialog.h"
#include "statemodel.h"
#include "statusdialog.h"
#include "statusmodel.h"
#include "ui_masterdatadialog.h"
#include "venuedialog.h"
#include "venuemodel.h"
#include <QActionGroup>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QToolBar>

MasterdataDialog::MasterdataDialog(EntityManager *em, QWidget *parent, Type type)
    : QMainWindow(parent)
    , ui(new Ui::MasterdataDialog)
    , m_em(em)
{
    ui->setupUi(this);

    auto tb = new QToolBar();
    tb->setAllowedAreas(Qt::LeftToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setOrientation(Qt::Vertical);
    tb->addAction(ui->act_sportler);
    tb->addAction(ui->act_vereine);
    tb->addAction(ui->act_disziplinen);
    tb->addAction(ui->act_sport);
    tb->addAction(ui->act_bereiche);
    tb->addAction(ui->act_personen);
    tb->addAction(ui->act_status);
    tb->addAction(ui->act_konten);
    tb->addAction(ui->act_orte);
    tb->addAction(ui->act_gaue);
    tb->addAction(ui->act_verbaende);
    tb->addAction(ui->act_countries);
    tb->addAction(ui->act_penalty);
    tb->addAction(ui->act_disgruppe);
    tb->addAction(ui->act_formel);
    ui->sidebar->layout()->addWidget(tb);

    auto ag = new QActionGroup(this);
    ag->addAction(ui->act_sportler);
    ag->addAction(ui->act_vereine);
    ag->addAction(ui->act_disziplinen);
    ag->addAction(ui->act_sport);
    ag->addAction(ui->act_bereiche);
    ag->addAction(ui->act_personen);
    ag->addAction(ui->act_status);
    ag->addAction(ui->act_konten);
    ag->addAction(ui->act_orte);
    ag->addAction(ui->act_gaue);
    ag->addAction(ui->act_verbaende);
    ag->addAction(ui->act_countries);
    ag->addAction(ui->act_penalty);
    ag->addAction(ui->act_disgruppe);
    ag->addAction(ui->act_formel);

    m_sortModel = new QSortFilterProxyModel();
    m_sortModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->db_table->setModel(m_sortModel);

    switch (type) {
    case Type::AthleteData:
        ui->act_sportler->setChecked(true);
        break;
    case Type::ClubData:
        ui->act_vereine->setChecked(true);
        break;
    case Type::DisciplineData:
        ui->act_disziplinen->setChecked(true);
        break;
    case Type::SportData:
        ui->act_sport->setChecked(true);
        break;
    case Type::DivisionData:
        ui->act_bereiche->setChecked(true);
        break;
    case Type::PersonData:
        ui->act_personen->setChecked(true);
        break;
    case Type::StatusData:
        ui->act_status->setChecked(true);
        break;
    case Type::BankAccountData:
        ui->act_konten->setChecked(true);
        break;
    case Type::VenueData:
        ui->act_orte->setChecked(true);
        break;
    case Type::RegionData:
        ui->act_gaue->setChecked(true);
        break;
    case Type::StateData:
        ui->act_verbaende->setChecked(true);
        break;
    case Type::CountryData:
        ui->act_countries->setChecked(true);
        break;
    case Type::PenaltyData:
        ui->act_penalty->setChecked(true);
        break;
    case Type::DisciplineGroupData:
        ui->act_disgruppe->setChecked(true);
        break;
    case Type::FormulaData:
        ui->act_formel->setChecked(true);
        break;
    }

    connect(ui->cmb_filter, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilterColumn(int)));
    connect(ui->txt_filter, SIGNAL(textChanged(QString)), this, SLOT(updateFilterText(QString)));

    connect(ui->act_sportler, &QAction::triggered, this, [this]() {
        updateModel(Type::AthleteData);
    });
    connect(ui->act_vereine, &QAction::triggered, this, [this]() { updateModel(Type::ClubData); });
    connect(ui->act_disziplinen, &QAction::triggered, this, [this]() {
        updateModel(Type::DisciplineData);
    });
    connect(ui->act_sport, &QAction::triggered, this, [this]() { updateModel(Type::SportData); });
    connect(ui->act_bereiche, &QAction::triggered, this, [this]() {
        updateModel(Type::DivisionData);
    });
    connect(ui->act_personen, &QAction::triggered, this, [this]() {
        updateModel(Type::PersonData);
    });
    connect(ui->act_status, &QAction::triggered, this, [this]() { updateModel(Type::StatusData); });
    connect(ui->act_konten, &QAction::triggered, this, [this]() {
        updateModel(Type::BankAccountData);
    });
    connect(ui->act_orte, &QAction::triggered, this, [this]() { updateModel(Type::VenueData); });
    connect(ui->act_gaue, &QAction::triggered, this, [this]() { updateModel(Type::RegionData); });
    connect(ui->act_verbaende, &QAction::triggered, this, [this]() {
        updateModel(Type::StateData);
    });
    connect(ui->act_countries, &QAction::triggered, this, [this]() {
        updateModel(Type::CountryData);
    });
    connect(ui->act_penalty, &QAction::triggered, this, [this]() {
        updateModel(Type::PenaltyData);
    });
    connect(ui->act_disgruppe, &QAction::triggered, this, [this]() {
        updateModel(Type::DisciplineGroupData);
    });
    connect(ui->act_formel, &QAction::triggered, this, [this]() { updateModel(Type::FormulaData); });

    connect(ui->but_add, &QPushButton::clicked, this, &MasterdataDialog::add);
    connect(ui->but_edit, &QPushButton::clicked, this, &MasterdataDialog::edit);
    connect(ui->but_del, &QPushButton::clicked, this, &MasterdataDialog::del);
    connect(ui->db_table, &QTableView::doubleClicked, this, &MasterdataDialog::edit);

    updateModel(type);
}

MasterdataDialog::~MasterdataDialog()
{
    delete ui;
}

void MasterdataDialog::updateModel(Type type)
{
    m_currentType = type;

    QList<int> sizes;
    QList<QHeaderView::ResizeMode> resizeModes;
    QList<int> hiddenColumns;

    switch (type) {
    case DisciplineData: {
        auto disciplineModel = new DisciplineModel(m_em, this);
        disciplineModel->fetchDisciplines();
        m_model = disciplineModel;
        resizeModes << QHeaderView::Stretch << QHeaderView::Fixed;
        sizes << 200 << 90;
        break;
    }
    case SportData: {
        auto sportModel = new SportModel(m_em, this);
        sportModel->fetchSports();
        m_model = sportModel;
        resizeModes << QHeaderView::Stretch;
        sizes << 200;
        break;
    }
    case FormulaData: {
        auto formulaModel = new FormulaModel(m_em, this);
        formulaModel->fetchFormulas();
        m_model = formulaModel;
        resizeModes << QHeaderView::Fixed << QHeaderView::Stretch << QHeaderView::Fixed;
        sizes << 250 << 200 << 60;
        break;
    }
    case VenueData: {
        auto venueModel = new VenueModel(m_em, this);
        venueModel->fetchVenues();
        m_model = venueModel;
        resizeModes << QHeaderView::Stretch << QHeaderView::Fixed << QHeaderView::Stretch;
        sizes << 200 << 60 << 200;
        hiddenColumns << 3;
        break;
    }
    case CountryData: {
        auto countryModel = new CountryModel(m_em, this);
        countryModel->fetchCountries();
        m_model = countryModel;
        resizeModes << QHeaderView::Stretch << QHeaderView::ResizeToContents;
        sizes << 200 << 60;
        break;
    }
    case StateData: {
        auto stateModel = new StateModel(m_em, this);
        stateModel->fetchStates();
        m_model = stateModel;
        resizeModes << QHeaderView::Stretch << QHeaderView::ResizeToContents
                    << QHeaderView::ResizeToContents;
        sizes << 200 << 60 << 200;
        break;
    }
    case ClubData: {
        auto clubModel = new ClubModel(m_em, this);
        clubModel->fetchClubs();
        m_model = clubModel;
        resizeModes << QHeaderView::Stretch << QHeaderView::Stretch << QHeaderView::Stretch;
        sizes << 200 << 200 << 200;
        break;
    }
    case AthleteData: {
        auto athleteModel = new AthleteModel(m_em, this);
        athleteModel->fetchAthletes();
        m_model = athleteModel;
        resizeModes << QHeaderView::Stretch << QHeaderView::Fixed << QHeaderView::Stretch;
        sizes << 200 << 60 << 200;
        break;
    }
    case DivisionData: {
        auto divisionModel = new DivisionModel(m_em, this);
        divisionModel->fetchDivisions();
        m_model = divisionModel;
        resizeModes << QHeaderView::Stretch;
        sizes << 200;
        break;
    }
    case PersonData: {
        auto personModel = new PersonModel(m_em, this);
        personModel->fetchPersons();
        m_model = personModel;
        resizeModes << QHeaderView::Stretch << QHeaderView::Stretch << QHeaderView::Fixed
                    << QHeaderView::Fixed << QHeaderView::Fixed << QHeaderView::ResizeToContents
                    << QHeaderView::Fixed << QHeaderView::ResizeToContents << QHeaderView::Fixed;
        sizes << 200 << 200 << 0 << 0 << 0 << 200 << 0 << 200 << 0;
        hiddenColumns << 2 << 3 << 4 << 6 << 8;
        break;
    }
    case StatusData: {
        auto statusModel = new StatusModel(m_em, this);
        statusModel->fetchStatuses();
        m_model = statusModel;
        resizeModes << QHeaderView::Stretch;
        sizes << 200;
        break;
    }
    case BankAccountData: {
        auto bankAccountModel = new BankAccountModel(m_em, this);
        bankAccountModel->fetchAccounts();
        m_model = bankAccountModel;
        resizeModes << QHeaderView::Stretch << QHeaderView::Stretch << QHeaderView::Stretch;
        sizes << 200 << 200 << 200;
        break;
    }
    case RegionData: {
        auto regionModel = new RegionModel(m_em, this);
        regionModel->fetchRegions();
        m_model = regionModel;
        resizeModes << QHeaderView::Stretch << QHeaderView::ResizeToContents
                    << QHeaderView::ResizeToContents;
        sizes << 200 << 60 << 200;
        break;
    }
    case PenaltyData: {
        auto penaltyModel = new PenaltyModel(m_em, this);
        penaltyModel->fetchPenalties();
        m_model = penaltyModel;
        resizeModes << QHeaderView::Stretch << QHeaderView::ResizeToContents;
        sizes << 200 << 60;
        break;
    }
    case DisciplineGroupData: {
        auto disciplineGroupModel = new DisciplineGroupModel(m_em, this);
        disciplineGroupModel->fetchGroups();
        m_model = disciplineGroupModel;
        resizeModes << QHeaderView::Stretch << QHeaderView::Stretch;
        sizes << 200 << 200;
        break;
    }
    }

    m_sortModel->setSourceModel(m_model);

    for (int i = 0; i < resizeModes.length(); i++) {
        ui->db_table->horizontalHeader()->setSectionResizeMode(i, resizeModes.at(i));
    }

    for (int i = 0; i < sizes.length(); i++) {
        ui->db_table->horizontalHeader()->resizeSection(i, sizes.at(i));
    }

    ui->cmb_filter->clear();
    for (int i = 0; i < m_sortModel->columnCount(); i++) {
        bool hidden = hiddenColumns.contains(i);
        ui->db_table->setColumnHidden(i, hidden);

        if (hidden)
            continue;

        ui->cmb_filter->addItem(m_sortModel->headerData(i, Qt::Horizontal).toString());
    }
    ui->txt_filter->setText("");
}

void MasterdataDialog::add()
{
    switch (m_currentType) {
    case DisciplineData: {
        auto dialog = new DisciplineDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<DisciplineModel *>(m_model)->fetchDisciplines();
        }
        break;
    }
    case SportData: {
        auto dialog = new SportDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<SportModel *>(m_model)->fetchSports();
        }
        break;
    }
    case FormulaData: {
        auto dialog = new FormulaDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<FormulaModel *>(m_model)->fetchFormulas();
        }
        break;
    }
    case VenueData: {
        auto dialog = new VenueDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<VenueModel *>(m_model)->fetchVenues();
        }
        break;
    }
    case CountryData: {
        auto dialog = new CountryDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<CountryModel *>(m_model)->fetchCountries();
        }
        break;
    }
    case StateData: {
        auto dialog = new StateDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<StateModel *>(m_model)->fetchStates();
        }
        break;
    }
    case AthleteData: {
        auto dialog = new AthleteDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<AthleteModel *>(m_model)->fetchAthletes();
        }
        break;
    }
    case ClubData: {
        auto dialog = new ClubDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<ClubModel *>(m_model)->fetchClubs();
        }
        break;
    }
    case DivisionData: {
        auto dialog = new DivisionDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<DivisionModel *>(m_model)->fetchDivisions();
        }
        break;
    }
    case PersonData: {
        auto dialog = new PersonDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<PersonModel *>(m_model)->fetchPersons();
        }
        break;
    }
    case StatusData: {
        auto dialog = new StatusDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<StatusModel *>(m_model)->fetchStatuses();
        }
        break;
    }
    case BankAccountData: {
        auto dialog = new BankAccountDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<BankAccountModel *>(m_model)->fetchAccounts();
        }
        break;
    }
    case RegionData: {
        auto dialog = new RegionDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<RegionModel *>(m_model)->fetchRegions();
        }
        break;
    }
    case PenaltyData: {
        auto dialog = new PenaltyDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<PenaltyModel *>(m_model)->fetchPenalties();
        }
        break;
    }
    case DisciplineGroupData: {
        auto dialog = new DisciplineGroupDialog(nullptr, m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<DisciplineGroupModel *>(m_model)->fetchGroups();
        }
        break;
    }
    }
}

void MasterdataDialog::edit()
{
    QVariant obj = m_sortModel->data(ui->db_table->currentIndex(), Qt::UserRole);

    switch (m_currentType) {
    case DisciplineData: {
        auto dialog = new DisciplineDialog(qvariant_cast<Discipline *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<DisciplineModel *>(m_model)->fetchDisciplines();
        }
        break;
    }
    case SportData: {
        auto dialog = new SportDialog(qvariant_cast<Sport *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<SportModel *>(m_model)->fetchSports();
        }
        break;
    }
    case FormulaData: {
        auto dialog = new FormulaDialog(qvariant_cast<Formula *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<FormulaModel *>(m_model)->fetchFormulas();
        }
        break;
    }
    case VenueData: {
        auto dialog = new VenueDialog(qvariant_cast<Venue *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<VenueModel *>(m_model)->fetchVenues();
        }
        break;
    }
    case CountryData: {
        auto dialog = new CountryDialog(qvariant_cast<Country *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<CountryModel *>(m_model)->fetchCountries();
        }
        break;
    }
    case StateData: {
        auto dialog = new StateDialog(qvariant_cast<State *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<StateModel *>(m_model)->fetchStates();
        }
        break;
    }
    case AthleteData: {
        auto dialog = new AthleteDialog(qvariant_cast<Athlete *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<AthleteModel *>(m_model)->fetchAthletes();
        }
        break;
    }
    case ClubData: {
        auto dialog = new ClubDialog(qvariant_cast<Club *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<ClubModel *>(m_model)->fetchClubs();
        }
        break;
    }
    case DivisionData: {
        auto dialog = new DivisionDialog(qvariant_cast<Division *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<DivisionModel *>(m_model)->fetchDivisions();
        }
        break;
    }
    case PersonData: {
        auto dialog = new PersonDialog(qvariant_cast<Person *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<PersonModel *>(m_model)->fetchPersons();
        }
        break;
    }
    case StatusData: {
        auto dialog = new StatusDialog(qvariant_cast<Status *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<StatusModel *>(m_model)->fetchStatuses();
        }
        break;
    }
    case BankAccountData: {
        auto dialog = new BankAccountDialog(qvariant_cast<BankAccount *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<BankAccountModel *>(m_model)->fetchAccounts();
        }
        break;
    }
    case RegionData: {
        auto dialog = new RegionDialog(qvariant_cast<Region *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<RegionModel *>(m_model)->fetchRegions();
        }
        break;
    }
    case PenaltyData: {
        auto dialog = new PenaltyDialog(qvariant_cast<Penalty *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<PenaltyModel *>(m_model)->fetchPenalties();
        }
        break;
    }
    case DisciplineGroupData: {
        auto dialog = new DisciplineGroupDialog(qvariant_cast<DisciplineGroup *>(obj), m_em, this);
        if (dialog->exec() == 1) {
            dynamic_cast<DisciplineGroupModel *>(m_model)->fetchGroups();
        }
        break;
    }
    }
}

void MasterdataDialog::del()
{
    QVariant obj = m_sortModel->data(ui->db_table->currentIndex(), Qt::UserRole);
    switch (m_currentType) {
    case AthleteData: {
        QMessageBox msg(QMessageBox::Question,
                        "Teilnehmer löschen",
                        "Wollen sie diesen Teilnehmer wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->athleteRepository()->remove(qvariant_cast<Athlete *>(obj))) {
                static_cast<AthleteModel *>(m_model)->fetchAthletes();
            } else {
                QMessageBox::information(this,
                                         "Fehler!",
                                         "Dieser Teilnehmer kann nicht gelöscht werden, da er noch "
                                         "einem Wettkampf zugeordnet ist!",
                                         QMessageBox::Ok);
            }
        }
    }; break;
    case ClubData: {
        QMessageBox msg(QMessageBox::Question,
                        "Verein löschen",
                        "Wollen sie diesen Verein wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->clubRepository()->remove(qvariant_cast<Club *>(obj))) {
                static_cast<ClubModel *>(m_model)->fetchClubs();
            } else {
                QMessageBox::information(this,
                                         "Fehler!",
                                         "Dieser Verein kann nicht gelöscht werden, da er noch "
                                         "einem Teilnehmer zugeordnet ist!",
                                         QMessageBox::Ok);
            }
        }
    }; break;
    case DisciplineData: {
        QMessageBox msg(QMessageBox::Question,
                        "Disziplin löschen",
                        "Wollen sie diese Disziplin wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->disciplineRepository()->remove(qvariant_cast<Discipline *>(obj))) {
                static_cast<DisciplineModel *>(m_model)->fetchDisciplines();
            } else {
                QMessageBox::information(
                    this,
                    "Fehler!",
                    "Diese Disziplin kann nicht gelöscht werden, da sie noch zugeordnet ist!",
                    QMessageBox::Ok);
            }
        }
    }; break;
    case SportData: {
        QMessageBox msg(QMessageBox::Question,
                        tr("Sport löschen"),
                        tr("Wollen sie diesen Sport wirklich löschen?"),
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->sportRepository()->remove(qvariant_cast<Sport *>(obj))) {
                static_cast<SportModel *>(m_model)->fetchSports();
            } else {
                QMessageBox::information(
                    this,
                    "Fehler!",
                    "Dieser Sport kann nicht gelöscht werden, da er noch zugeordnet ist!",
                    QMessageBox::Ok);
            }
        }
    }; break;
    case DivisionData: {
        QMessageBox msg(QMessageBox::Question,
                        "Bereich löschen",
                        "Wollen sie diesen Bereich wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->divisionRepository()->remove(qvariant_cast<Division *>(obj))) {
                static_cast<DivisionModel *>(m_model)->fetchDivisions();
            } else {
                QMessageBox::information(
                    this,
                    "Fehler!",
                    "Dieser Bereich kann nicht gelöscht werden, da er noch zugeordnet ist!",
                    QMessageBox::Ok);
            }
        }
    }; break;
    case PersonData: {
        QMessageBox msg(QMessageBox::Question,
                        "Person löschen",
                        "Wollen sie diese Person wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->personRepository()->remove(qvariant_cast<Person *>(obj))) {
                static_cast<PersonModel *>(m_model)->fetchPersons();
            } else {
                QMessageBox::information(
                    this,
                    "Fehler!",
                    "Dieser Person kann nicht gelöscht werden, da er noch zugeordnet ist!",
                    QMessageBox::Ok);
            }
        }
    }; break;
    case StatusData: {
        QMessageBox msg(QMessageBox::Question,
                        "Status löschen",
                        "Wollen sie diesen Status wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->statusRepository()->remove(qvariant_cast<Status *>(obj))) {
                static_cast<StatusModel *>(m_model)->fetchStatuses();
            } else {
                QMessageBox::information(
                    this,
                    "Fehler!",
                    "Dieser Status kann nicht gelöscht werden, da er noch zugeordnet ist!",
                    QMessageBox::Ok);
            }
        }
    }; break;
    case BankAccountData: {
        QMessageBox msg(QMessageBox::Question,
                        "Konto löschen",
                        "Wollen sie dieses Konto wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->bankAccountRepository()->remove(qvariant_cast<BankAccount *>(obj))) {
                static_cast<BankAccountModel *>(m_model)->fetchAccounts();
            } else {
                QMessageBox::information(
                    this,
                    "Fehler!",
                    "Dieses Konto kann nicht gelöscht werden, da es noch zugeordnet ist!",
                    QMessageBox::Ok);
            }
        }
    }; break;
    case VenueData: {
        QMessageBox msg(QMessageBox::Question,
                        "Wettkampfort löschen",
                        "Wollen sie diesen Wettkampfort wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->venueRepository()->remove(qvariant_cast<Venue *>(obj))) {
                static_cast<VenueModel *>(m_model)->fetchVenues();
            } else {
                QMessageBox::information(
                    this,
                    "Fehler!",
                    "Dieser Wettkampfort kann nicht gelöscht werden, da er noch einem "
                    "Wettkampf zugeordnet ist!",
                    QMessageBox::Ok);
            }
        }
    }; break;
    case RegionData: {
        QMessageBox msg(QMessageBox::Question,
                        "Turnkreis/-gau löschen",
                        "Wollen sie diesen Turnkreis/-gau wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->regionRepository()->remove(qvariant_cast<Region *>(obj))) {
                static_cast<RegionModel *>(m_model)->fetchRegions();
            } else {
                QMessageBox::information(this,
                                         "Fehler!",
                                         "Dieser Turnkreis/-gau kann nicht gelöscht werden, da er "
                                         "noch einem Verein zugeordnet ist!",
                                         QMessageBox::Ok);
            }
        }
    }; break;
    case StateData: {
        QMessageBox msg(QMessageBox::Question,
                        "Landesverband löschen",
                        "Wollen sie diesen Landesverband wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->stateRepository()->remove(qvariant_cast<State *>(obj))) {
                static_cast<StateModel *>(m_model)->fetchStates();
            } else {
                QMessageBox::information(
                    this,
                    "Fehler!",
                    "Dieser Landesverband kann nicht gelöscht werden, da er noch einem "
                    "Turnkreis/-gau zugeordnet ist!",
                    QMessageBox::Ok);
            }
        }
    }; break;
    case CountryData: {
        QMessageBox msg(QMessageBox::Question,
                        "Land löschen",
                        "Wollen sie dieses Land wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->countryRepository()->remove(qvariant_cast<Country *>(obj))) {
                static_cast<CountryModel *>(m_model)->fetchCountries();
            } else {
                QMessageBox::information(this,
                                         "Fehler!",
                                         "Dieses Land kann nicht gelöscht werden, da er noch einem "
                                         "Landesverband zugeordnet ist!",
                                         QMessageBox::Ok);
            }
        }
    }; break;
    case PenaltyData: {
        QMessageBox msg(QMessageBox::Question,
                        "Strafe löschen",
                        "Wollen sie diese Strafe wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->penaltyRepository()->remove(qvariant_cast<Penalty *>(obj))) {
                static_cast<PenaltyModel *>(m_model)->fetchPenalties();
            } else {
                QMessageBox::information(
                    this,
                    "Fehler!",
                    "Diese Strafe kann nicht gelöscht werden, da sie noch zugeordnet ist!",
                    QMessageBox::Ok);
            }
        }
    }; break;
    case DisciplineGroupData: {
        QMessageBox msg(QMessageBox::Question,
                        "Disziplinengruppe löschen",
                        "Wollen sie diese Disziplinengruppe wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->disciplineGroupRepository()->remove(qvariant_cast<DisciplineGroup *>(obj))) {
                static_cast<DisciplineGroupModel *>(m_model)->fetchGroups();
            } else {
                QMessageBox::information(
                    this,
                    "Fehler!",
                    "Diese Gruppe kann nicht gelöscht werden, da sie noch zugeordnet ist!",
                    QMessageBox::Ok);
            }
        }
    }; break;
    case FormulaData: {
        QMessageBox msg(QMessageBox::Question,
                        "Formel löschen",
                        "Wollen sie diese Formel wirklich löschen?",
                        QMessageBox::Ok | QMessageBox::Cancel);
        if (msg.exec() == QMessageBox::Ok) {
            if (m_em->formulaRepository()->remove(qvariant_cast<Formula *>(obj))) {
                static_cast<FormulaModel *>(m_model)->fetchFormulas();
            } else {
                QMessageBox::information(
                    this,
                    "Fehler!",
                    "Diese Formel kann nicht gelöscht werden, da sie noch zugeordnet ist!",
                    QMessageBox::Ok);
            }
        }
    }; break;
    }
}

void MasterdataDialog::updateFilterColumn(int index)
{
    m_sortModel->setFilterKeyColumn(index);
    ui->lbl_rowcount->setText(QString::number(m_sortModel->rowCount()));
}

void MasterdataDialog::updateFilterText(const QString &text)
{
    m_sortModel->setFilterRegExp(text);
    ui->lbl_rowcount->setText(QString::number(m_sortModel->rowCount()));
}
