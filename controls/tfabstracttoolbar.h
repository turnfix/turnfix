#ifndef TFABSTRACTTOOLBAR_H
#define TFABSTRACTTOOLBAR_H

#include <QWidget>

class QVBoxLayout;
class QToolBar;
class QActionGroup;

class TFAbstractToolbar : public QWidget
{
    Q_OBJECT
public:
    explicit TFAbstractToolbar(QWidget *parent = 0);
    void addButton(QAction* action);
    void selectFirstButton();

protected:
    QToolBar *toolbar;

private:
    QVBoxLayout *mainLayout;
    QActionGroup *actionGroup;
    QList<QAction*> actions;

private slots:
    void buttonTriggered(int index);

signals:
    void currentTabChanged(int index);


};

#endif // TFABSTRACTTOOLBAR_H
