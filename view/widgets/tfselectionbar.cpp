#include <QToolBar>
#include "tfselectionbar.h"

TFSelectionBar::TFSelectionBar(QWidget *parent) :
    TFAbstractToolbar(parent)
{
    toolbar->setAllowedAreas(Qt::TopToolBarArea);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->setOrientation(Qt::Horizontal);
    toolbar->setFixedHeight(25);
    toolbar->setIconSize(QSize(21,21));

    setFixedHeight(25);

    setStyleSheet(" \
            QWidget { \
                    background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(57, 57, 57, 255), stop:1 rgba(180, 180, 180, 255)); \
                    border-bottom:1px solid rgba(57, 57, 57, 255); \
            } \
            QToolBar { \
                    border: 0; \
                    font-weight: bold; \
                    border-bottom:1px solid rgba(57, 57, 57, 255); \
                    spacing:0; \
            } \
            QToolBar:top { \
                    height:25px; \
                    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(57, 57, 57, 255), stop:1 rgba(130, 130, 130, 255)); \
            } \
            QToolButton { \
                    height:25px; \
                    border:0; \
                    padding-left:10px; \
                    padding-right:10px; \
                    border-left:1px qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(57, 57, 57, 255), stop:1 rgba(130, 130, 130, 255)); \
                    border-right:1px qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(57, 57, 57, 255), stop:1 rgba(130, 130, 130, 255)); \
                    border-bottom:1px solid rgba(57, 57, 57, 255); \
                    color:white; \
                    font-weight:bold; \
                    font-size:12px; \
            } \
            QToolButton:checked, QToolButton:selected { \
                color:#000000; \
                background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(196, 196, 196, 255), stop:1 rgba(252, 252, 252, 255)); \
                border-left:1px solid rgba(57, 57, 57, 255); \
                border-right:1px solid rgba(57, 57, 57, 255); \
            } \
            QToolButton:disabled { \
                color:#cccccc; \
            } \
    ");
}
