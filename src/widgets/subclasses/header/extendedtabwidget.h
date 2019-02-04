#ifndef EXTENDEDTABWIDGET_H
#define EXTENDEDTABWIDGET_H

#include <QTabWidget>

class ExtendedTabWidget : public QTabWidget {

    Q_OBJECT
public:
    explicit ExtendedTabWidget(QWidget *parent = 0);

protected:
    void resizeEvent ( QResizeEvent * e );

};

#endif // EXTENDEDTABWIDGET_H
