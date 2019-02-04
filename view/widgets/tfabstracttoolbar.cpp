#include <QAction>
#include <QSignalMapper>
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
    mapper = new QSignalMapper(this);

    connect(mapper, SIGNAL(mapped(int)), this, SLOT(buttonTriggered(int)));

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
    mapper->setMapping(action, actions.size());
    connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
    actions.append(action);
}

void TFAbstractToolbar::selectFirstButton()
{
    actions.at(0)->setChecked(true);
}


