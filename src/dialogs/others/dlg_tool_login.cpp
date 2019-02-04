#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDate>
#include <QStandardItemModel>
#include <QSettings>
#include "header/dlg_tool_login.h"
#include "../../global/header/_global.h"

Tool_Login_Dialog::Tool_Login_Dialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    model = new QStandardItemModel();
    model->setColumnCount(4);
    tbl_events->setModel(model);
    QHeaderView::ResizeMode resizeMode[] = {QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed};
    int resize[] = {65,500,200,0};
    QString headers[4] = {"Datum","Name","Ort","id"};
    for (int i=0;i<4;i++) {
        model->setHeaderData(i, Qt::Horizontal, headers[i]);
        tbl_events->horizontalHeader()->setResizeMode(i, resizeMode[i]);
        tbl_events->horizontalHeader()->resizeSection(i, resize[i]);
    }
    connect(but_login, SIGNAL(clicked()), this, SLOT(connectdb()));
    connect(but_event, SIGNAL(clicked()), this, SLOT(selectEvent()));
    connect(tbl_events, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectEvent()));
}

void Tool_Login_Dialog::connectdb() {
    QSettings settings("tool.ini", QSettings::IniFormat);
    settings.setIniCodec("ISO 8859-1");
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    if (db.isOpen()) {
        db.close();
    }
    db.setHostName(settings.value("Database/Server").toString());
    db.setDatabaseName(settings.value("Database/Database").toString());
    db.setUserName(txt_username->text());
    db.setPassword(txt_password->text());
    model->removeRows(0,model->rowCount());
    if (db.open()) {
        gbx_events->setEnabled(true);
        updateList();
    } else {
        QMessageBox msg(QMessageBox::Warning, "Datenbankfehler!", "Es konnte keine Verbindung zur Datenbank hergestellt werden. Überprüfen sie ihre Angaben");
        msg.exec();
        gbx_events->setEnabled(false);
    }
}

void Tool_Login_Dialog::updateList() {
    model->removeRows(0,model->rowCount());
    QSqlQuery query("SELECT int_veranstaltungenid, tfx_veranstaltungen.var_name, dat_von, tfx_wettkampforte.var_name, tfx_wettkampforte.var_ort FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) ORDER BY dat_von DESC");
    while (query.next()) {
        model->setItem(query.at(),0,new QStandardItem(QDate().fromString(query.value(2).toString(),"yyyy-MM-dd").toString("dd.MM.yyyy")));
        model->setItem(query.at(),1,new QStandardItem(query.value(1).toString()));
        model->setItem(query.at(),2,new QStandardItem(query.value(3).toString() + " " + query.value(4).toString()));
        model->setItem(query.at(),3,new QStandardItem(query.value(0).toString()));
    }
}

void Tool_Login_Dialog::selectEvent() {
    _global::setWkVars(QVariant(model->data(model->index(tbl_events->currentIndex().row(),3))).toInt(),0,0);
    done(1);
}

void Tool_Login_Dialog::autoLogin() {
    QSqlDatabase db = QSqlDatabase::database();
    txt_username->setText(db.userName());
    txt_password->setText(db.password());
    gbx_events->setEnabled(true);
    updateList();
}
