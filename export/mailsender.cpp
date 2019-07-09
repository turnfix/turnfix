#include "mailsender.h"
#include <time.h>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDataStream>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QHostInfo>
#include <QString>
#include <QTextCodec>
#include <QTextStream>

static QString encodeBase64( QString s ) {
    QByteArray text;
    text.append(s);
    return text.toBase64();
}


static QString timeStamp() {
    QTime now = QTime::currentTime();
    QDate today = QDate::currentDate();
    QStringList monthList = QStringList() <<"Jan"<<"Feb"<<"Mar"<<"Apr"<<"May"<<"Jun"<<"Jul"<<"Aug"<<"Sep"<<"Oct"<<"Nov"<<"Dec";
    QString month = monthList.value(today.month()-1);
    QStringList dayList = QStringList() <<"Mon"<<"Tue"<<"Wed"<<"Thu"<<"Fri"<<"Sat"<<"Sun";
    QString daystr = dayList.value(today.dayOfWeek()-1);
    QString day = QString::number(today.day());
    QString year = QString::number(today.year());
    QString result = QString( "Date: %1 %2 %3 %4 %5 +0100" ).arg(daystr, day, month, year, now.toString("hh:mm:ss"));
    return result;
}

static QString createBoundary() {
    QByteArray hash = QCryptographicHash::hash(QString(QString::number(qrand())).toUtf8(),QCryptographicHash::Md5);
    QString boundary = hash.toHex();
    boundary.truncate(26);
    boundary.prepend("----=_NextPart_");
    return boundary;
}


MailSender::MailSender(const QString &smtpServer, const QString &from, const QStringList &to) {
    setSmtpServer(smtpServer);
    setPort(25);
    setTimeout(3000000);
    setFrom(from);
    setTo(to);
    setSubject("(no subject)");
    setEncoding(Encoding_7bit);
    setISO(utf8);
}

MailSender::~MailSender() {
    if(m_socket) {
        delete m_socket;
    }
}

void MailSender::setFrom(const QString &from) {
    m_from = from;
    m_fromName = from;
}

void MailSender::setISO(ISO iso) {
    switch(iso) {
      case iso88591: m_charset = "iso-8859-1"; m_bodyCodec = "ISO 8859-1"; break;
      case utf8:     m_charset = "utf-8"; m_bodyCodec = "UTF-8"; break;
    }
}

void MailSender::setEncoding(Encoding encoding) {
    switch(encoding) {
      case Encoding_7bit:     m_encoding = "7bit"; break;
      case Encoding_8bit:     m_encoding = "8bit"; break;
      case Encoding_base64:    m_encoding = "base64"; break;
    }
}


void MailSender::addMimeAttachment(QString *pdata, const QString &filename) const {
    QFile file(filename);
    bool ok = file.open(QIODevice::ReadOnly);
    if(!ok) {
        pdata->append("Error attaching file: " + filename + "\r\n");
        return;
    }

    QFileInfo fileinfo(filename);
    pdata->append("Content-Type: application/pdf;\n");
    pdata->append("  name=" + filename + "\n");
    pdata->append("Content-Transfer-Encoding: base64\n");
    pdata->append("Content-Disposition: attachment\n");
    pdata->append("  filename=" + filename + "\n\n");
    QString encodedFile;
    QDataStream in(&file);    
    quint8 a;
    char c;
    QString b;
    while (!in.atEnd()) {
        in >> a;
        c = static_cast<char>(a);
        b.append(c);
    }
    encodedFile = encodeBase64(b);
    for (int i=1;(i+1)*50<encodedFile.length();i++) {
        encodedFile.insert(i*50,"\n");
    }
    pdata->append(encodedFile);
    pdata->append("\r\n\n");
}

void MailSender::addMimeBody(QString *pdata) const {
    pdata->append("Content-Type: text/plain;\n");
    pdata->append("  charset=" + m_charset + "\r\n");
    pdata->append("Content-Transfer-Encoding: " + m_encoding + "\r\n");
    pdata->append("\r\n\n");
    QByteArray encodedBody(m_body.toLocal8Bit());
    QTextCodec *codec = QTextCodec::codecForName(m_bodyCodec.toLocal8Bit());
    pdata->append(codec->toUnicode(encodedBody) + "\r\n");
}

QString MailSender::mailData() const {
    QString data;
    QByteArray hash = QCryptographicHash::hash(QString(QString::number(qrand())).toUtf8(),QCryptographicHash::Md5);
    QString id = hash.toHex();
    data.append("From: =?iso-8859-1?B?" + encodeBase64(m_fromName) + "?= <" + m_from + ">\n");
    if ( m_to.count() > 0 ) {
        data.append("To: ");
        bool first = true;
        foreach (QString val, m_to) {
            if(!first) {
                data.append(",");
            }
            data.append("<" + val + ">");
            first = false;
        }
        data.append("\n");
    }
    data.append("Subject: =?iso-8859-1?B?" + encodeBase64(m_subject) + "?=\n");
    data.append(timeStamp() + "\n");
    data.append("<Message-ID: " + id + "@" + QHostInfo::localHostName() + ">\n");
    data.append("MIME-Version: 1.0\n");

    QString boundary = createBoundary();    
    data.append("Content-Type: multipart/mixed; boundary=\"" + boundary + "\"\n");
    data.append("X-Mailer: TurnFix\r\n");
    data.append("\n");
    data.append("This is a multi-part message in MIME format.\r\n\n");

    data.append("--" + boundary + "\n");

    addMimeBody(&data);

    if ( m_attachments.count() > 0 ) {
        foreach (QString val, m_attachments) {
            data.append("--" + boundary + "\n");
            addMimeAttachment(&data, val);
        }
    }

    data.append("--" + boundary + "--\r\n\n");

    return data; 
}

