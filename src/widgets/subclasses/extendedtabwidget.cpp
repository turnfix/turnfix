#include <QEvent>
#include <math.h>
#include "header/extendedtabwidget.h"

ExtendedTabWidget::ExtendedTabWidget(QWidget *parent) : QTabWidget(parent) {

}

void ExtendedTabWidget::resizeEvent(QResizeEvent *) {
    setStyleSheet(QString("QTabBar::tab { width: %1px; } ").arg(floor(size().width()/count())));
}
