#ifndef DLG_TN_H
#define DLG_TN_H
#include <QDialog>
#include "ui_dlg_tn.h"

class Tn_Dialog : public QDialog, public Ui::Tn_Dialog_Ui {
	Q_OBJECT;
	public:
                Tn_Dialog(int edit=0, QWidget* parent=0);
            protected slots:
                void initData();
		void save();
		void checkUpdate();
		void checkDisziplinen();
                void checkJg();
                void changeDat();
                void addClub();
                void updateClubs();
	protected:
                int editid;
};
#endif
