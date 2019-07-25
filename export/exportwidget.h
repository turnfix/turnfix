#ifndef EXPORTWIDGET_H
#define EXPORTWIDGET_H
#include <QPrinter>
#include <QWidget>

namespace Ui {
class ExportWidget;
}

class Print;
class EntityManager;
class Event;
class QPrintPreviewDialog;

class ExportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExportWidget(QWidget *parent = nullptr);
    ~ExportWidget();
    void updateData();

private slots:
    void csvGesamt();
    void csvRunde();
    void startPrint();
    void showDetailinfoDialog();
    void showDisziplinenDialog();
    void showWKDialog();
    void showTNDialog();
    void showVereineDialog();
    void showRiegenDialog();
    void showPrintPreview(QPrinter*);

private:
    Ui::ExportWidget *ui;
    Event *m_event;
    EntityManager *m_em;
    void createCSV(int mode);
    Print *ausdruck;
    QPrintPreviewDialog *dialog;
};
#endif
