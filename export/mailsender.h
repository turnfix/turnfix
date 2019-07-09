#ifndef WEB_SENDMAIL_H
#define WEB_SENDMAIL_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QTcpSocket>
#include <QPointer>
#include <QAuthenticator>

class MailSender : public QObject
{
    Q_OBJECT

public:

    enum Encoding {Encoding_7bit, Encoding_8bit, Encoding_base64};
    enum ISO {utf8, iso88591};

    MailSender(const QString &smtpServer, const QString &from, const QStringList &to);
    ~MailSender();
    bool send();
    QString lastError() {return m_lastError;}

    void setSmtpServer (const QString &smtpServer) 	{m_smtpServer = smtpServer;}
    void setPort (int port)					        {m_port = port;}
    void setTimeout (int timeout)				    {m_timeout = timeout;}
    void setLogin (const QString &login, const QString &passwd)		{m_login = login; m_password = passwd;}
    void setAttachments (const QStringList &attachments)	{m_attachments = attachments;}
    void setFrom (const QString &from);
    void setTo (const QStringList &to) 				{m_to = to;}
    void setSubject (const QString &subject) 		{m_subject = subject;}
    void setBody (const QString &body) 				{m_body = body;}
    void setFromName (const QString &fromName)      {m_fromName = fromName;}
    void setISO(ISO iso);
    void setEncoding(Encoding encoding);
    void setProxyAuthenticator(const QAuthenticator &authenticator);

private slots:
    void errorReceived(QAbstractSocket::SocketError socketError);
    void proxyAuthentication(const QNetworkProxy & proxy, QAuthenticator * authenticator);

private:
    void addMimeAttachment(QString *pdata, const QString &filename) const;
    void addMimeBody(QString *pdata) const;
    QString mailData() const;
    QString contentType() const;
    bool read(const QString &waitfor);
    bool sendCommand(const QString &cmd, const QString &waitfor);
    void error(const QString &msg);

    QString	    m_smtpServer;
    int         m_port;
    int         m_timeout;
    QString     m_login;
    QString     m_password;
    QPointer<QTcpSocket>  m_socket;
    QAuthenticator m_authenticator;
    QString     m_lastError;
    QString     m_lastCmd;
    QString     m_lastResponse;
    QString     m_lastMailData;

    QString	    m_from;
    QStringList	m_to;
    QString	    m_subject;
    QString	    m_body;
    QStringList m_attachments;
    QString	    m_fromName;
    QString	    m_encoding;
    QString	    m_charset;
    QString	    m_bodyCodec;
};

#endif
