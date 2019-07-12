#include "settingswidget.h"
#include "checkdatabasedialog.h"
#include "model/settings/tfsettings.h"
#include "ui_settingswidget.h"
#include <QFileDialog>
#include <QToolBar>

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    ui->toolbarWidget->addButton(ui->emailAction);
    ui->toolbarWidget->addButton(ui->onlineAccountAction);
    ui->toolbarWidget->addButton(ui->miscAction);

    ui->toolbarWidget->selectFirstButton();

    connect(ui->toolbarWidget, SIGNAL(currentTabChanged(int)), ui->configStack, SLOT(setCurrentIndex(int)));

    //Connect other signal & slots

    connect(ui->controlButtons,
            SIGNAL(clicked(QAbstractButton *)),
            this,
            SLOT(buttonController(QAbstractButton *)));

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
