#ifndef RESULTSSHEETDIALOG_H
#define RESULTSSHEETDIALOG_H
#include <QDialog>

namespace Ui {
class ResultsSheetDialog;
}

class ResultsSheetTableModel;
class Event;

class ResultsSheetDialog : public QDialog
{
    Q_OBJECT

public:
    ResultsSheetDialog(Event *event, QWidget *parent = nullptr);
    void init(QString riege, int geraet, bool kuer);

private slots:
    void fillPETable();
    void finishEdit();
    void statusChange1();
    void saveClose();
    void saveJuryMethod();

private:
    Ui::ResultsSheetDialog *ui;
    ResultsSheetTableModel *pe_model;
    Event *event;
    void calc();
    QString riege;
    int geraet;
    bool kuer;
    bool berechnen;
    int versuche;
};

#endif
