#include "mailthread.h"
#include "export/misc/registrationmatrix.h"
#include "export/misc/timetable.h"
#include "export/participants/registration.h"
#include "export/participants/squad.h"
#include "export/print.h"
#include "export/results/detail.h"
#include "export/results/individual.h"
#include "export/results/round.h"
#include "mailsender.h"
#include "model/entity/event.h"
#include "src/global/header/_global.h"
#include "src/global/header/settings.h"
#include <QFile>
#include <QSqlQuery>
#include <QVariant>

MailThread::MailThread(Event *event)
    : QThread()
{
    this->event = event;
}

void MailThread::run()
{
    Print::setCoverID(0);
    Print::setDetailInfo(detailinfo);
    createPDF();
    sendMails();
    delFiles();
    quit();
}

void MailThread::setVereine(QStringList ver)
{
    vereine = ver;
}

void MailThread::setDetailinfo(int di)
{
    detailinfo = di;
}

void MailThread::setChecked(QList<bool> chk)
{
    checked = chk;
}

void MailThread::setMailVars(QString sub, QString bod)
{
    subject = sub;
    body = bod;
}

void MailThread::createPDF()
{
    for(int i=0;i<checked.size();i++) {
        if (checked.at(i) == true) {
            Print *ausdruck;
            switch (i) {
            case 0: {
                    for (int j=0; j<vereine.size(); j++) {
                        QSqlQuery query;
                        query.prepare("SELECT var_name FROM tfx_vereine WHERE int_vereineid=? LIMIT 1");
                        query.bindValue(0,vereine.at(j));
                        query.exec();
                        query.next();
                        QString send_filename = "Meldeliste_"+ query.value(0).toString().replace(" ","_").replace("ä","ae").replace("ö","oe").replace("ü","ue").replace("ß","ss").replace("Ä","Ae").replace("Ö","Oe").replace("Ü","Ue") + ".pdf";
                        files.append(send_filename);
                        emit(textChanged("PDF-Dateien werden erzeugt...\n" + send_filename));
                        ausdruck = new Registration(this->event);
                        ausdruck->setOutputFileName(send_filename);
                        ausdruck->setVerein(vereine.at(j).toInt());
                        ausdruck->setTypeString("Meldeliste");
                    }
                }; break;
            case 1: {
                    files.append("Meldeliste.pdf");
                    emit(textChanged("PDF-Dateien werden erzeugt...\nMeldeliste.pdf"));
                    ausdruck = new Registration(this->event);
                    ausdruck->setOutputFileName("Meldeliste.pdf");
                    ausdruck->setTypeString("Meldeliste");
                }; break;
            case 2: {
                    files.append("Meldematrix.pdf");
                    emit(textChanged("PDF-Dateien werden erzeugt...\nMeldematrix.pdf"));
                    ausdruck = new RegistrationMatrix(this->event);
                    ausdruck->setOutputFileName("Meldematrix.pdf");
                    ausdruck->setTypeString("Meldematrix");
                }; break;
            case 3: {
                    files.append("Riegenlisten.pdf");
                    emit(textChanged("PDF-Dateien werden erzeugt...\nRiegenlisten.pdf"));
                    ausdruck = new Squad(this->event);
                    ausdruck->setOutputFileName("Riegenlisten.pdf");
                    ausdruck->setTypeString("Riegen");
                }; break;
            case 4: {
                    files.append("Ergebnisse_mit_Details.pdf");
                    emit(textChanged("PDF-Dateien werden erzeugt...\nErgebnisse_mit_Details.pdf"));
                    ausdruck = new Detail(this->event);
                    ausdruck->setOutputFileName("Ergebnisse_mit_Details.pdf");
                    ausdruck->setTypeString("Ergebnisse");
                }; break;
            case 5: {
                    files.append("Ergebnisse_ohne_Details.pdf");
                    emit(textChanged("PDF-Dateien werden erzeugt...\nErgebnisse_ohne_Details.pdf"));
                    ausdruck = new Individual(this->event);
                    ausdruck->setOutputFileName("Ergebnisse_ohne_Details.pdf");
                    ausdruck->setTypeString("Ergebnisse");
                }; break;
            case 6: {
                    for (int j=0; j<vereine.size(); j++) {
                        QSqlQuery query;
                        query.prepare("SELECT var_name FROM tfx_vereine WHERE int_vereineid=? LIMIT 1");
                        query.bindValue(0,vereine.at(j));
                        query.exec();
                        query.next();
                        QString send_filename = "Vereinsergebnisse_mit_Details_"+ query.value(0).toString().replace(" ","_").replace("ä","ae").replace("ö","oe").replace("ü","ue").replace("ß","ss").replace("Ä","Ae").replace("Ö","Oe").replace("Ü","Ue") + ".pdf";
                        files.append(send_filename);
                        emit(textChanged("PDF-Dateien werden erzeugt...\n" + send_filename));
                        ausdruck = new Detail(this->event);
                        ausdruck->setOutputFileName(send_filename);
                        ausdruck->setVerein(vereine.at(j).toInt());
                        ausdruck->setTypeString("Ergebnisse");
                    }
                }; break;
            case 7: {
                    for (int j=0; j<vereine.size(); j++) {
                        QSqlQuery query;
                        query.prepare("SELECT var_name FROM tfx_vereine WHERE int_vereineid=? LIMIT 1");
                        query.bindValue(0,vereine.at(j));
                        query.exec();
                        query.next();
                        QString send_filename = "Vereinsergebnisse_ohne_Details_"+ query.value(0).toString().replace(" ","_").replace("ä","ae").replace("ö","oe").replace("ü","ue").replace("ß","ss").replace("Ä","Ae").replace("Ö","Oe").replace("Ü","Ue") + ".pdf";
                        files.append(send_filename);
                        emit(textChanged("PDF-Dateien werden erzeugt...\n" + send_filename));
                        ausdruck = new Individual(this->event);
                        ausdruck->setOutputFileName(send_filename);
                        ausdruck->setVerein(vereine.at(j).toInt());
                        ausdruck->setTypeString("Ergebnisse");
                    }
                }; break;
            case 8: {
                    files.append("Gesamtergebnisliste.pdf");
                    emit(textChanged("PDF-Dateien werden erzeugt...\nGesamtergebnisliste.pdf"));
                    ausdruck = new Round(this->event);
                    ausdruck->setOutputFileName("Gesamtergebnisliste.pdf");
                    ausdruck->setTypeString("Ergebnisse");
                }; break;
            case 9: {
                    files.append("Zeitplan.pdf");
                    emit(textChanged("PDF-Dateien werden erzeugt...\nZeitplan.pdf"));
                    ausdruck = new Timetable(this->event);
                    ausdruck->setOutputFileName("Zeitplan.pdf");
                    ausdruck->setTypeString("Zeitplan");
                }; break;
            }
            ausdruck->setOutputType(2);
            ausdruck->start();
            ausdruck->wait();
        }
    }
}

