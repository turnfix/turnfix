#include <QSqlQuery>
#include <QInputDialog>
#include <QMessageBox>
#include "win_changer.h"
#include "dlg_tool_login.h"
#include "dlg_select_club.h"

Changer_Window::Changer_Window(int wnr) {
    wknr = wnr;
    setupUi(this);
    showMaximized();
    init();
    connect(act_change_contest, SIGNAL(triggered()), this, SLOT(changeWK()));
    connect(act_exit, SIGNAL(triggered()), this, SLOT(close()));
    connect(but_1, SIGNAL(clicked()), this, SLOT(save1()));
    connect(but_2, SIGNAL(clicked()), this, SLOT(save2()));
    connect(but_3, SIGNAL(clicked()), this, SLOT(save3()));
    connect(but_4, SIGNAL(clicked()), this, SLOT(save4()));
    connect(but_5, SIGNAL(clicked()), this, SLOT(save5()));
    connect(but_6, SIGNAL(clicked()), this, SLOT(save6()));
}

void Changer_Window::init() {
    QSqlQuery query;
    query.prepare("SELECT int_runde, int_hauptwettkampf, bol_rundenwettkampf, tfx_veranstaltungen.var_name, tfx_wettkampforte.var_name || ' ' || var_ort FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) WHERE int_veranstaltungenid=? LIMIT 1");
    query.bindValue(0,wknr);
    query.exec();
    query.next();
    if (query.value(2).toBool()) {
        runde = query.value(0).toInt();
        hwk = query.value(1).toInt();
    } else {
        runde = 1;
        hwk = 0;
    }
    _gfkt.setWkVars(wknr, runde, hwk);
    lbl_1->setText(query.value(3).toString());
    lbl_2->setText(query.value(4).toString());
}

void Changer_Window::save1() {
    bool ok;
    int i = QInputDialog::getInteger(this, tr("Geben Sie die Startnummer ein!"), tr("Startnummer:"), 1, 1, 999999, 1, &ok);
    if (ok) {
        QSqlQuery query2;
        query2.prepare("SELECT int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_startnummer=?");
        query2.bindValue(0, wknr);
        query2.bindValue(1, i);
        query2.exec();
        if (query2.size() > 0) {
            QStringList items;
            QSqlQuery query("SELECT var_name FROM tfx_status WHERE bol_karte=true ORDER BY int_statusid");
            while(query.next()) {
                items.append(query.value(0).toString());
            }
            bool ok2;
            QString item = QInputDialog::getItem(this, tr("Wählen Sie den Status!"),tr("Status:"), items, 0, false, &ok2);
            if (ok2 && !item.isEmpty()) {
                QSqlQuery query3;
                query3.prepare("SELECT int_statusid FROM tfx_status WHERE var_name=? LIMIT 1");
                query3.bindValue(0, item);
                query3.exec();
                query3.next();
                query2.next();
                QSqlQuery query4;
                query4.prepare("UPDATE tfx_wertungen SET int_statusid=? WHERE int_wertungenid=?");
                query4.bindValue(0, query3.value(0).toInt());
                query4.bindValue(1, query2.value(0).toInt());
                query4.exec();
            }
        } else {
            QMessageBox msg(QMessageBox::Information,"Ungültige Startnummer","Diese Startnummer ist ungültig!");
            msg.exec();
        }
    }
}

void Changer_Window::save2() {
    bool ok;
    int i = QInputDialog::getInteger(this, tr("Geben Sie die Riegennummer ein!"), tr("Riegennummer:"), 1, 1, 999999, 1, &ok);
    if (ok) {
        QSqlQuery query2;
        query2.prepare("SELECT int_riegen_x_disziplinenid FROM tfx_riegen_x_disziplinen WHERE int_veranstaltungenid=? AND int_riege=?");
        query2.bindValue(0, wknr);
        query2.bindValue(1, i);
        query2.exec();
        if (query2.size() > 0) {
            QStringList items;
            QSqlQuery query("SELECT var_name FROM tfx_status WHERE bol_bogen=true ORDER BY int_statusid");
            while(query.next()) {
                items.append(query.value(0).toString());
            }
            bool ok2;
            QString item = QInputDialog::getItem(this, tr("Wählen Sie den Status!"),tr("Status:"), items, 0, false, &ok2);
            if (ok2 && !item.isEmpty()) {
                QSqlQuery query3;
                query3.prepare("SELECT int_statusid FROM tfx_status WHERE var_name=? LIMIT 1");
                query3.bindValue(0, item);
                query3.exec();
                query3.next();
                while(query2.next()) {
                    QSqlQuery query4;
                    query4.prepare("UPDATE tfx_riegen_x_disziplinen SET int_statusid=? WHERE int_riegen_x_disziplinenid=?");
                    query4.bindValue(0, query3.value(0).toInt());
                    query4.bindValue(1, query2.value(0).toInt());
                    query4.exec();
                }
            }
        } else {
            QMessageBox msg(QMessageBox::Information,"Ungültige Riegennummer","Diese Riegennummer ist ungültig!");
            msg.exec();
        }
    }
}

