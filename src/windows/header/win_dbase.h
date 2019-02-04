#ifndef WIN_DBASE_H
#define WIN_DBASE_H
#include <QMainWindow>
#include "ui_win_dbase.h"

class QStandardItemModel;

class DBCheck : public QMainWindow, public Ui::DBWindow {
	Q_OBJECT;
	public:
		DBCheck();
	public slots:
                bool startConnect();               
	private slots:
                bool connectdb ();
                void check_versions();
                void backup();
                void updateFunctions();
                void updateTables();
                void restore();
                void install();
                void dropdb();
	protected:
                QStandardItemModel *tb_model;
                QStandardItemModel *ft_model;
};
#endif
