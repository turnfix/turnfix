#include<QToolBar>
#include "tftoolbarwidget.h"

TFToolbarWidget::TFToolbarWidget(QWidget *parent) :
    TFAbstractToolbar(parent)
{
    toolbar->setAllowedAreas(Qt::LeftToolBarArea);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolbar->setOrientation(Qt::Vertical);
    toolbar->setFixedWidth(75);

    setFixedWidth(75);

    setStyleSheet(" \
            QWidget { \
                    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(57, 57, 57, 255), stop:1 rgba(130, 130, 130, 255)); \
                    border-right:1px solid rgba(57, 57, 57, 255); \
            } \
            QToolBar { \
                    border: 0; \
                    font-weight: bold; \
                    border-right:1px solid rgba(57, 57, 57, 255); \
                    spacing:0; \
            } \
            QToolBar:left { \
                    width:75px; \
                    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(57, 57, 57, 255), stop:1 rgba(130, 130, 130, 255)); \
            } \
            QToolButton { \
                    width:75px; \
                    border:0; \
                    padding:1px; \
                    border-right:1px solid rgba(57, 57, 57, 255); \
                    border-top:1px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(57, 57, 57, 255), stop:1 rgba(130, 130, 130, 255)); \
                    border-bottom:1px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(57, 57, 57, 255), stop:1 rgba(130, 130, 130, 255)); \
                    color:white; \
                    font-weight:bold; \
                    font-size:10px; \
            } \
            QToolButton:checked, QToolButton:selected { \
                color:#000000; \
                background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(196, 196, 196, 255), stop:1 rgba(252, 252, 252, 255)); \
                    border-top:1px solid rgba(57, 57, 57, 255); \
                    border-bottom:1px solid rgba(57, 57, 57, 255); \
            } \
            QToolButton:disabled { \
                    color:#cccccc; \
            } \
    ");
}
