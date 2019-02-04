#ifndef WDG_TAB_SR_H
#define WDG_TAB_SR_H

#include <QWidget>

namespace Ui {
    class Tab_SR;
}

class Tab_SR : public QWidget {
    Q_OBJECT

public:
    explicit Tab_SR(QWidget *parent = 0);
    ~Tab_SR();
    void setRgDis(QString riege, QString dis);
public slots:
    void init();
    QStringList getClubs();
private:
    Ui::Tab_SR *ui;
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

#endif // WDG_TAB_SR_H
