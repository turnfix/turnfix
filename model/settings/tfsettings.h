#ifndef TFSETTINGS_H
#define TFSETTINGS_H

#include <QFont>
#include <QObject>

class TFSettings : public QObject
{
    Q_OBJECT
public:

    bool isVersionUpToDate();
    bool getClubFilter();
    bool getUsedFilter();
    bool getJuryResults();
    bool getIconMode();
    bool getBarcodes();

    int getJuryRound();
    int getNameFormat();
    int getDbTyp();

    QFont getBarcodeFont();

    QString getMailServer();
    QString getMailUser();
    QString getMailPass();
    QString getMailFrom();
    QString getMailEmail();

    QString getOnlineUser();
    QString getOnlinePass();

    QString getVersion();
    QString getCreator();

    void setIconMode(bool iconMode);
    void setBarcodes(bool barcodes);

    void setJuryRound(int juryRound);
    void setNameFormat(int nameFormat);
    void setDbTyp(int dbTyp);

    void setBarcodeFont(QFont barcodeFont);

    void setMailServer(QString smtpServer);
    void setMailUser(QString smtpUser);
    void setMailPass(QString smtpPass);
    void setMailFrom(QString smtpFrom);
    void setMailEmail(QString smtpMail);

    void setOnlineUser(QString toaUser);
    void setOnlinePass(QString toaPass);

    void setCreator(QString creator);

    void saveData();

    static TFSettings* getInstance();
    static void dropInstance();

private:

    bool clubFilter;
    bool usedFilter;
    bool juryResults;
    bool iconMode;
    bool barcodes;

    int juryRound;
    int nameFormat;
    int dbTyp;

    QFont barcodeFont;

    QString smtpServer;
    QString smtpUser;
    QString smtpPass;
    QString smtpFrom;
    QString smtpMail;

    QString toaUser;
    QString toaPass;

    QString version;
    QString creator;

    static TFSettings* instance;

    TFSettings() {}
    TFSettings(const TFSettings &);
    TFSettings& operator=(const TFSettings &);
    void readData();

};

#endif // TFSETTINGS_H
