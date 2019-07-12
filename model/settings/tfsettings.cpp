#include <QMutex>
#include <QSettings>
#include "tfsettings.h"

TFSettings* TFSettings::instance = 0;
const QString TFVERSION = "1.2.0";

bool TFSettings::isVersionUpToDate()
{
    return this->version == TFVERSION;
}

bool TFSettings::getClubFilter()
{
    return this->clubFilter;
}

bool TFSettings::getUsedFilter()
{
    return this->usedFilter;
}

bool TFSettings::getJuryResults()
{
    return this->juryResults;
}

bool TFSettings::getIconMode()
{
    return this->iconMode;
}

bool TFSettings::getBarcodes()
{
    return this->barcodes;
}

int TFSettings::getJuryRound()
{
    return this->juryRound;
}

int TFSettings::getNameFormat()
{
    return this->nameFormat;
}

int TFSettings::getDbTyp()
{
    return this->dbTyp;
}

QFont TFSettings::getBarcodeFont()
{
    return this->barcodeFont;
}

QString TFSettings::getMailServer()
{
    return this->smtpServer;
}

QString TFSettings::getMailUser()
{
    return this->smtpUser;
}

QString TFSettings::getMailPass()
{
    return this->smtpPass;
}

QString TFSettings::getMailFrom()
{
    return this->smtpFrom;
}

QString TFSettings::getMailEmail()
{
    return this->smtpMail;
}

QString TFSettings::getOnlineUser()
{
    return this->toaUser;
}

QString TFSettings::getOnlinePass()
{
    return this->toaPass;
}

QString TFSettings::getVersion()
{
    return this->version;
}

QString TFSettings::getCreator()
{
    return this->creator;
}

void TFSettings::setIconMode(bool iconMode)
{
    this->iconMode = iconMode;
}

void TFSettings::setBarcodes(bool barcodes)
{
    this->barcodes = barcodes;
}

void TFSettings::setJuryRound(int juryRound)
{
    this->juryRound = juryRound;
}

void TFSettings::setNameFormat(int nameFormat)
{
    this->nameFormat = nameFormat;
}

void TFSettings::setDbTyp(int dbTyp)
{
    this->dbTyp = dbTyp;
}

void TFSettings::setBarcodeFont(QFont barcodeFont)
{
    this->barcodeFont = barcodeFont;
}

void TFSettings::setMailServer(QString smtpServer)
{
    this->smtpServer = smtpServer;
}

void TFSettings::setMailUser(QString smtpUser)
{
    this->smtpUser = smtpUser;
}

void TFSettings::setMailPass(QString smtpPass)
{
    this->smtpPass = smtpPass;
}

void TFSettings::setMailEmail(QString smtpMail)
{
    this->smtpMail = smtpMail;
}

void TFSettings::setMailFrom(QString smtpFrom)
{
    this->smtpFrom = smtpFrom;
}

void TFSettings::setOnlineUser(QString toaUser)
{
    this->toaUser = toaUser;
}

void TFSettings::setOnlinePass(QString toaPass)
{
    this->toaPass = toaPass;
}

void TFSettings::setCreator(QString creator)
{
    this->creator = creator;
}

void TFSettings::readData()
{
    QSettings settings("turnfix.ini", QSettings::IniFormat);
    settings.setIniCodec("ISO 8859-1");
    smtpServer = settings.value("Mail/SMTPServer").toString();
    smtpUser = settings.value("Mail/SMTPUser").toString();
    smtpPass = settings.value("Mail/SMTPPassword").toString();
    smtpMail = settings.value("Mail/SMTPFromMail").toString();
    smtpFrom = settings.value("Mail/SMTPFrom").toString();
    toaUser = settings.value("TOA/Username").toString();
    toaPass = settings.value("TOA/Password").toString();
    creator = settings.value("Misc/Creator").toString();
    iconMode = settings.value("Misc/Iconmode").toBool();
    barcodes = settings.value("Misc/Barcodes").toBool();
    barcodeFont = QFont(settings.value("Misc/BarcodeFont").toString());
    nameFormat = settings.value("Misc/ListNameFormat").toInt();
    juryRound = settings.value("Misc/JuryRound").toInt();
    version = settings.value("Misc/Version").toString();
    juryResults = settings.value("Application/JuryResults").toBool();
    clubFilter = settings.value("Application/ClubFilter").toBool();
    usedFilter = settings.value("Application/UsedFilter").toBool();
}

void TFSettings::saveData()
{
    QSettings settings("turnfix.ini", QSettings::IniFormat);
    settings.setIniCodec("ISO 8859-1");
    settings.setValue("Mail/SMTPServer",smtpServer);
    settings.setValue("Mail/SMTPUser",smtpUser);
    settings.setValue("Mail/SMTPPassword",smtpPass);
    settings.setValue("Mail/SMTPFromMail",smtpMail);
    settings.setValue("Mail/SMTPFrom",smtpFrom);
    settings.setValue("TOA/Username",toaUser);
    settings.setValue("TOA/Password",toaPass);
    settings.setValue("Misc/Creator",creator);
    settings.setValue("Misc/Iconmode",iconMode);
    settings.setValue("Misc/Barcodes",barcodes);
    settings.setValue("Misc/BarcodeFont",barcodeFont.family());
    settings.setValue("Misc/ListNameFormat",nameFormat);
    settings.setValue("Misc/JuryRound",juryRound);
    settings.setValue("Misc/Version", version);
    settings.setValue("Application/JuryResults", juryResults);
    settings.setValue("Application/ClubFilter", clubFilter);
    settings.setValue("Application/UsedFilter", usedFilter);
}

TFSettings* TFSettings::getInstance()
{
    static QMutex mutex;
    if (!instance)
    {
        mutex.lock();

        if (!instance)
        {
            instance = new TFSettings;
            instance->readData();
        }

        mutex.unlock();
    }
    return instance;
}

void TFSettings::dropInstance()
{
    static QMutex mutex;
    mutex.lock();
    delete instance;
    instance = 0;
    mutex.unlock();
}

