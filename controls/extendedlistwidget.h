#ifndef EXTENDEDLISTWIDGET_H
#define EXTENDEDLISTWIDGET_H

#include <QListWidget>

class ExtendedListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ExtendedListWidget(QWidget *parent = 0);
    void dropEvent ( QDropEvent * m_event );

signals:
    void itemDropped();
};

#endif // EXTENDEDLISTWIDGET_H
