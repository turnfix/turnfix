#ifndef CAPTUREWIDGET_H
#define CAPTUREWIDGET_H
#include <QWidget>

namespace Ui {
class CaptureWidget;
}

class Event;
class QSqlQueryModel;
class QStandardItemModel;

class CaptureWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CaptureWidget(QWidget *parent = nullptr);
    ~CaptureWidget();

public slots:
    void squadChange(QString squad="");
    void init();

private slots:
    void startBogen();
    void startKarte();
    void startBarcode();

private:
    Ui::CaptureWidget *ui;
    Event *m_event;
    bool eventFilter(QObject *obj, QEvent *ev);
};
#endif
