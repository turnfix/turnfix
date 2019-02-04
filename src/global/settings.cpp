#include <QSettings>
#include <QFont>
#include "header/settings.h"

QString Settings::smtpServer = "";
QString Settings::smtpUser = "";
QString Settings::smtpPass = "";
QString Settings::smtpFrom = "";
QString Settings::smtpMail = "";

QString Settings::toaPass = "";
QString Settings::toaUser = "";
QString Settings::creator = "";
QString Settings::version = "";
QString Settings::license = "";
QString Settings::organisation = "";

QString Settings::dbServer = "";
QString Settings::dbUser = "";
QString Settings::dbPass = "";
QString Settings::dbDatabase = "";

QFont Settings::barCodeFont = QFont();

int Settings::nameFormat = 0;
int Settings::juryRound = 0;
int Settings::iconMode = 0;
int Settings::barCodes = 0;
int Settings::dbTyp = 0;

bool Settings::juryResults = false;
bool Settings::usedFilter = false;
bool Settings::clubFilter = false;

void Settings::loadAllSettings() {
    QSettings settings("turnfix.ini", QSettings::IniFormat);
    settings.setIniCodec("ISO 8859-1");
    QSettings settings2("license.ini", QSettings::IniFormat);
    settings2.setIniCodec("ISO 8859-1");
    smtpServer = settings.value("Mail/SMTPServer").toString();
    smtpUser = settings.value("Mail/SMTPUser").toString();
    smtpPass = settings.value("Mail/SMTPPassword").toString();
    smtpMail = settings.value("Mail/SMTPFromMail").toString();
    smtpFrom = settings.value("Mail/SMTPFrom").toString();
    toaUser = settings.value("TOA/Username").toString();
    toaPass = settings.value("TOA/Password").toString();
    creator = settings.value("Misc/Creator").toString();
    iconMode = settings.value("Misc/Iconmode").toInt();
    barCodes = settings.value("Misc/Barcodes").toInt();
    barCodeFont = QFont(settings.value("Misc/BarcodeFont").toString());
    nameFormat = settings.value("Misc/ListNameFormat").toInt();
    juryRound = settings.value("Misc/JuryRound").toInt();
    dbUser = settings.value("Database/Username").toString();
    dbPass = settings.value("Database/Password").toString();
    dbDatabase = settings.value("Database/Database").toString();
    dbServer = settings.value("Database/Server").toString();
    dbTyp = settings.value("Database/DBTyp").toInt();
    version = settings.value("Misc/Version").toString();
    license = settings2.value("License/Type").toString();
    organisation = settings2.value("License/Organisation").toString();
    juryResults = settings.value("Application/JuryResults").toBool();
    clubFilter = settings.value("Application/ClubFilter").toBool();
    usedFilter = settings.value("Application/UsedFilter").toBool();
}

void Settings::saveAllSettings() {
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
    settings.setValue("Misc/Barcodes",barCodes);
    settings.setValue("Misc/BarcodeFont",barCodeFont.family());
    settings.setValue("Misc/ListNameFormat",nameFormat);
    settings.setValue("Misc/JuryRound",juryRound);
    settings.setValue("Database/DBTyp", dbTyp);
    settings.setValue("Database/Username", dbUser);
    settings.setValue("Database/Password", dbPass);
    settings.setValue("Database/Database", dbDatabase);
    settings.setValue("Database/Server", dbServer);
    settings.setValue("Misc/Version", version);
    settings.setValue("Application/JuryResults", juryResults);
    settings.setValue("Application/ClubFilter", clubFilter);
    settings.setValue("Application/UsedFilter", usedFilter);
}

void Settings::updatePrefs(int i_dbTyp, QString i_dbServer, QString i_dbUser, QString i_dbPass,
                           QString i_dbDatabase, QString i_toaUser, QString i_toaPass,
                           QString i_smtpServer, QString i_smtpUser, QString i_smtpPass,
                           QString i_smtpFrom, QString i_smtpMail, QString i_creator,
                           QFont i_barCodeFont, int i_nameFormat, int i_barCodes, int i_iconMode,
                           int i_juryRound) {
    dbTyp = i_dbTyp;
    dbServer = i_dbServer;
    dbUser = i_dbUser;
    dbPass = i_dbPass;
    dbDatabase = i_dbDatabase;
    toaUser = i_toaUser;
    toaPass = i_toaPass;
    smtpServer = i_smtpServer;
    smtpUser = i_smtpUser;
    smtpPass = i_smtpPass;
    smtpMail = i_smtpMail;
    smtpFrom = i_smtpFrom;
    creator = i_creator;
    barCodeFont = i_barCodeFont;
    nameFormat = i_nameFormat;
    juryRound = i_juryRound;
    barCodes = i_barCodes;
    iconMode = i_iconMode;
}

void Settings::updateJuryResults(bool set) {
    juryResults = set;
}

void Settings::updateDatabase(QString i_dbServer, QString i_dbUser, QString i_dbPass, QString i_dbDatabase) {
    dbServer = i_dbServer;
    dbUser = i_dbUser;
    dbPass = i_dbPass;
    dbDatabase = i_dbDatabase;
}

void Settings::updateFiler(bool i_clubFilter, bool i_usedFilter) {
    clubFilter = i_clubFilter;
    usedFilter = i_usedFilter;
}
