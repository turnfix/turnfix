#ifndef TRD_PROGRESS_H
#define TRD_PROGRESS_H

#include <QThread>
#include <QStringList>
#include <QMessageBox>

class Event;

class Progress_Thread : public QThread {
    Q_OBJECT

public:
    Progress_Thread(Event *Event);
    void setVereine(QStringList vereine);
    void setChecked(QList<bool> chklist);
    void setMailVars(QString subject, QString body);
    void setDetailinfo(int di);

protected:
    void run();
    void createPDF();
    void sendMails();
    void delFiles();

private:
    Event *event;
    QStringList vereine;
    QStringList files;
    QString subject;
    QString body;
    QList<bool> checked;
    int detailinfo;

signals:
    void textChanged(const QString&);
    void message(QString icon,QString titel, QString text);

};

#endif
