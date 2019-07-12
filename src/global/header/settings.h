#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class QFont;

class Settings : public QObject {

    Q_OBJECT

public:
    static void saveAllSettings();
    static void loadAllSettings();

    static bool clubFilter;
    static bool usedFilter;
    static bool juryResults;

    static int juryRound;
    static int iconMode;
    static int barCodes;
    static int nameFormat;
    static int dbTyp;

    static QFont barCodeFont;

    static QString creator;
    static QString license;
    static QString version;
    static QString organisation;

    static QString smtpServer;
    static QString smtpUser;
    static QString smtpPass;
    static QString smtpFrom;
    static QString smtpMail;

    static QString toaUser;
    static QString toaPass;

    static void updatePrefs(int dbTyp, QString dbServer, QString dbUser, QString dbPass, QString dbDatabase, QString toaUser, QString toaPass, QString smtpServer, QString smtpUser, QString smtpPass, QString smtpFrom, QString smtpMail, QString creator, QFont barCodeFont, int nameFormat, int barCodes, int iconMode, int juryRound);
    static void updateJuryResults(bool set);
    static void updateFiler(bool clubFilter, bool usedFilter);

};

#endif // SETTINGS_H
