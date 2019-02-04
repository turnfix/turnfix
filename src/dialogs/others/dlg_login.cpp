#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDate>
#include <QStandardItemModel>
#include <QWizard>
#include <QFileDialog>
#include "header/dlg_login.h"
#include "../wk/header/dlg_event.h"
#include "../../global/header/_global.h"
#include "../../global/header/settings.h"
#include "../../dialogs/others/header/dlg_db_check_dialog.h"
#include "../../wizards/header/wzd_database.h"
#include "../../dialogs/wk/header/dlg_delete.h"

Login_Dialog::Login_Dialog(QWidget* parent) : QDialog(parent) {
    Settings::loadAllSettings();
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    txt_username->setText(Settings::dbUser);
    txt_password->setText(Settings::dbPass);
    sta_login->setCurrentIndex(Settings::dbTyp);
    model = new QStandardItemModel();
    model->setColumnCount(4);
    tbl_events->setModel(model);
    QHeaderView::ResizeMode resizeMode[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed};
    int resize[] = {85,500,200};
    QString headers[4] = {"Datum","Name","Ort"};
    for (int i=0;i<3;i++) {
        model->setHeaderData(i, Qt::Horizontal, headers[i]);
        tbl_events->horizontalHeader()->setResizeMode(i, resizeMode[i]);
        tbl_events->horizontalHeader()->resizeSection(i, resize[i]);
    }
    tbl_events->hideColumn(3);

    connect(but_login, SIGNAL(clicked()), this, SLOT(connectdb()));
    connect(but_event, SIGNAL(clicked()), this, SLOT(selectEvent()));
    connect(tbl_events, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectEvent()));
    connect(but_delete,SIGNAL(clicked()),this, SLOT(deleteEvent()));
    connect(but_checkdb, SIGNAL(clicked()), this, SLOT(checkDB()));
    connect(prefTab, SIGNAL(dbTypChanged(int)), sta_login, SLOT(setCurrentIndex(int)));
}

void Login_Dialog::connectdb() {
    int typ = Settings::dbTyp;
    _global::setDBTyp(typ);
    QSqlDatabase db;
    if(typ == 1) {
        QString fileName;
        if (chk_newdb->isChecked()) {
            fileName = QFileDialog::getSaveFileName(this,tr("Datenbank anlegen"), "", tr("TurnFix-Datenbanken (*.tfdb)"));
            if (fileName != "" && fileName.right(5) != ".tfdb") fileName += ".tfdb";
        } else {
            fileName = QFileDialog::getOpenFileName(this,tr("Datenbank öffnen"), "", tr("TurnFix-Datenbanken (*.tfdb)"));
        }
        if (fileName == "") return;
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(fileName);
    } else if (typ == 0) {
        db = QSqlDatabase::addDatabase("QPSQL");
        db.setHostName(Settings::dbServer);
        db.setDatabaseName(Settings::dbDatabase);
        db.setUserName(txt_username->text());
        db.setPassword(txt_password->text());
    }
    model->removeRows(0,model->rowCount());
    if (db.open()) {
        if (typ == 1) {
            QSqlQuery("PRAGMA foreign_keys=true");
        }
        if (Settings::version != "1.1.2" || typ==1) {
            if (checkDB()) {
                Settings::version = "1.1.2";
            } else {
                return;
            }
        }
        gbx_events->setEnabled(true);
        updateList();
    } else {
        db.close();
        QMessageBox msg(QMessageBox::Warning, tr("Datenbankfehler!"), tr("Es konnte keine Verbindung zur Datenbank hergestellt werden. Eventuell wurde die Datenbank noch nicht eingerichtet. Wollen Sie den Installations-Assistenten starten?"),QMessageBox::Yes | QMessageBox::No);
        int ret = msg.exec();
        if (ret == QMessageBox::Yes) {
            DatabaseWizard wizard;
            wizard.exec();
        }
        gbx_events->setEnabled(false);
    }
}

void Login_Dialog::updateList() {
    model->removeRows(0,model->rowCount());
    model->setItem(0,0,new QStandardItem(tr("neue Veranstaltung erstellen...")));
    tbl_events->setSpan(0,0,1,3);
    model->setItem(0,3,new QStandardItem("0"));
    QSqlQuery query("SELECT int_veranstaltungenid, tfx_veranstaltungen.var_name, dat_von, tfx_wettkampforte.var_name, tfx_wettkampforte.var_ort FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) ORDER BY dat_von DESC");
    int i=1;
    while (query.next()) {
        model->setItem(i,0,new QStandardItem(QDate().fromString(query.value(2).toString(),"yyyy-MM-dd").toString("dd.MM.yyyy")));
        model->setItem(i,1,new QStandardItem(query.value(1).toString()));
        model->setItem(i,2,new QStandardItem(query.value(3).toString() + " " + query.value(4).toString()));
        model->setItem(i,3,new QStandardItem(query.value(0).toString()));
        i++;
    }
}

void Login_Dialog::selectEvent() {
    if (QVariant(model->data(model->index(tbl_events->currentIndex().row(),3))).toInt() == 0) {
        Event_Dialog *nwkw = new Event_Dialog();
        if (nwkw->exec() == 1) {
            _global::setWkVars(nwkw->getWknr(),0,0);
            done(1);
            close();
        }
    } else {
        _global::setWkVars(QVariant(model->data(model->index(tbl_events->currentIndex().row(),3))).toInt(),0,0);
        done(1);
        close();
    }
}

void Login_Dialog::autoLogin() {
    QSqlDatabase db = QSqlDatabase::database();
    txt_username->setText(db.userName());
    txt_password->setText(db.password());
    gbx_events->setEnabled(true);
    updateList();
}

void Login_Dialog::deleteEvent() {
    Delete_Dialog *dialog = new Delete_Dialog();
    if (dialog->exec()==1) {
        updateList();
    }
}

bool Login_Dialog::checkDB() {
    DB_Check_Dialog *check = new DB_Check_Dialog();
    if (check->exec() == 1) {
        updateList();
        return true;
    }
    return false;
}

void Login_Dialog::closeEvent(QCloseEvent *) {
    Settings::saveAllSettings();
}