void Changer_Window::save3() {
    bool ok;
    int i = QInputDialog::getInteger(this, tr("Geben Sie die Riegennummer ein!"), tr("Riegennummer:"), 1, 1, 999999, 1, &ok);
    if (ok) {
        QSqlQuery query2;
        query2.prepare("SELECT int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_riege=?");
        query2.bindValue(0, wknr);
        query2.bindValue(1, i);
        query2.exec();
        if (query2.size() > 0) {
            QStringList items;
            QSqlQuery query("SELECT var_name FROM tfx_status WHERE bol_karte=true ORDER BY int_statusid");
            while(query.next()) {
                items.append(query.value(0).toString());
            }
            bool ok2;
            QString item = QInputDialog::getItem(this, tr("Wählen Sie den Status!"),tr("Status:"), items, 0, false, &ok2);
            if (ok2 && !item.isEmpty()) {
                QSqlQuery query3;
                query3.prepare("SELECT int_statusid FROM tfx_status WHERE var_name=? LIMIT 1");
                query3.bindValue(0, item);
                query3.exec();
                query3.next();
                while(query2.next()) {
                    QSqlQuery query4;
                    query4.prepare("UPDATE tfx_wertungen SET int_statusid=? WHERE int_wertungenid=?");
                    query4.bindValue(0, query3.value(0).toInt());
                    query4.bindValue(1, query2.value(0).toInt());
                    query4.exec();
                }
            }
        } else {
            QMessageBox msg(QMessageBox::Information,"Ungültige Riegennummer","Diese Riegennummer ist ungültig!");
            msg.exec();
        }
    }
}

void Changer_Window::save4() {
    bool ok;
    int i = QInputDialog::getInteger(this, tr("Geben Sie die Wettkampfnummer ein!"), tr("Wettkampfnummer:"), 1, 1, 999999, 1, &ok);
    if (ok) {
        QSqlQuery query2;
        query2.prepare("SELECT int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND tfx_wettkaempfe.int_nummer=?");
        query2.bindValue(0, wknr);
        query2.bindValue(1, i);
        query2.exec();
        if (query2.size() > 0) {
            QStringList items;
            QSqlQuery query("SELECT var_name FROM tfx_status WHERE bol_karte=true ORDER BY int_statusid");
            while(query.next()) {
                items.append(query.value(0).toString());
            }
            bool ok2;
            QString item = QInputDialog::getItem(this, tr("Wählen Sie den Status!"),tr("Status:"), items, 0, false, &ok2);
            if (ok2 && !item.isEmpty()) {
                QSqlQuery query3;
                query3.prepare("SELECT int_statusid FROM tfx_status WHERE var_name=? LIMIT 1");
                query3.bindValue(0, item);
                query3.exec();
                query3.next();
                while(query2.next()) {
                    QSqlQuery query4;
                    query4.prepare("UPDATE tfx_wertungen SET int_statusid=? WHERE int_wertungenid=?");
                    query4.bindValue(0, query3.value(0).toInt());
                    query4.bindValue(1, query2.value(0).toInt());
                    query4.exec();
                }
            }
        } else {
            QMessageBox msg(QMessageBox::Information,"Ungültige Wettkampfnummer","Diese Wettkampfnummer ist ungültig!");
            msg.exec();
        }
    }
}

void Changer_Window::save5() {
    QStringList vereine;
    Select_Club_Dialog *vn = new Select_Club_Dialog();
    vn->setWkVars(wknr,hwk,_gfkt);
    vn->exec();
    vereine = vn->returnVereine();
    for (int i=0;i<vereine.size();i++) {

        QSqlQuery query2;
        query2.prepare("SELECT int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) WHERE int_veranstaltungenid=? AND int_vereineid=?");
        query2.bindValue(0, wknr);
        query2.bindValue(1, vereine.at(i));
        query2.exec();
        if (query2.size() > 0) {
            QStringList items;
            QSqlQuery query("SELECT var_name FROM tfx_status WHERE bol_karte=true ORDER BY int_statusid");
            while(query.next()) {
                items.append(query.value(0).toString());
            }
            bool ok2;
            QString item = QInputDialog::getItem(this, tr("Wählen Sie den Status!"),tr("Status:"), items, 0, false, &ok2);
            if (ok2 && !item.isEmpty()) {
                QSqlQuery query3;
                query3.prepare("SELECT int_statusid FROM tfx_status WHERE var_name=? LIMIT 1");
                query3.bindValue(0, item);
                query3.exec();
                query3.next();
                while(query2.next()) {
                    QSqlQuery query4;
                    query4.prepare("UPDATE tfx_wertungen SET int_statusid=? WHERE int_wertungenid=?");
                    query4.bindValue(0, query3.value(0).toInt());
                    query4.bindValue(1, query2.value(0).toInt());
                    query4.exec();
                }
            }
        } else {
            QMessageBox msg(QMessageBox::Information,"Ungültige Wettkampfnummer","Diese Wettkampfnummer ist ungültig!");
            msg.exec();
        }
    }
}

void Changer_Window::save6() {
    QSqlQuery query2;
    query2.prepare("SELECT int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=?");
    query2.bindValue(0, wknr);
    query2.exec();
    QStringList items;
    QSqlQuery query("SELECT var_name FROM tfx_status WHERE bol_karte=true ORDER BY int_statusid");
    while(query.next()) {
        items.append(query.value(0).toString());
    }
    bool ok2;
    QString item = QInputDialog::getItem(this, tr("Wählen Sie den Status!"),tr("Status:"), items, 0, false, &ok2);
    if (ok2 && !item.isEmpty()) {
        QSqlQuery query3;
        query3.prepare("SELECT int_statusid FROM tfx_status WHERE var_name=? LIMIT 1");
        query3.bindValue(0, item);
        query3.exec();
        query3.next();
        while(query2.next()) {
            QSqlQuery query4;
            query4.prepare("UPDATE tfx_wertungen SET int_statusid=? WHERE int_wertungenid=?");
            query4.bindValue(0, query3.value(0).toInt());
            query4.bindValue(1, query2.value(0).toInt());
            query4.exec();
        }
    }
}

void Changer_Window::changeWK() {
    hide();
    Tool_Login_Dialog *select = new Tool_Login_Dialog(this);
    select->autoLogin();
    if (select->exec() == 1) {
        wknr = select->getWknr();
        init();
        show();
    }
}
