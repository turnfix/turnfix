#include <QSqlQuery>
#include <QTimer>
#include <QResizeEvent>
#include "header/win_display.h"
#include "math.h"
#include <QDebug>

Display_Window::Display_Window() {
    setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    er_model = new QErgebnisTableModel();
    tbl_overview->setModel(er_model);
    time = new QTimer(this);
    lbl_time->hide();
    main->setCurrentIndex(0);
    connect(time, SIGNAL(timeout()), this, SLOT(updateClock()));
}

void Display_Window::updateLast(QString name, QString points) {
    lbl_last->setText(name);
    lbl_score->setText(points);
}

void Display_Window::updateCurrent(QString name, QString vorname, QString verein, QString snr) {
    lbl_current_vorname->setText(vorname);
    lbl_current_nachname->setText(name.toUpper());
    lbl_current_club->setText(verein);
    lbl_current_snr->setText(snr);
}
void Display_Window::updateNext(QString name) {
    lbl_next->setText(name);
}
void Display_Window::updateDis(QString icon, QString name) {
    QPixmap pix(300,300);
    pix.load(icon);
    lbl_icon->setPixmap(pix);
    lbl_icon_2->setPixmap(pix);
    lbl_geraet->setText(name);
}
void Display_Window::updateAmpel(int mode) {
    sta_ampel->setCurrentIndex(mode);
}
void Display_Window::startClock() {
    if (time->isActive()) {
        time->stop();
    }
    secs = 0;
    lbl_time->setText("0:00");
    emit(clock(lbl_time->text()));
    time->start(1000);
    lbl_time->show();
}
void Display_Window::stopClock() {
    if (time->isActive()) {
        time->stop();
    }
    emit(clock(lbl_time->text()));
}
void Display_Window::resetClock() {
    if (time->isActive()) {
        time->stop();
    }
    secs=0;
    lbl_time->setText("0:00");
    emit(clock(lbl_time->text()));
}

void Display_Window::updateClock() {
    secs++;
    QString min;
    QString sec;
    if (secs < 60) {
        min = "0";
        sec = QString().setNum(secs);
        if (sec.length()==1) sec = "0" + sec;
    } else {
        int mins = floor(secs/60);
        min = QString().setNum(mins);
        int calc = secs-mins*60;
        sec = QString().setNum(calc);
        if (sec.length()==1) sec = "0" + sec;
    }
    lbl_time->setText(min + ":" + sec);
    emit(clock(lbl_time->text()));
}

void Display_Window::hideClock() {
    if (time->isActive()) {
        time->stop();
    }
    secs=0;
    lbl_time->setText("");
    lbl_time->hide();
    emit(clock(lbl_time->text()));
}


void Display_Window::resize() {
    double height = (double)this->height()/100;
    sta_ampel->setFixedSize(height*16,height*16);
    sta_ampel_2->setFixedSize(height*40,height*40);
    lbl_icon->setFixedSize(height*16,height*16);
    lbl_icon_2->setFixedSize(height*16,height*16);
    lbl_green->setFixedSize(height*16,height*16);
    lbl_red->setFixedSize(height*16,height*16);
    lbl_green_2->setFixedSize(height*40,height*40);
    lbl_red_2->setFixedSize(height*40,height*40);
    lbl_geraet->setFixedHeight(height*16);
    lbl_time->setFixedSize(height*35,height*16);
    lbl_time_2->setFixedSize(height*42,height*21);
    lbl_geraet->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(0, 10, 154); font-size:"+QString::number(height*12,'f',0)+"px; font-family:Arial; font-weight:bold; padding:1px;");
    lbl_time->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(0, 10, 154); font-size:"+QString::number(height*14,'f',0)+"px; font-family:Arial; font-weight:bold;");
    lbl_time_2->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(0, 10, 154); font-size:"+QString::number(height*19,'f',0)+"px; font-family:Arial; font-weight:bold;");
    lbl_einturnen->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(0, 10, 154); font-size:"+QString::number(height*14,'f',0)+"px; font-family:Arial; font-weight:bold;");
    lbl_verein->setStyleSheet("color: rgb(255, 248, 0); font-size:"+QString::number(height*9,'f',0)+"px; font-family:Arial Narrow; font-weight:bold;");
    lbl_current_nachname->setStyleSheet("color: rgb(255, 255, 255); font-size:"+QString::number(height*14,'f',0)+"px; font-family:Arial Narrow; font-weight:bold;");
    lbl_current_vorname->setStyleSheet("color: rgb(255, 255, 255); font-size:"+QString::number(height*13,'f',0)+"px; font-family:Arial Narrow; font-weight:bold;");
    lbl_current_club->setStyleSheet("color: rgb(255, 248, 0); font-size:"+QString::number(height*6,'f',0)+"px; font-family:Arial Narrow;");
    lbl_next->setStyleSheet("color: rgb(255, 255, 255); font-size:"+QString::number(height*9,'f',0)+"px; font-family:Arial Narrow; font-weight:bold;");
    lbl_last->setStyleSheet("color: rgb(255, 255, 0); font-size:"+QString::number(height*9,'f',0)+"px; font-family:Arial Narrow; font-weight:bold;");
    lbl_score->setStyleSheet("color: rgb(255, 255, 0); font-size:"+QString::number(height*14,'f',0)+"px; font-family:Arial; font-weight:bold;");
    lbl_zwischen->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(0, 10, 154); font-size:"+QString::number(height*14,'f',0)+"px; font-family:Arial; font-weight:bold;");
    tbl_overview->setStyleSheet("color:white; font-size:"+QString::number(height*7,'f',0)+"px; border:0");
    tbl_overview->verticalHeader()->setDefaultSectionSize(height*7);
}