void MailThread::sendMails()
{
    for (int i=0;i<vereine.size();i++) {
        QSqlQuery query;
        query.prepare("SELECT tfx_vereine.var_name, var_email, var_vorname || ' ' || var_nachname FROM tfx_vereine INNER JOIN tfx_personen USING (int_personenid) WHERE int_vereineid=? LIMIT 1");
        query.bindValue(0,vereine.at(i));
        query.exec();
        query.next();
        if (query.value(1).toString().length() > 0) {
            emit(textChanged("E-Mails werden versendet...\n" + query.value(0).toString() + " (" + query.value(1).toString() + ")"));
            MailSender mail(Settings::smtpServer, Settings::smtpMail, QStringList(query.value(1).toString()));
            mail.setSubject(subject);
            mail.setFromName(Settings::smtpFrom);
            mail.setBody(body + "\n\n\n\nDiese Email wurde automatisch mit TurnFix generiert.\nCopyright (c) 2008-2010 Christoph Krämer\nhttp://www.turnfix.de");
            mail.setLogin(Settings::smtpUser, Settings::smtpPass);
            QStringList attachments;
            for(int j=0;j<checked.size();j++) {
                if (checked.at(j) == true) {
                    switch(j) {
                        case 0: attachments.append("Meldeliste_"+ query.value(0).toString().replace(" ","_").replace("ä","ae").replace("ö","oe").replace("ü","ue").replace("ß","ss").replace("Ä","Ae").replace("Ö","Oe").replace("Ü","Ue") + ".pdf"); break;
                        case 1: attachments.append("Meldeliste.pdf"); break;
                        case 2: attachments.append("Meldematrix.pdf"); break;
                        case 3: attachments.append("Riegenlisten.pdf"); break;
                        case 4: attachments.append("Ergebnisse_mit_Details.pdf"); break;
                        case 5: attachments.append("Ergebnisse_ohne_Details.pdf"); break;
                        case 6: attachments.append("Vereinsergebnisse_mit_Details_"+ query.value(0).toString().replace(" ","_").replace("ä","ae").replace("ö","oe").replace("ü","ue").replace("ß","ss").replace("Ä","Ae").replace("Ö","Oe").replace("Ü","Ue") + ".pdf"); break;
                        case 7: attachments.append("Vereinsergebnisse_ohne_Details_"+ query.value(0).toString().replace(" ","_").replace("ä","ae").replace("ö","oe").replace("ü","ue").replace("ß","ss").replace("Ä","Ae").replace("Ö","Oe").replace("Ü","Ue") + ".pdf"); break;
                        case 8: attachments.append("Gesamtergebnisliste.pdf"); break;
                        case 9: attachments.append("Zeitplan.pdf"); break;
                    }
                }
            }
            mail.setAttachments(attachments);
            mail.setEncoding(MailSender::Encoding_8bit);
            mail.setISO(MailSender::utf8);
            bool ok = mail.send();
            if(!ok) {
                emit message("Critical", "Fehler beim versenden!", "Email an " + query.value(1).toString() + " konnte nicht gesendet werden! Fehlermeldung: "+mail.lastError());
            }
        } else {
            emit message("Warning", "Keine Emailadresse", "Keine Emailadresse für " + query.value(0).toString() + " vorhanden!");
        }
    }
}

void MailThread::delFiles()
{
    for (int i=0;i<files.size();i++) {
        QFile(files.at(i)).remove();
    }
}
