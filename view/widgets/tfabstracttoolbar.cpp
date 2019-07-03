#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>
#include "tfabstracttoolbar.h"

TFAbstractToolbar::TFAbstractToolbar(QWidget *parent) :
    QWidget(parent)
{
    toolbar = new QToolBar(this);
    toolbar->setMovable(false);
    toolbar->setFloatable(false);

    actionGroup = new QActionGroup(this);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(toolbar);

    setLayout(mainLayout);
}

void TFAbstractToolbar::buttonTriggered(int index)
{
    emit currentTabChanged(index);
}

void TFAbstractToolbar::addButton(QAction *action)
{
    toolbar->addAction(action);
    actionGroup->addAction(action);
    int index = actions.size();
    connect(action, &QAction::triggered,[this, index](){
        buttonTriggered(index);
    });
    actions.append(action);
}

void TFAbstractToolbar::selectFirstButton()
{
    actions.at(0)->setChecked(true);
}