void MailSender::errorReceived(QAbstractSocket::SocketError socketError) {
    QString msg;

    switch(socketError) {
        case QAbstractSocket::ConnectionRefusedError: msg = "ConnectionRefusedError"; break;
        case QAbstractSocket::RemoteHostClosedError: msg = "RemoteHostClosedError"; break;
        case QAbstractSocket::HostNotFoundError: msg = "HostNotFoundError"; break;
        case QAbstractSocket::SocketAccessError: msg = "SocketAccessError"; break;
        case QAbstractSocket::SocketResourceError: msg = "SocketResourceError"; break;
        case QAbstractSocket::SocketTimeoutError: msg = "SocketTimeoutError"; break;
        case QAbstractSocket::DatagramTooLargeError: msg = "DatagramTooLargeError"; break;
        case QAbstractSocket::NetworkError: msg = "NetworkError"; break;
        case QAbstractSocket::AddressInUseError: msg = "AddressInUseError"; break;
        case QAbstractSocket::SocketAddressNotAvailableError: msg = "SocketAddressNotAvailableError"; break;
        case QAbstractSocket::UnsupportedSocketOperationError: msg = "UnsupportedSocketOperationError"; break;
        case QAbstractSocket::ProxyAuthenticationRequiredError: msg = "ProxyAuthenticationRequiredError"; break;
        default: msg = "Unknown Error";
    }

    error("Socket error [" + msg + "]");
}


bool MailSender::send() {
    m_lastError = "";

    if(m_socket) {
        delete m_socket;
    }

    m_socket = new QTcpSocket(this);
 
    connect( m_socket, SIGNAL( error( QAbstractSocket::SocketError) ), this, SLOT( errorReceived( QAbstractSocket::SocketError ) ) );
    connect( m_socket, SIGNAL( proxyAuthenticationRequired(const QNetworkProxy & , QAuthenticator *) ), this, SLOT(proxyAuthentication(const QNetworkProxy &, QAuthenticator * ) ) );

    bool auth = ! m_login.isEmpty();

    m_socket->connectToHost( m_smtpServer, m_port );

    if( !m_socket->waitForConnected( m_timeout ) ) {
        error("Time out connecting host");
        return false;
    }

    if(!read("220")) {
        return false;
    }

    if ( !sendCommand("EHLO there", "250") ) {
        if ( !sendCommand("HELO there", "250") ) {
            return false;
        }
    }

    if ( auth ) {
        if( !sendCommand("AUTH LOGIN", "334") ) {
            return false;
        }
        if( !sendCommand(encodeBase64(m_login), "334") ) {
            return false;
        }
        if( !sendCommand(encodeBase64(m_password), "235") ) {
            return false;
        }
    }

    if( !sendCommand(QString::fromLatin1("MAIL FROM:<") +m_from + QString::fromLatin1(">"), "250") ) {
        return false;
    }

    for (int i=0; i< m_to.count(); i++) {
        if( !sendCommand(QString::fromLatin1("RCPT TO:<") + m_to.at(i) + QString::fromLatin1(">"), "250") ) {
            return false;
        }
    }

    if( !sendCommand(QString::fromLatin1("DATA"), "354") ) {
        return false;
    }
    if( !sendCommand(mailData() + QString::fromLatin1("\r\n."), "250") ) {
        return false;
    }
    if( !sendCommand(QString::fromLatin1("QUIT"), "221") ) {
        return false;
    }

    m_socket->disconnectFromHost();
    return true;
}

bool MailSender::read(const QString &waitfor) {
    if( ! m_socket->waitForReadyRead( m_timeout ) ) {
        error("Read timeout");
        return false;
    }

    if( !m_socket->canReadLine() ) {
        error("Can't read");
        return false;
    }

    QString responseLine;

    do {
        responseLine = m_socket->readLine();
    } while( m_socket->canReadLine() && responseLine[3] != ' ' );

    m_lastResponse = responseLine;

    QString prefix = responseLine.left(3);
    bool isOk = (prefix == waitfor);
    if(!isOk) {
        error("waiting for " + waitfor + ", received " + prefix + " : "+responseLine);
    }

    return isOk;
}


bool MailSender::sendCommand(const QString &cmd, const QString &waitfor) {
    QTextStream t(m_socket);
    t << cmd + "\r\n";
    t.flush();

    m_lastCmd = cmd;

    return read(waitfor);
}

void MailSender::error(const QString &msg) {
    m_lastError = msg;
}


void MailSender::proxyAuthentication(const QNetworkProxy &, QAuthenticator * authenticator) {
    *authenticator = m_authenticator;
}

void MailSender::setProxyAuthenticator(const QAuthenticator &authenticator) {
    m_authenticator = authenticator;
}
