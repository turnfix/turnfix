#include "logindialog.h"
#include "checkdatabasedialog.h"
#include "eventdialog.h"
#include "model/entity/abstractconnection.h"
#include "model/entity/event.h"
#include "model/entitymanager.h"
#include "model/view/connectionmodel.h"
#include "model/view/eventmodel.h"
#include "postgressetupwizard.h"
#include "ui_logindialog.h"
#include <QDataWidgetMapper>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>

LoginDialog::LoginDialog(EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
    , connectionModel(new ConnectionModel(em, this))
    , em(em)
    , eventModel(new EventModel(em, this))
{
    ui->setupUi(this);

    ui->selectionBarWidget->addButton(ui->loginAction);
    ui->selectionBarWidget->addButton(ui->settingsAction);
    ui->selectionBarWidget->selectFirstButton();

    ui->eventsTable->setModel(eventModel);
    ui->eventsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->eventsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->eventsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(connectionModel);
    mapper->addMapping(ui->postgresNameText, 0);
    mapper->addMapping(ui->databaseServerText, 1);
    mapper->addMapping(ui->databaseUserText, 2);
    mapper->addMapping(ui->databasePasswordText, 3);
    mapper->addMapping(ui->databaseText, 4);
    mapper->addMapping(ui->sqliteNameText, 0);
    mapper->addMapping(ui->databaseFileText, 1);

    ui->connectionComboBox->setModel(connectionModel);
    ui->connectionListView->setModel(connectionModel);

    auto connectionMenu = new QMenu(ui->addConnectionButton);
    connectionMenu->addAction("SQLite", connectionModel, [this]() {
        connectionModel->addConnection(AbstractConnection::Type::SQLite);
    });
    connectionMenu->addAction("PostgreSQL", connectionModel, [this]() {
        connectionModel->addConnection(AbstractConnection::Type::PostgreSQL);
    });
    ui->addConnectionButton->setMenu(connectionMenu);

    connect(ui->selectionBarWidget,
            &TFSelectionBar::currentTabChanged,
            ui->mainStack,
            &QStackedWidget::setCurrentIndex);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginDialog::doLogin);
    connect(ui->checkDatabaseButton, &QPushButton::clicked, this, &LoginDialog::checkDatabase);
    connect(ui->eventsTable, &QTableView::doubleClicked, this, &LoginDialog::selectCurrentEvent);
    connect(ui->createEventButton, &QPushButton::clicked, this, &LoginDialog::createEvent);
    connect(ui->selectEventButton, &QPushButton::clicked, this, &LoginDialog::selectCurrentEvent);
    connect(ui->removeConnectionButton, &QPushButton::clicked, connectionModel, [this]() {
        connectionModel->removeConnection(ui->connectionListView->currentIndex());
    });
    connect(ui->connectionListView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &LoginDialog::connectionListSelectionChanged);
    connect(ui->databaseFileButton, &QToolButton::clicked, this, &LoginDialog::openDatabaseFile);
    connect(ui->databaseCreateButton, &QToolButton::clicked, this, &LoginDialog::createDatabaseFile);
    connect(ui->databaseAssistantButton,
            &QPushButton::clicked,
            this,
            &LoginDialog::setupPostgresDatabase);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

Event *LoginDialog::selectedEvent()
{
    return m_selectedEvent;
}

void LoginDialog::doLogin()
{
    bool connectionEstablished;

    ui->eventsWidget->setEnabled(false);
    eventModel->clear();

    if (ui->connectionComboBox->currentIndex()==-1)
       return;

    AbstractConnection *connection = connectionModel->connectionAt(
        ui->connectionComboBox->currentIndex());

    connect(connection, &AbstractConnection::errorOccured, this, &LoginDialog::errorHandler);

    connectionEstablished = connection->connect("main");

    if (connectionEstablished) {
        em->setConnectionName("main");
        ui->eventsWidget->setEnabled(true);
        eventModel->getEvents();
    }

    disconnect(connection, &AbstractConnection::errorOccured, this, &LoginDialog::errorHandler);
}

void LoginDialog::errorHandler(const QString &errorText)
{
    QMessageBox::critical(this,tr("Datenbankfehler"),tr("Es ist ein Fehler bei der Kommunikation mit der Datenbank aufgetreten.\n\nFehlerbeschreibung:\n%1").arg(errorText));
}

void LoginDialog::checkDatabase()
{
    auto index = ui->connectionListView->currentIndex();
    if (!index.isValid()) {
        return;
    }

    AbstractConnection *connection = connectionModel->connectionAt(index);

    CheckDatabaseDialog dialog(connection);
    dialog.exec();
}

void LoginDialog::createEvent()
{
    auto *nwkw = new EventDialog(new Event, em, this);
    if (nwkw->exec() == 1) {
        eventModel->getEvents();
    }
}

void LoginDialog::selectCurrentEvent()
{
    if (!ui->eventsTable->currentIndex().isValid())
        return;

    int row = ui->eventsTable->currentIndex().row();
    m_selectedEvent = eventModel->getEventFromRow(row);

    done(1);
}

void LoginDialog::openDatabaseFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::List);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilters(QStringList(tr("TurnFix-Datenbank-Dateien (*.tfdb)")));
    if (dialog.exec()) {
        if (dialog.selectedFiles().size() > 0) {
            connectionModel->setData(connectionModel->index(mapper->currentIndex(), 1),
                                     dialog.selectedFiles().first());
        }
    }
}

void LoginDialog::createDatabaseFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::List);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilters(QStringList(tr("TurnFix-Datenbank-Dateien (*.tfdb)")));
    if (dialog.exec()) {
        if (dialog.selectedFiles().size() > 0) {
            connectionModel->setData(connectionModel->index(mapper->currentIndex(), 1),
                                     dialog.selectedFiles().first());
            //            db = DB::getInstance();
            //            fileName = dialog.selectedFiles().first();

            //            if (fileName.right(5) != ".tfdb")
            //                fileName += ".tfdb";

            //            // ui->databaseFileText->setText(fileName);
            //            this->saveData();
            //            db->establishConnection();
            //            CheckDatabaseDialog checker;
            //            checker.exec();
            //            db->closeConnection();
        }
    }
}

void LoginDialog::setupPostgresDatabase()
{
    PostgresSetupWizard wiz;
    if (wiz.exec() == 1) {
        // this->loadData();
    }
}

void LoginDialog::connectionListSelectionChanged(const QItemSelection &selected,
                                                 const QItemSelection &)
{
    if (selected.indexes().length() == 0) {
        ui->connectionStackedWidget->setCurrentIndex(0);
        return;
    }

    auto index = selected.indexes().at(0);
    auto *connection = connectionModel->connectionAt(index);
    QString className = connection->metaObject()->className();

    mapper->setCurrentModelIndex(index);

    if (className == "SQLiteConnection") {
        ui->connectionStackedWidget->setCurrentIndex(1);
    } else if (className == "PostgreSQLConnection") {
        ui->connectionStackedWidget->setCurrentIndex(2);
    } else {
        ui->connectionStackedWidget->setCurrentIndex(0);
    }
}
