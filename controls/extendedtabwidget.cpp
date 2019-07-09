#include "extendedtabwidget.h"
#include <math.h>
#include <QEvent>

ExtendedTabWidget::ExtendedTabWidget(QWidget *parent) : QTabWidget(parent) {

}

void ExtendedTabWidget::resizeEvent(QResizeEvent *) {
    setStyleSheet(QString("QTabBar::tab { width: %1px; } ").arg(floor(size().width()/count())));
}
