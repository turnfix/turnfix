#ifndef STARTINGORDERWIDGET_H
#define STARTINGORDERWIDGET_H

#include <QWidget>

namespace Ui {
class StartingOrderWidget;
}

class Event;

class StartingOrderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartingOrderWidget(QWidget *parent = nullptr);
    ~StartingOrderWidget();
    void setRgDis(QString riege, QString dis);

public slots:
    void init();
    QStringList getClubs();

private:
    Event *m_event;
    Ui::StartingOrderWidget *ui;
    QList<int> wertungenInList();
    QList<int> wertungenInList2();
    QStringList clubs;
    bool eventFilter(QObject *, QEvent *);

private slots:
    void squadChange(QString squad="");
    void load();
    void reset();
    void itemDropped();
    void itemRemoved();
    void copy();
};

#endif // STARTINGORDERWIDGET_H
