#ifndef TFTOOLBARWIDGET_H
#define TFTOOLBARWIDGET_H

#include "tfabstracttoolbar.h"

class QVBoxLayout;
class QToolBar;
class QActionGroup;
class QSignalMapper;

class TFToolbarWidget : public TFAbstractToolbar
{
    Q_OBJECT
public:
    explicit TFToolbarWidget(QWidget *parent = 0);

};

#endif // TFTOOLBARWIDGET_H
