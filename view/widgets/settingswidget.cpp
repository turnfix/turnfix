#include <QFileDialog>
#include <QToolBar>
#include "settingswidget.h"
#include "ui_settingswidget.h"
#include "model/settings/tfsettings.h"
#include "view/wizards/postgressetupwizard.h"
#include "view/dialogs/checkdatabasedialog.h"
#include "model/database/db.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    ui->toolbarWidget->addButton(ui->databaseAction);
    ui->toolbarWidget->addButton(ui->emailAction);
    ui->toolbarWidget->addButton(ui->onlineAccountAction);
    ui->toolbarWidget->addButton(ui->miscAction);

    ui->toolbarWidget->selectFirstButton();

    connect(ui->toolbarWidget, SIGNAL(currentTabChanged(int)), ui->configStack, SLOT(setCurrentIndex(int)));

    //Connect other signal & slots
    connect(ui->databaseFileButton, SIGNAL(clicked()), this, SLOT(openDatabaseFile()));
    connect(ui->databaseCreateButton, SIGNAL(clicked()), this, SLOT(createDatabaseFile()));
    connect(ui->controlButtons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonController(QAbstractButton*)));
    connect(ui->databaseAssistantButton, SIGNAL(clicked()), this, SLOT(setupPostgresDatabase()));

    //Load settings
    this->loadData();
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::buttonController(QAbstractButton *button)
{
    if (button == ui->controlButtons->buttons().at(0))
    {
        this->saveData();
    }
    else
    {
        this->loadData();
    }
}

void SettingsWidget::loadData()
{
    TFSettings *settings = TFSettings::getInstance();

    ui->databaseTypeCombo->setCurrentIndex(settings->getDbTyp());
    ui->databaseUserText->setText(settings->getDbUser());
    ui->databasePasswordText->setText(settings->getDbPass());
    ui->databaseText->setText(settings->getDbDatabase());
    ui->databaseServerText->setText(settings->getDbServer());
    ui->databaseFileText->setText(settings->getDbFilename());
    ui->mailServerText->setText(settings->getMailServer());
    ui->mailUserText->setText(settings->getMailUser());
    ui->mailPasswordText->setText(settings->getMailPass());
    ui->emailText->setText(settings->getMailEmail());
    ui->senderText->setText(settings->getMailFrom());
    ui->onlineUserText->setText(settings->getOnlineUser());
    ui->onlinePasswordText->setText(settings->getOnlinePass());
    ui->creatorText->setText(settings->getCreator());
    ui->iconCheckbox->setChecked(settings->getIconMode());
    ui->barcodeCheckbox->setChecked(settings->getBarcodes());
    ui->fontCombo->setCurrentFont(settings->getBarcodeFont());
    ui->nameFormatCombo->setCurrentIndex(settings->getNameFormat());
    ui->juryCombo->setCurrentIndex(settings->getJuryRound());
}

void SettingsWidget::saveData()
{
    TFSettings *settings = TFSettings::getInstance();

    settings->setDbTyp(ui->databaseTypeCombo->currentIndex());
    settings->setDbUser(ui->databaseUserText->text());
    settings->setDbPass(ui->databasePasswordText->text());
    settings->setDbDatabase(ui->databaseText->text());
    settings->setDbServer(ui->databaseServerText->text());
    settings->setDbFilename(ui->databaseFileText->text());
    settings->setMailServer(ui->mailServerText->text());
    settings->setMailUser(ui->mailUserText->text());
    settings->setMailPass(ui->mailPasswordText->text());
    settings->setMailEmail(ui->emailText->text());
    settings->setMailFrom(ui->senderText->text());
    settings->setOnlineUser(ui->onlineUserText->text());
    settings->setOnlinePass(ui->onlinePasswordText->text());
    settings->setCreator(ui->creatorText->text());
    settings->setIconMode(ui->iconCheckbox->isChecked());
    settings->setBarcodes(ui->barcodeCheckbox->isChecked());
    settings->setBarcodeFont(ui->fontCombo->currentFont());
    settings->setNameFormat(ui->nameFormatCombo->currentIndex());
    settings->setJuryRound(ui->juryCombo->currentIndex());
    settings->saveData();
}

void SettingsWidget::openDatabaseFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::List);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilters(QStringList(tr("TurnFix-Datenbank-Dateien (*.tfdb)")));
    if (dialog.exec())
    {
        if (dialog.selectedFiles().size()>0)
        {
            ui->databaseFileText->setText(dialog.selectedFiles().first());
        }
    }
}

void SettingsWidget::createDatabaseFile()
{
    QString fileName;
    DB *db;
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::List);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilters(QStringList(tr("TurnFix-Datenbank-Dateien (*.tfdb)")));
    if (dialog.exec())
    {
        if (dialog.selectedFiles().size()>0)
        {
            db = DB::getInstance();
            fileName = dialog.selectedFiles().first();

            if (fileName.right(5) != ".tfdb")
                fileName += ".tfdb";

            ui->databaseFileText->setText(fileName);
            this->saveData();
            db->establishConnection();
            CheckDatabaseDialog checker;
            checker.exec();
            db->closeConnection();
        }
    }
}

void SettingsWidget::setupPostgresDatabase()
{
    PostgresSetupWizard wiz;
    if (wiz.exec() == 1)
    {
        this->loadData();
    }
}
