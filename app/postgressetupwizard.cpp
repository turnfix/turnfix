#include "postgressetupwizard.h"
#include "checkdatabasedialog.h"
#include "dbinitializerpostgres.h"
#include "model/settings/tfsettings.h"
#include "ui_postgressetupwizard.h"
#include <QAbstractButton>
#include <QMessageBox>

PostgresSetupWizard::PostgresSetupWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::PostgresSetupWizard)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(prePageAction(int)));
}

PostgresSetupWizard::~PostgresSetupWizard()
{
    delete ui;
}

void PostgresSetupWizard::prePageAction(int pageId)
{
    DBInitializerPostgres *initializer;

    if (pageId == 4)
    {
        this->button(QWizard::FinishButton)->setEnabled(false);
        this->button(QWizard::CancelButton)->setEnabled(false);
        this->button(QWizard::BackButton)->setEnabled(false);
        initializer = new DBInitializerPostgres();
        initializer->setData(ui->userText->text(),ui->passwordText->text(),
                             ui->turnfixUserText->text(), ui->turnfixPassText->text(),
                             ui->databaseNameText->text(),ui->serverText->text());
        connect(initializer,SIGNAL(statusUpdate(int,bool,QString)),this,SLOT(updateStatus(int,bool,QString)));
        initializer->start();
    }
}

void PostgresSetupWizard::updateStatus(int stage, bool successful, QString message)
{
    QPixmap pixmap;
    TFSettings *settings;

    if (successful)
    {
        pixmap = QPixmap(":/icons/tick.png");
        if (stage == 3)
        {
            settings = TFSettings::getInstance();
            settings->setDbTyp(0);
            //settings->setDbServer(ui->serverText->text());
            //settings->setDbUser(ui->turnfixUserText->text());
            //settings->setDbPass(ui->turnfixPassText->text());
            //settings->setDbDatabase(ui->databaseNameText->text());
            settings->saveData();

            //CheckDatabaseDialog dialog;
            //dialog.exec();

            this->button(QWizard::FinishButton)->setEnabled(true);
        }
    }
    else
    {
        pixmap = QPixmap(":/icons/cross.png");
        this->button(QWizard::CancelButton)->setEnabled(true);
        this->button(QWizard::BackButton)->setEnabled(true);
    }
    switch (stage)
    {
    case 0: ui->connectionStatus->setPixmap(pixmap); break;
    case 1: ui->createUserStatus->setPixmap(pixmap); break;
    case 2: ui->createDatabaseStatus->setPixmap(pixmap); break;
    case 3: ui->checkLoginStatus->setPixmap(pixmap); break;
    }
    if (message != "")
        QMessageBox::warning(this,"Hinweis",message);
}
