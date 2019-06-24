#include <QSqlQuery>
#include <QMessageBox>
#include "header/dlg_db_formel.h"
#include "ui_dlg_db_formel.h"
#include "libs/fparser/fparser.hh"

Formel_Dialog::Formel_Dialog(int fid, QWidget *parent) : QDialog(parent), ui(new Ui::Formel_Dialog) {
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    formelid = fid;
    if (formelid>0) {
        QSqlQuery query;
        query.prepare("SELECT var_name, var_formel FROM tfx_formeln WHERE int_formelid=?");
        query.bindValue(0,formelid);
        query.exec();
        query.next();
        ui->txt_name->setText(query.value(0).toString());
        ui->txt_formel->setText(query.value(1).toString());
    }
    connect(ui->bbx_done,SIGNAL(accepted()),this,SLOT(save()));
    connect(ui->bbx_done,SIGNAL(rejected()),this,SLOT(close()));
    connect(ui->but_add,SIGNAL(clicked()),this,SLOT(addPart()));
    connect(ui->but_check,SIGNAL(clicked()),this,SLOT(checkFormula()));
}

Formel_Dialog::~Formel_Dialog() {
    delete ui;
}

void Formel_Dialog::save() {
    QSqlQuery save;
    if (formelid == 0) {
        save.prepare("INSERT INTO tfx_formeln (var_name,var_formel) VALUES(?,?)");
    } else {
        save.prepare("UPDATE tfx_formeln SET var_name=?, var_formel=? WHERE int_formelid=?");
        save.bindValue(2,formelid);
    }
    save.bindValue(0,ui->txt_name->text());
    save.bindValue(1,ui->txt_formel->text());
    save.exec();
    done(1);
}

void Formel_Dialog::addPart() {
    int start = ui->txt_formel->selectionStart();
    if (start==-1) start = 0;
    ui->txt_formel->insert(ui->cmb_formel->currentText().split(": ").at(1));
    ui->txt_formel->setSelection(ui->txt_formel->text().indexOf("(x",start)+1,1);
    ui->txt_formel->setFocus();
}

void Formel_Dialog::checkFormula() {
    QStringList lst;
    lst << "A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J"<<"K"<<"L"<<"M"<<"N"<<"O"<<"P"<<"Q"<<"R"<<"S"<<"T"<<"U"<<"V"<<"W"<<"X"<<"Y"<<"Z";
    QString vars;
    for (int i=0;i<lst.size();i++) {
        if (ui->txt_formel->text().contains(lst.at(i))) {
            vars += lst.at(i)+",";
        }
    }
    vars = vars.left(vars.length()-1);
    FunctionParser fparser;
    fparser.Parse(ui->txt_formel->text().toStdString(),vars.toStdString());
    QString error = fparser.ErrorMsg();
    if (error == "") {
        error = "Keine Fehler gefunden.";
    }
    QMessageBox::information(this,"Formelüberprüfung",error);
}
