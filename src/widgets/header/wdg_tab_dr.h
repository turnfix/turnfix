#ifndef WDG_TAB_DR_H
#define WDG_TAB_DR_H
#include <QWidget>
#include <QPrinter>
#include "ui_wdg_tab_dr.h"

class Print;
class Event;
class QPrintPreviewDialog;

class Tab_DR : public QWidget, public Ui::Tab_DR_Ui {
    Q_OBJECT

public:
    Tab_DR(QWidget* parent = nullptr);
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
    Event *event;
    void createCSV(int mode);
    Print *ausdruck;
    QPrintPreviewDialog *dialog;
};
#endif
