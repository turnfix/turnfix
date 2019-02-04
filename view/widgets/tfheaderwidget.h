#ifndef TFHEADERWIDGET_H
#define TFHEADERWIDGET_H

#include <QWidget>

class QHBoxLayout;
class QLabel;

class TFHeaderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TFHeaderWidget(QWidget *parent = 0);
    void setHeaderText(QString headerText);

private:
    QHBoxLayout *mainLayout;
    QLabel *headerTextLabel;

};

#endif // TFHEADERWIDGET_H
