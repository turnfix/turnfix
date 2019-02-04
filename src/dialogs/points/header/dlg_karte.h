#ifndef DLG_KARTE_H
#define DLG_KARTE_H
#include <QDialog>
#include "ui_dlg_karte.h"

class QKartenTableModel;

class WK_Karte : public QDialog, public Ui::WK_Karte_Ui {
    Q_OBJECT;
public:
    WK_Karte(QWidget* parent=0);
    void init(int stnr, int wertid, QList< QList<int> > disids);
    QKartenTableModel *pe_model;
private slots:
    void fillPETable();
    void finishEdit();
    void statusChange1();
    void saveClose();
protected:
    QList< QList<int> > disids;
    int stnr;
    int wertid;
};

#endif
