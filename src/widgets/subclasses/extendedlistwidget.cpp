#include "header/extendedlistwidget.h"

ExtendedListWidget::ExtendedListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

void ExtendedListWidget::dropEvent(QDropEvent *event) {
    QListWidget::dropEvent(event);
    emit itemDropped();
}
