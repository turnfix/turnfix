#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include "tfheaderwidget.h"

TFHeaderWidget::TFHeaderWidget(QWidget *parent) :
    QWidget(parent)
{
    QSpacerItem *topSpacer;
    QFont font;

    mainLayout = new QHBoxLayout(this);

    headerTextLabel = new QLabel(this);
    headerTextLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    font = headerTextLabel->font();
    font.setBold(true);
    headerTextLabel->setFont(font);

    topSpacer = new QSpacerItem(0,0,QSizePolicy::Expanding);

    mainLayout->setContentsMargins(6,12,6,12);
    mainLayout->addWidget(headerTextLabel);
    mainLayout->addSpacerItem(topSpacer);

    headerTextLabel->setText("blubb");

    setLayout(mainLayout);
    setAttribute(Qt::WA_StyledBackground);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    setStyleSheet(" \
                  TFHeaderWidget { \
                          background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(255, 131, 0, 255), stop:1 rgba(255, 223, 100, 255)); \
                          border-bottom:1px solid rgba(57, 57, 57, 255); \
                  } \
                  ");
}

void TFHeaderWidget::setHeaderText(QString headerText)
{
    headerTextLabel->setText(headerText);
}
