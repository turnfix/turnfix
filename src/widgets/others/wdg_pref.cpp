#include "header/wdg_pref.h"
#include "ui_wdg_pref.h"
#include "../../global/header/settings.h"
#include <QSignalMapper>
#include <QToolBar>
#include <QActionGroup>

Preferences_Widget::Preferences_Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Preferences_Widget) {
    ui->setupUi(this);

    ui->cmb_typ->setCurrentIndex(Settings::dbTyp);
    ui->txt_db_user->setText(Settings::dbUser);
    ui->txt_db_pass->setText(Settings::dbPass);
    ui->txt_db_database->setText(Settings::dbDatabase);
    ui->txt_db_server->setText(Settings::dbServer);
    ui->txt_mail_server->setText(Settings::smtpServer);
    ui->txt_mail_user->setText(Settings::smtpUser);
    ui->txt_mail_pass->setText(Settings::smtpPass);
    ui->txt_mail_email->setText(Settings::smtpMail);
    ui->txt_mail_from->setText(Settings::smtpFrom);
    ui->txt_toa_user->setText(Settings::toaUser);
    ui->txt_toa_pass->setText(Settings::toaPass);
    ui->txt_by->setText(Settings::creator);
    ui->chk_icon->setChecked(!(bool)Settings::iconMode);
    ui->chk_barcode->setChecked(!(bool)Settings::barCodes);
    ui->cmb_font->setCurrentFont(Settings::barCodeFont);
    ui->cmb_nameformat->setCurrentIndex(Settings::nameFormat);
    ui->cmb_jury->setCurrentIndex(Settings::juryRound);
    connect(ui->cmb_typ, SIGNAL(currentIndexChanged(int)), this, SLOT(newDbTyp(int)));
    connect(ui->cmb_typ, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePrefs()));
    connect(ui->txt_db_database, SIGNAL(textChanged(QString)), this, SLOT(updatePrefs()));
    connect(ui->txt_db_user, SIGNAL(textChanged(QString)), this, SLOT(updatePrefs()));
    connect(ui->txt_db_server, SIGNAL(textChanged(QString)), this, SLOT(updatePrefs()));
    connect(ui->txt_db_pass, SIGNAL(textChanged(QString)), this, SLOT(updatePrefs()));
    connect(ui->txt_toa_user, SIGNAL(textChanged(QString)), this, SLOT(updatePrefs()));
    connect(ui->txt_toa_pass, SIGNAL(textChanged(QString)), this, SLOT(updatePrefs()));
    connect(ui->txt_mail_email, SIGNAL(textChanged(QString)), this, SLOT(updatePrefs()));
    connect(ui->txt_mail_from, SIGNAL(textChanged(QString)), this, SLOT(updatePrefs()));
    connect(ui->txt_mail_pass, SIGNAL(textChanged(QString)), this, SLOT(updatePrefs()));
    connect(ui->txt_mail_server, SIGNAL(textChanged(QString)), this, SLOT(updatePrefs()));
    connect(ui->txt_mail_user, SIGNAL(textChanged(QString)), this, SLOT(updatePrefs()));
    connect(ui->txt_by, SIGNAL(textChanged(QString)), this, SLOT(updatePrefs()));
    connect(ui->chk_barcode, SIGNAL(stateChanged(int)), this, SLOT(updatePrefs()));
    connect(ui->chk_icon, SIGNAL(stateChanged(int)), this, SLOT(updatePrefs()));
    connect(ui->cmb_font, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePrefs()));
    connect(ui->cmb_jury, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePrefs()));
    connect(ui->cmb_nameformat, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePrefs()));

    QToolBar *tb = new QToolBar();
    QActionGroup *ag = new QActionGroup(this);
    tb->setAllowedAreas(Qt::LeftToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setOrientation(Qt::Vertical);
    tb->addAction(ui->act_db);
    ag->addAction(ui->act_db);
    tb->addAction(ui->act_mail);
    ag->addAction(ui->act_mail);
    tb->addAction(ui->act_online);
    ag->addAction(ui->act_online);
    tb->addAction(ui->act_misc);
    ag->addAction(ui->act_misc);
    ui->act_db->setChecked(true);
    ui->configSidebar->layout()->addWidget(tb);
    QSignalMapper *mapper = new QSignalMapper(this);
    mapper->setMapping(ui->act_db, 0);
    mapper->setMapping(ui->act_mail, 1);
    mapper->setMapping(ui->act_online, 2);
    mapper->setMapping(ui->act_misc, 3);
    connect(ui->act_db, SIGNAL(triggered()), mapper, SLOT(map()));
    connect(ui->act_mail, SIGNAL(triggered()), mapper, SLOT(map()));
    connect(ui->act_online, SIGNAL(triggered()), mapper, SLOT(map()));
    connect(ui->act_misc, SIGNAL(triggered()), mapper, SLOT(map()));
    connect(mapper, SIGNAL(mapped(int)), ui->configStack, SLOT(setCurrentIndex(int)));
}

Preferences_Widget::~Preferences_Widget() {
    delete ui;
}

void Preferences_Widget::updatePrefs() {
    Settings::updatePrefs(ui->cmb_typ->currentIndex(), ui->txt_db_server->text(),
                          ui->txt_db_user->text(), ui->txt_db_pass->text(),
                          ui->txt_db_database->text(), ui->txt_toa_user->text(),
                          ui->txt_toa_pass->text(), ui->txt_mail_server->text(),
                          ui->txt_mail_user->text(), ui->txt_db_pass->text(),
                          ui->txt_mail_from->text(), ui->txt_mail_email->text(),
                          ui->txt_by->text(), ui->cmb_font->currentFont(),
                          ui->cmb_nameformat->currentIndex(), (int)!ui->chk_barcode->isChecked(),
                          (int)!ui->chk_icon->isChecked(), ui->cmb_jury->currentIndex());
}

void Preferences_Widget::newDbTyp(int v) {
    emit dbTypChanged(v);
}
