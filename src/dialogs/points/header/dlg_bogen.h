#ifndef DLG_BOGEN_H
#define DLG_BOGEN_H
#include <QDialog>
#include "ui_dlg_bogen.h"

class QRiegenTableModel;

class WK_Bogen : public QDialog, public Ui::WK_Bogen_Ui {
    Q_OBJECT;
public:
    WK_Bogen(QWidget* parent=0);
    void init(QString riege, int geraet, bool kuer);
    QRiegenTableModel *pe_model;
private slots:
    void fillPETable();
    void finishEdit();
    void statusChange1();
    void saveClose();
    void saveJuryMethod();
private:
    void calc();
    QString riege;
    int geraet;
    bool kuer;
    bool berechnen;
    int versuche;
};

#endif
