#ifndef WIN_EINGABE_H
#define WIN_EINGABE_H
#include <QMainWindow>
#include "ui_win_eingabe.h"

class Display_Window;
class QRiegenTableModel;
class QTimer;

class Eingabe_Tool : public QMainWindow, public Ui::Eingabe_Tool_Ui {
    Q_OBJECT;
public:
    Eingabe_Tool();
    QRiegenTableModel *pe_model;
protected:
    Display_Window *dis;
    QButtonGroup *bg;
    QButtonGroup *bg2;
    void closeEvent(QCloseEvent*);
    QList<int> availDisziplinen;
    QStringList availRiegen;
    QTimer *time;
protected slots:
    void init();
    void cotestantChange(bool last=false);
    void red();
    void green();
    void startClock();
    void stopClock();
    void resetClock();
    void updateDis();
    void hideClock();
    void updateClock(QString text);
    void showHideDisplay();
    QString getScore(bool last=false);

    void fillRiege();
    void fillDis();
    void fillOrder();
    void fillTable();
    void back1();
    void end();

    void finishEdit();
    void calc();

    void showStanding(bool showit);

    void etClubChange(int idx);
    void etUpdateTime(QTime time);
    void etStartStop();
    void etSetTime();
};

class FlowLayout : public QLayout {
 public:
     FlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
     FlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
     ~FlowLayout();

     void addItem(QLayoutItem *item);
     int horizontalSpacing() const;
     int verticalSpacing() const;
     Qt::Orientations expandingDirections() const;
     bool hasHeightForWidth() const;
     int heightForWidth(int) const;
     int count() const;
     QLayoutItem *itemAt(int index) const;
     QSize minimumSize() const;
     void setGeometry(const QRect &rect);
     QSize sizeHint() const;
     QLayoutItem *takeAt(int index);

 private:
     int doLayout(const QRect &rect, bool testOnly) const;
     int smartSpacing(QStyle::PixelMetric pm) const;

     QList<QLayoutItem *> itemList;
     int m_hSpace;
     int m_vSpace;
 };
#endif
