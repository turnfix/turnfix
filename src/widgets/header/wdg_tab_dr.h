#ifndef WDG_TAB_DR_H
#define WDG_TAB_DR_H
#include <QWidget>
#include "ui_wdg_tab_dr.h"

class Drucken;
class QPrintPreviewDialog;

class Tab_DR : public QWidget, public Ui::Tab_DR_Ui {
    Q_OBJECT;
public:
    Tab_DR(QWidget* parent=0);
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
    void createCSV(int mode);
    Drucken *ausdruck;
    QPrintPreviewDialog *dialog;
};
#endif
