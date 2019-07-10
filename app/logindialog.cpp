#include "logindialog.h"
#include "checkdatabasedialog.h"
#include "connectionmodel.h"
#include "eventdialog.h"
#include "eventtablemodel.h"
#include "model/database/db.h"
#include "model/entitymanager.h"
#include "model/objects/event.h"
#include "model/settings/session.h"
#include "ui_logindialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(EntityManager *em, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    ui->selectionBarWidget->addButton(ui->loginAction);
    ui->selectionBarWidget->addButton(ui->settingsAction);

    ui->selectionBarWidget->selectFirstButton();

    auto connectionModel = new ConnectionModel(em, this);
    ui->connectionComboBox->setModel(connectionModel);

    eventTableModel = new EventTableModel();

    ui->eventsTable->setModel(eventTableModel);

    ui->eventsTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->eventsTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->eventsTable->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);

    connect(ui->selectionBarWidget, SIGNAL(currentTabChanged(int)), ui->mainStack, SLOT(setCurrentIndex(int)));
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(doLogin()));
    connect(ui->checkDatabaseButton, SIGNAL(clicked()), this, SLOT(checkDatabase()));
    connect(DB::getInstance(), SIGNAL(errorOccured(QString)), this, SLOT(errorHandler(QString)));
    connect(ui->eventsTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectCurrentEvent()));
    connect(ui->createEventButton, SIGNAL(clicked()), this, SLOT(createEvent()));
    connect(ui->selectEventButton, SIGNAL(clicked()), this, SLOT(selectCurrentEvent()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::doLogin()
{
    bool connectionEstablished;

    ui->eventsWidget->setEnabled(false);
    eventTableModel->clear();

    connectionEstablished = DB::getInstance()->establishConnection();

    if (connectionEstablished) {
        ui->eventsWidget->setEnabled(true);
        eventTableModel->getEvents();
    }
}

void LoginDialog::errorHandler(QString errorText)
{
    QMessageBox::critical(this,tr("Datenbankfehler"),tr("Es ist ein Fehler bei der Kommunikation mit der Datenbank aufgetreten.\n\nFehlerbeschreibung:\n%1").arg(errorText));
}

void LoginDialog::checkDatabase()
{
    CheckDatabaseDialog dialog;
    dialog.exec();
}

void LoginDialog::createEvent()
{
    EventDialog *nwkw = new EventDialog(new Event);
    if (nwkw->exec() == 1) {
        eventTableModel->getEvents();
    }
}

void LoginDialog::selectCurrentEvent()
{
    int row;
    Event *event;

    if (!ui->eventsTable->currentIndex().isValid())
        return;

    row = ui->eventsTable->currentIndex().row();
    event = eventTableModel->getEventFromRow(row);

    Session::getInstance()->setEvent(event);

    done(1);
}
