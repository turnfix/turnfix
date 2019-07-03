#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QColor>
#include "header/trd_checktablesthread.h"
#include "../global/header/_global.h"

#include <QDebug>

void CheckTable_Thread::run() {

    QStringList tabellenNamen;
    QMap< QString, QList<column> > tabellen;
    QMap< QString, QList<cons> > constraints;
    QMap< QString, QString > creator;
    QMap< QString, QStringList > entries;

    QList<column> tfx_bereiche;
    QList<cons> ctfx_bereiche;
    column col1_1 = {"int_bereicheid","integer",0,"NO","nextval('tfx_bereiche_int_bereicheid_seq'::regclass)",""};
    column col1_2 =  {"var_name",varchar(150,false),150,"YES","",""};
    column col1_3 = {"bol_maennlich","boolean",0,"YES","'true'",""};
    column col1_4 = {"bol_weiblich","boolean",0,"YES","'true'",""};
    tfx_bereiche << col1_1 << col1_2 << col1_3 << col1_4;
    cons con1_1 = {"pkx_bereicheid","PRIMARY KEY","int_bereicheid","","","",""};
    ctfx_bereiche << con1_1;
    tabellenNamen.append("tfx_bereiche");
    tabellen.insert("tfx_bereiche",tfx_bereiche);
    constraints.insert("tfx_bereiche",ctfx_bereiche);
    QStringList lst;
    lst << "INSERT INTO tfx_bereiche (var_name, bol_maennlich, bol_weiblich) VALUES ('männlich', 'true', 'false')";
    lst << "INSERT INTO tfx_bereiche (var_name, bol_maennlich, bol_weiblich) VALUES ('weiblich', 'false', 'true')";
    lst << "INSERT INTO tfx_bereiche (var_name, bol_maennlich, bol_weiblich) VALUES ('mixed', 'true', 'true')";
    entries.insert("tfx_bereiche",lst);
    creator.insert("tfx_bereiche","CREATE TABLE tfx_bereiche ("
                   "int_bereicheid "+primary()+" NOT NULL,"
                   "var_name "+varchar(150)+","
                   "bol_maennlich boolean DEFAULT 'true',"
                   "bol_weiblich boolean DEFAULT 'true',"
                   "CONSTRAINT pkx_bereicheid PRIMARY KEY (int_bereicheid)"
                   ")");

    QList<column> tfx_sport;
    QList<cons> ctfx_sport;
    column col10_1 = {"int_sportid","integer",0,"NO","nextval(',0,_int_sportid_seq'::regclass)",""};
    column col10_2 = {"var_name",varchar(100,false),100,"YES","",""};
    tfx_sport << col10_1 << col10_2;
    cons con10_1 = {"pky_sportid","PRIMARY KEY","int_sportid","","","",""};
    ctfx_sport << con10_1;
    tabellenNamen.append("tfx_sport");
    tabellen.insert("tfx_sport",tfx_sport);
    constraints.insert("tfx_sport",ctfx_sport);
    QStringList lst2;
    lst2 << "INSERT INTO tfx_sport (var_name) VALUES ('Turnen')";
    lst2 << "INSERT INTO tfx_sport (var_name) VALUES ('Leichtathletik')";
    lst2 << "INSERT INTO tfx_sport (var_name) VALUES ('Rope-Skipping')";
    lst2 << "INSERT INTO tfx_sport (var_name) VALUES ('Trampolin')";
    lst2 << "INSERT INTO tfx_sport (var_name) VALUES ('Schwimmen')";
    lst2 << "INSERT INTO tfx_sport (var_name) VALUES ('Gymnastik')";
    lst2 << "INSERT INTO tfx_sport (var_name) VALUES ('TGW')";
    lst2 << "INSERT INTO tfx_sport (var_name) VALUES ('Röhnrad')";
    entries.insert("tfx_sport",lst2);
    creator.insert("tfx_sport","CREATE TABLE tfx_sport ("
                   "int_sportid "+primary()+" NOT NULL,"
                   "var_name "+varchar(100)+","
                   "CONSTRAINT pky_sportid PRIMARY KEY (int_sportid)"
                   ")");


    QList<column> tfx_formeln;
    QList<cons> ctfx_formeln;
    column col33_1 = {"int_formelid","integer",0,"NO","nextval('tfx_formeln_int_formelid_seq'::regclass)",""};
    column col33_2 = {"var_name",varchar(100,false),100,"YES","",""};
    column col33_3 = {"var_formel",varchar(200,false),200,"YES","",""};
    column col33_4 = {"int_typ","smallint",0,"YES","0",""};
    tfx_formeln << col33_1 << col33_2 << col33_3 << col33_4;
    cons con33_1 = {"pky_formeln","PRIMARY KEY","int_formelid","","","",""};
    ctfx_formeln << con33_1;
    tabellenNamen.append("tfx_formeln");
    tabellen.insert("tfx_formeln",tfx_formeln);
    constraints.insert("tfx_formeln",ctfx_formeln);
    creator.insert("tfx_formeln","CREATE TABLE tfx_formeln ("
                   "int_formelid "+primary()+" NOT NULL,"
                   "var_name "+varchar(100)+","
                   "var_formel "+varchar(200)+","
                   "int_typ smallint DEFAULT 0,"
                   "CONSTRAINT pky_formeln PRIMARY KEY (int_formelid)"
                   ")");

    QList<column> tfx_disziplinen;
    QList<cons> ctfx_disziplinen;
    column col2_1 = {"int_disziplinenid","integer",0,"NO","nextval('tfx_disziplinen_int_disziplinenid_seq'::regclass)",""};
    column col2_2 = {"int_sportid","integer",0,"NO","",""};
    column col2_3 = {"var_name",varchar(100,false),100,"YES","",""};
    column col2_4 = {"var_kurz1",varchar(6,false),6,"YES","",""};
    column col2_5 = {"var_kurz2",varchar(20,false),20,"YES","",""};
    column col2_6 = {"var_formel",varchar(300,false),300,"YES","",""};
    column col2_7 = {"var_maske",varchar(10,false),10,"YES","",""};
    column col2_8 = {"int_versuche","integer",0,"YES","1",""};
    column col2_9 = {"var_icon",varchar(50,false),50,"YES","",""};
    column col2_10 = {"var_kuerzel",varchar(50,false),50,"YES","",""};
    column col2_11 = {"int_berechnung","smallint",0,"YES",2,""};
    column col2_12 = {"var_einheit",varchar(5,false),5,"YES","",""};
    column col2_13 = {"bol_bahnen","boolean",0,"YES","'false'",""};
    column col2_14 = {"bol_m","boolean",0,"YES","'true'",""};
    column col2_15 = {"bol_w","boolean",0,"YES","'true'",""};
    column col2_16 = {"int_formelid","integer",0,"YES",""," USING NULL"};
    column col2_17 = {"bol_berechnen","boolean",0,"YES","'true'",""};
    tfx_disziplinen << col2_1 << col2_2 << col2_3 << col2_4 << col2_5 << col2_6 << col2_7 << col2_8 << col2_9 << col2_10 << col2_11 << col2_12 << col2_13 << col2_14 << col2_15 << col2_16 << col2_17;
    cons con2_1 = {"pky_disziplinenid","PRIMARY KEY","int_disziplinenid","","","",""};
    cons con2_2 = {"fky_sportid","FOREIGN KEY","int_sportid","tfx_sport","int_sportid","RESTRICT","RESTRICT"};
    cons con2_3 = {"fky_formelid","FOREIGN KEY","int_formelid","tfx_formeln","int_formelid","RESTRICT","RESTRICT"};
    ctfx_disziplinen << con2_1 << con2_2 << con2_3;
    tabellenNamen.append("tfx_disziplinen");
    tabellen.insert("tfx_disziplinen",tfx_disziplinen);
    constraints.insert("tfx_disziplinen",ctfx_disziplinen);
    QStringList lst4;
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, 'Vollball', 'VBALL', 'Vollball', 'x/2,5', '00.00', 3, ':/icons/ball.png', 'Ctrl+Shift+L', 2, 'm', 'false', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (3, 'Speed 30', 'SPE30', 'Speed 30', 'x/5', '000', 1, ':/icons/rope2.png', 'Ctrl+Shift+F11', 0, NULL, 'false', 'true', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '2.000-m-Lauf', '2000', '2000m', '(((2000/x)-1,784)/0,006)/49', '00:00.00', 1, ':/icons/2000.png', 'Ctrl+Shift+D', 2, '', 'false', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '50-m-Lauf elektrische Zeitm.', '50M', '50m-Lauf', '(((100/(x*1,7)-4,341)/0,00676)/49)', '00.00', 1, ':/icons/50.png', 'Ctrl+Shift+O', 2, 's', 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '2.000-m-Lauf', '2000', '2000m', '(((2000/x)-1,584)/0,00566)/49', '00:00.00', 1, ':/icons/2000.png', 'Ctrl+Shift+D', 2, '', 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, 'Weitsprung / Zone', 'WEZ', 'Weit (Zone)', '2,2*x', '0.00', 3, ':/icons/weitzone.png', 'Ctrl+Shift+G', 2, 'm', 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, 'Kugelstoßen + Medizinball', 'KUGEL', 'Kugel', '1*x', '00.00', 3, ':/icons/kugel.png', 'Ctrl+Shift+J', 2, 'm', 'false', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, 'Schleuderball / Schlagball', 'SBALL', 'Schleuderball', 'x/4,5', '00.00', 3, ':/icons/schleuder.png', 'Ctrl+Shift+K', 2, 'm', 'false', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, 'Weitsprung aus dem Stand', 'WES', 'Weit (Stand)', '4*x', '0.00', 3, ':/icons/weitstand.png', 'Ctrl+Shift+H', 2, 'm', 'false', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, 'Weitsprung aus dem Stand', 'WES', 'Weit (Stand)', '4,4*x', '0.00', 3, ':/icons/weitstand.png', 'Ctrl+Shift+H', 2, 'm', 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, 'Kugelstoßen + Medizinball', 'KUGEL', 'Kugel', '1,2*x', '00.00', 3, ':/icons/kugel.png', 'Ctrl+Shift+J', 2, 'm', 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, 'Schleuderball / Schlagball', 'SBALL', 'Schleuderball', 'x/3,5', '00.00', 3, ':/icons/schleuder.png', 'Ctrl+Shift+K', 2, 'm', 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, 'Vollball', 'VBALL', 'Vollball', 'x/1,5', '00.00', 3, ':/icons/ball.png', 'Ctrl+Shift+L', 2, 'm', 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, 'Weitsprung / Zone', 'WEZ', 'Weit (Zone)', '2*x', '0.00', 3, ':/icons/weitzone.png', 'Ctrl+Shift+G', 2, 'm', 'false', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '25 m Streckentauchen', '25TAU', '25 Tauchen', '12*(((100/(1,2*(4*x+1,5)))-0,3))', '00:00.00', 1, ':/icons/25t.png', 'Ctrl+Shift+F8', 2, NULL, 'false', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '15 m Streckentauchen', '15TAU', '15 Tauchen', '12*(((100/(1,2*(8*x-2,5)))-0,3))', '00:00.00', 1, ':/icons/15t.png', 'Ctrl+Shift+F7', 2, NULL, 'false', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '10 m Streckentauchen', '10TAU', '10 Tauchen', '12*(((100/(1,2*(15*x-16,5)))-0,3))', '00:00.00', 1, ':/icons/10t.png', 'Ctrl+Shift+F6', 2, NULL, 'false', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '10 m Streckentauchen', '10TAU', '10 Tauchen', '12*(((100/(1,2*(15*x-16,5)))-0,3))', '00:00.00', 1, ':/icons/10t.png', 'Ctrl+Shift+F6', 2, NULL, 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '25 m Streckentauchen', '25TAU', '25 Tauchen', '12*(((100/(1,2*(4*x-2,5)))-0,3))', '00:00.00', 1, ':/icons/25t.png', 'Ctrl+Shift+F8', 2, NULL, 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '15 m Streckentauchen', '15TAU', '15 Tauchen', '12*(((100/(1,2*(8*x-10,5)))-0,3))', '00:00.00', 1, ':/icons/15t.png', 'Ctrl+Shift+F7', 2, NULL, 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '75-m-Lauf elektrische Zeitm.', '75M', '75m-Lauf', '(((100/(x*1,3)-4,341)/0,00676)/49)', '00.00', 1, ':/icons/75.png', 'Ctrl+Shift+P', 2, 's', 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '100-m-Lauf elektrische Zeitm.', '100M', '100m-Lauf', '(((100/x)-4,341)/0,00676)/49', '00.00', 1, ':/icons/100.png', 'Ctrl+Shift+F12', 2, 's', 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '50-m-Lauf elektrische Zeitm.', '50M', '50m-Lauf', '(((100/(x*1,6)-4,341)/0,00676)/49)', '00.00', 1, ':/icons/50.png', 'Ctrl+Shift+O', 2, 's', 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '75-m-Lauf Handstoppung', '75M', '75m-Lauf', '(((100/((x+0,24)*1,2)-4,341)/0,00676)/49)', '00.00', 1, ':/icons/75.png', '', 2, 's', 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '75-m-Lauf elektrische Zeitm.', '75M', '75m-Lauf', '(((100/(x*1,2)-4,341)/0,00676)/49)', '00.00', 1, ':/icons/75.png', 'Ctrl+Shift+P', 2, 's', 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '100-m-Lauf Handstoppung', '100M', '100m-Lauf', '(((100/((x+0,24)*0,90)-4,341)/0,00676)/49)', '00.00', 1, ':/icons/100.png', '', 2, 's', 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '100 m Kraul', '100KR', '100 Kraul', '12*(((100/(1,2*x))-0,3))', '00:00.00', 1, ':/icons/100k.png', 'Ctrl+Shift+N', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '25 m Kraul', '25KR', '25 Kraul', '12*(((100/(1,2*(4*x+6,3)))-0,3))', '00:00.00', 1, ':/icons/25k.png', 'Ctrl+Shift+V', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '100 m Brust', '100BR', '100 Brust', '12*(((100/(1,2*(x-14,5)))-0,3))', '00:00.00', 1, ':/icons/100b.png', 'Ctrl+Shift+C', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '50 m Brust', '50BR', '50 Brust', '12*(((100/(1,2*(2*x-10,5)))-0,3))', '00:00.00', 1, ':/icons/50b.png', 'Ctrl+Shift+X', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (1, 'Pauschenpferd', 'PPFR', 'Pauschenpferd', '1*x', '00.000', 1, ':/icons/seitpferd.png', 'Ctrl+Shift+U', 3, '', 'false', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '50 m Rücken', '50RÜ', '50 Rücken', '12*(((100/(1,2*(2*x-4,5)))-0,3))', '00:00.00', 1, ':/icons/50r.png', 'Ctrl+Shift+F1', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '25 m Rücken', '25RÜ', '25 Rücken', '12*(((100/(1,2*(4*x-5,7)))-0,3))', '00:00.00', 1, ':/icons/25r.png', 'Ctrl+Shift+M', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '50 m Schmetterling', '50SM', '50 Schmetterling', '12*(((100/(1,2*(2*x-1,5)))-0,3))', '00:00.00', 1, ':/icons/50s.png', 'Ctrl+Shift+F4', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '25 m Schmetterling', '25SM', '25 Schmetterling', '12*(((100/(1,2*(4*x-1,7)))-0,3))', '00:00.00', 1, ':/icons/25s.png', 'Ctrl+Shift+F3', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '200 m Freistil', '200FS', '200 Freistil', '12*(((100/(1,2*(0,5*x-5)))-0,3))', '00:00.00', 1, ':/icons/200f.png', 'Ctrl+Shift+F9', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '100 m Kraul', '100KR', '100 Kraul', '12*(((100/(1,2*(x-6,5)))-0,3))', '00:00.00', 1, ':/icons/100k.png', 'Ctrl+Shift+N', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '50 m Kraul', '50KR', '50 Kraul', '12*(((100/(1,2*(2*x-2,5)))-0,3))', '00:00.00', 1, ':/icons/50k.png', 'Ctrl+Shift+B', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '100 m Brust', '100BR', '100 Brust', '12*(((100/(1,2*(x-22,5)))-0,3))', '00:00.00', 1, ':/icons/100b.png', 'Ctrl+Shift+C', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '50 m Brust', '50BR', '50 Brust', '12*(((100/(1,2*(2*x-18,5)))-0,3))', '00:00.00', 1, ':/icons/50b.png', 'Ctrl+Shift+X', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '25 m Brust', '25BR', '25 Brust', '12*(((100/(1,2*(4*x-13,7)))-0,3))', '00:00.00', 1, ':/icons/25b.png', 'Ctrl+Shift+Y', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '100 m Rücken', '100RÜ', '100 Rücken', '12*(((100/(1,2*(x-13,5)))-0,3))', '00:00.00', 1, ':/icons/100r.png', 'Ctrl+Shift+F2', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '25 m Rücken', '25RÜ', '25 Rücken', '12*(((100/(1,2*(4*x-5,7)))-0,3))', '00:00.00', 1, ':/icons/25r.png', 'Ctrl+Shift+M', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '1.000-m-Lauf', '1000', '1000m', '(((1000/x)-2,158)/0,006)/49', '00:00.00', 1, ':/icons/1000.png', 'Ctrl+Shift+S', 2, '', 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '1.000-m-Lauf', '1000', '1000m', '((((1000/x)-1,9231)/0,00566)/49)', '00:00.00', 1, ':/icons/1000.png', 'Ctrl+Shift+S', 2, '', 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (1, 'Reck', 'RECK', 'Reck', '1*x', '00.000', 1, ':/icons/reck.png', 'Ctrl+Shift+T', 3, '', 'false', 'true', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '25 m Schmetterling', '25SM', '25 Schmetterling', '12*(((100/(1,2*(4*x-1,7)))-0,3))', '00:00.00', 1, ':/icons/25s.png', 'Ctrl+Shift+F3', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '200 m Freistil', '200FS', '200 Freistil', '12*(((100/(1,2*(0,5*x-10,5)))-0,3))', '00:00.00', 1, ':/icons/200f.png', 'Ctrl+Shift+F9', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (3, 'Easy Jump 30', 'EJ30', 'Easy Jump 30', 'x/7,5', '000', 1, ':/icons/rope1.png', 'Ctrl+Shift+F10', 0, NULL, 'false', 'true', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (1, 'Ringe', 'RINGE', 'Ringe', '1*x', '00.000', 1, ':/icons/ringe.png', 'Ctrl+Shift+I', 3, '', 'false', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '50-m-Lauf Handstoppung', '50M', '50m-Lauf', '(((100/((x+0,24)*1,7)-4,341)/0,00676)/49)', '00.00', 1, ':/icons/50.png', '', 2, 's', 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (6, 'Ball', 'BALL', 'Ball', '1*x', '00.000', 1, ':/icons/g-ball.png', '', 3, '', 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '75-m-Lauf Handstoppung', '75M', '75m-Lauf', '(((100/((x+0,24)*1,3)-4,341)/0,00676)/49)', '00.00', 1, ':/icons/75.png', '', 2, 's', 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '100-m-Lauf Handstoppung', '100M', '100m-Lauf', '(((100/(x+0,24)-4,341)/0,00676)/49)', '00.00', 1, ':/icons/100.png', '', 2, 's', 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '100-m-Lauf elektrische Zeitm.', '100M', '100m-Lauf', '(((100/(x*0,90)-4,341)/0,00676)/49)', '00.00', 1, ':/icons/100.png', 'Ctrl+Shift+F12', 2, 's', 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '50 m Kraul', '50KR', '50 Kraul', '12*(((100/(1,2*(2*x+3,5)))-0,3))', '00:00.00', 1, ':/icons/50k.png', 'Ctrl+Shift+B', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '100 m Rücken', '100RÜ', '100 Rücken', '12*(((100/(1,2*(x-7,5)))-0,3))', '00:00.00', 1, ':/icons/100r.png', 'Ctrl+Shift+F2', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '100 m Schmetterling', '100SM', '100 Schmetterling', '12*(((100/(1,2*(x-5,5)))-0,3))', '00:00.00', 1, ':/icons/100s.png', 'Ctrl+Shift+F5', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '25 m Kraul', '25KR', '25 Kraul', '12*(((100/(1,2*(4*x+6,3)))-0,3))', '00:00.00', 1, ':/icons/25k.png', 'Ctrl+Shift+V', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '50 m Rücken', '50RÜ', '50 Rücken', '12*(((100/(1,2*(2*x-10,5)))-0,3))', '00:00.00', 1, ':/icons/50r.png', 'Ctrl+Shift+F1', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '50 m Schmetterling', '50SM', '50 Schmetterling', '12*(((100/(1,2*(2*x-7,5)))-0,3))', '00:00.00', 1, ':/icons/50s.png', 'Ctrl+Shift+F4', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (6, 'Band', 'BAND', 'Band', '1*x', '00.000', 1, ':/icons/band.png', '', 3, '', 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (6, 'Keule', 'KEUL', 'Keule', '1*x', '00.000', 1, ':/icons/keule.png', '', 3, '', 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '50-m-Lauf Handstoppung', '50M', '50m-Lauf', '(((100/((x+0,24)*1,6)-4,341)/0,00676)/49)', '00.000', 1, ':/icons/50.png', '', 3, 's', 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (6, 'Reifen', 'REIF', 'Reifen', '1*x', '00.000', 1, ':/icons/reifen.png', '', 3, '', 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (2, '3.000-m-Lauf', '3000', '3000m', '(((3000/x)-1,3)/0,006)/49', '00:00.00', 1, ':/icons/3000.png', 'Ctrl+Shift+F', 2, '', 'false', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (1, 'Bank', 'BANK', 'Bank', '1*x', '00.000', 1, ':/icons/bank.png', '', 3, '', 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (1, 'Stufenbarren', 'STBARR', 'Stufenbarren', '1*x', '00.000', 1, ':/icons/barren.png', '', 3, '', 'false', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '25 m Brust', '25BR', '25 Brust', '12*(((100/(1,2*(4*x-13,7)))-0,3))', '00:00.00', 1, ':/icons/25b.png', 'Ctrl+Shift+Y', 2, NULL, 'true', 'true', 'false')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (5, '100 m Schmetterling', '100SM', '100 Schmetterling', '12*(((100/(1,2*(x-11,5)))-0,3))', '00:00.00', 1, ':/icons/100s.png', 'Ctrl+Shift+F5', 2, NULL, 'true', 'false', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (1, 'Sprung', 'SPRU', 'Sprung', '1*x', '00.000', 2, ':/icons/sprung.png', 'Ctrl+Shift+Q', 3, '', 'false', 'true', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (1, 'Barren', 'BARR', 'Barren', '1*x', '00.000', 1, ':/icons/barren.png', 'Ctrl+Shift+W', 3, '', 'false', 'true', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (1, 'Balken', 'BALK', 'Balken', '1*x', '00.000', 1, ':/icons/balken.png', 'Ctrl+Shift+R', 3, '', 'false', 'true', 'true')";
    lst4 << "INSERT INTO tfx_disziplinen (int_sportid, var_name, var_kurz1, var_kurz2, var_formel, var_maske, int_versuche, var_icon, var_kuerzel, int_berechnung, var_einheit, bol_bahnen, bol_m, bol_w) VALUES (1, 'Boden', 'BODEN', 'Boden', '1*x', '00.000', 1, ':/icons/boden.png', 'Ctrl+Shift+E', 3, '', 'false', 'true', 'true')";
    entries.insert("tfx_disziplinen",lst4);
    creator.insert("tfx_disziplinen","CREATE TABLE tfx_disziplinen ("
                   "int_disziplinenid "+primary()+" NOT NULL,"
                   "int_sportid integer NOT NULL,"
                   "var_name "+varchar(100)+","
                   "var_kurz1 "+varchar(6)+","
                   "var_kurz2 "+varchar(20)+","
                   "var_formel "+varchar(300)+","
                   "var_maske "+varchar(10)+","
                   "int_versuche integer DEFAULT 1,"
                   "var_icon "+varchar(50)+","
                   "var_kuerzel "+varchar(50)+","
                   "int_berechnung smallint DEFAULT 2,"
                   "var_einheit "+varchar(5)+","
                   "bol_bahnen boolean DEFAULT 'false',"
                   "bol_m boolean DEFAULT 'true',"
                   "bol_w boolean DEFAULT 'true',"
                   "int_formelid integer,"
                   "bol_berechnen boolean DEFAULT 'true',"
                   "CONSTRAINT pky_disziplinenid PRIMARY KEY (int_disziplinenid),"
                   "CONSTRAINT fky_sportid FOREIGN KEY (int_sportid) REFERENCES tfx_sport (int_sportid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_formelid FOREIGN KEY (int_formelid) REFERENCES tfx_formeln (int_formelid) ON UPDATE RESTRICT ON DELETE RESTRICT"
                   ")");

    QList<column> tfx_disziplinen_felder;
    QList<cons> ctfx_disziplinen_felder;
    column col3_1 = {"int_disziplinen_felderid","integer",0,"NO","nextval('tfx_disziplinen_felder_int_disziplinen_felderid_seq'::regclass)",""};
    column col3_2 = {"int_disziplinenid","integer",0,"NO","",""};
    column col3_3 = {"var_name",varchar(15,false),15,"YES","",""};
    column col3_4 = {"int_sortierung","smallint",0,"YES","",""};
    column col3_5 = {"bol_endwert","boolean",0,"YES","'true'",""};
    column col3_6 = {"bol_ausgangswert","boolean",0,"YES","'true'",""};
    column col3_7 = {"int_gruppe","smallint",0,"YES",1,""};
    column col3_8 = {"bol_enabled","boolean",0,"YES","'true'",""};
    tfx_disziplinen_felder << col3_1 << col3_2 << col3_3 << col3_4 << col3_5 << col3_6 << col3_7 << col3_8;
    cons con3_1 = {"pky_disziplinen_felderid","PRIMARY KEY","int_disziplinen_felderid","","","",""};
    cons con3_2 = {"fky_disziplinenid","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","CASCADE"};
    cons con3_3 = {"fky_disziplinenid","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","RESTRICT"};
    ctfx_disziplinen_felder << con3_1 << con3_2 << con3_3;
    tabellenNamen.append("tfx_disziplinen_felder");
    tabellen.insert("tfx_disziplinen_felder",tfx_disziplinen_felder);
    constraints.insert("tfx_disziplinen_felder",ctfx_disziplinen_felder);
    creator.insert("tfx_disziplinen_felder","CREATE TABLE tfx_disziplinen_felder ("
                   "int_disziplinen_felderid "+primary()+" NOT NULL,"
                   "int_disziplinenid integer NOT NULL,"
                   "var_name "+varchar(15)+","
                   "int_sortierung smallint,"
                   "bol_endwert boolean DEFAULT 'true',"
                   "bol_ausgangswert boolean DEFAULT 'true',"
                   "int_gruppe smallint DEFAULT 1,"
                   "bol_enabled boolean DEFAULT 'true',"
                   "CONSTRAINT pky_disziplinen_felderid PRIMARY KEY (int_disziplinen_felderid),"
                   "CONSTRAINT fky_disziplinenid FOREIGN KEY (int_disziplinenid) REFERENCES tfx_disziplinen (int_disziplinenid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_laender;
    QList<cons> ctfx_laender;
    column col4_1 = {"int_laenderid","integer",0,"NO","nextval('tfx_laender_int_laenderid_seq'::regclass)",""};
    column col4_2 = {"var_name",varchar(150,false),150,"YES","",""};
    column col4_3 = {"var_kuerzel",varchar(4,false),4,"YES","",""};
    tfx_laender << col4_1 << col4_2 << col4_3;
    cons con4_1 = {"pky_laenderid","PRIMARY KEY","int_laenderid","","","",""};
    ctfx_laender << con4_1;
    tabellenNamen.append("tfx_laender");
    tabellen.insert("tfx_laender",tfx_laender);
    constraints.insert("tfx_laender",ctfx_laender);
    entries.insert("tfx_laender",QStringList("INSERT INTO tfx_laender (var_name,var_kuerzel) VALUES ('Deutschland','GER')"));
    creator.insert("tfx_laender","CREATE TABLE tfx_laender ("
                   "int_laenderid "+primary()+" NOT NULL,"
                   "var_name "+varchar(150)+","
                   "var_kuerzel "+varchar(4)+","
                   "CONSTRAINT pky_laenderid PRIMARY KEY (int_laenderid)"
                   ")");

    QList<column> tfx_verbaende;
    QList<cons> ctfx_verbaende;
    column col5_1 = {"int_verbaendeid","integer",0,"NO","nextval('tfx_verbaende_int_verbaendeid_seq'::regclass)",""};
    column col5_2 = {"int_laenderid","integer",0,"NO","",""};
    column col5_3 = {"var_name",varchar(150,false),150,"YES","",""};
    column col5_4 = {"var_kuerzel",varchar(8,false),8,"YES","",""};
    tfx_verbaende << col5_1 << col5_2 << col5_3 << col5_4;
    cons con5_1 = {"pky_verbaendeid","PRIMARY KEY","int_verbaendeid","","","",""};
    cons con5_2 = {"fkx_laenderid","FOREIGN KEY","int_laenderid","tfx_laender","int_laenderid","RESTRICT","RESTRICT"};
    ctfx_verbaende << con5_1 << con5_2;
    tabellenNamen.append("tfx_verbaende");
    tabellen.insert("tfx_verbaende",tfx_verbaende);
    constraints.insert("tfx_verbaende",ctfx_verbaende);
    entries.insert("tfx_verbaende",QStringList("INSERT INTO tfx_verbaende (int_laenderid,var_name,var_kuerzel) VALUES (1,'kein Verband','kA')"));
    creator.insert("tfx_verbaende","CREATE TABLE tfx_verbaende ("
                   "int_verbaendeid "+primary()+" NOT NULL,"
                   "int_laenderid integer NOT NULL,"
                   "var_name "+varchar(150)+","
                   "var_kuerzel "+varchar(8)+","
                   "CONSTRAINT pky_verbaendeid PRIMARY KEY (int_verbaendeid),"
                   "CONSTRAINT fkx_laenderid FOREIGN KEY (int_laenderid) REFERENCES tfx_laender (int_laenderid) ON UPDATE RESTRICT ON DELETE RESTRICT"
                   ")");

    QList<column> tfx_gaue;
    QList<cons> ctfx_gaue;
    column col6_1 = {"int_gaueid","integer",0,"NO","nextval('tfx_gaue_int_gaueid_seq'::regclass)",""};
    column col6_2 = {"int_verbaendeid","integer",0,"NO","",""};
    column col6_3 = {"var_name",varchar(150,false),150,"YES","",""};
    column col6_4 = {"var_kuerzel",varchar(15,false),15,"YES","",""};
    tfx_gaue << col6_1 << col6_2 << col6_3 << col6_4;
    cons con6_1 = {"pky_gaueid","PRIMARY KEY","int_gaueid","","","",""};
    cons con6_2 = {"fky_verbaendeid","FOREIGN KEY","int_verbaendeid","tfx_verbaende","int_verbaendeid","RESTRICT","RESTRICT"};
    ctfx_gaue << con6_1 << con6_2;
    tabellenNamen.append("tfx_gaue");
    tabellen.insert("tfx_gaue",tfx_gaue);
    constraints.insert("tfx_gaue",ctfx_gaue);
    entries.insert("tfx_gaue",QStringList("INSERT INTO tfx_gaue (int_verbaendeid,var_name,var_kuerzel) VALUES (1,'kein Turngau/Turnkreis','kA')"));
    creator.insert("tfx_gaue","CREATE TABLE tfx_gaue ("
                   "int_gaueid "+primary()+" NOT NULL,"
                   "int_verbaendeid integer NOT NULL,"
                   "var_name "+varchar(150)+","
                   "var_kuerzel "+varchar(15)+","
                   "CONSTRAINT pky_gaueid PRIMARY KEY (int_gaueid),"
                   "CONSTRAINT fky_verbaendeid FOREIGN KEY (int_verbaendeid) REFERENCES tfx_verbaende (int_verbaendeid) ON UPDATE RESTRICT ON DELETE RESTRICT"
                   ")");

    QList<column> tfx_personen;
    QList<cons> ctfx_personen;
    column col7_1 = {"int_personenid","integer",0,"NO","nextval('tfx_personen_int_personenid_seq'::regclass)",""};
    column col7_2 = {"var_vorname",varchar(150,false),150,"YES","",""};
    column col7_3 = {"var_nachname",varchar(150,false),150,"YES","",""};
    column col7_4 = {"var_adresse",varchar(200,false),200,"YES","",""};
    column col7_5 = {"var_plz",varchar(5,false),5,"YES","",""};
    column col7_6 = {"var_ort",varchar(150,false),150,"YES","",""};
    column col7_7 = {"var_telefon",varchar(25,false),25,"YES","",""};
    column col7_8 = {"var_fax",varchar(25,false),25,"YES","",""};
    column col7_9 = {"var_email",varchar(200,false),200,"YES","",""};
    tfx_personen << col7_1 << col7_2 << col7_3 << col7_4 << col7_5 << col7_6 << col7_7 << col7_8 << col7_9;
    cons con7_1 = {"pky_personenid","PRIMARY KEY","int_personenid","","","",""};
    ctfx_personen << con7_1;
    tabellenNamen.append("tfx_personen");
    tabellen.insert("tfx_personen",tfx_personen);
    constraints.insert("tfx_personen",ctfx_personen);
    creator.insert("tfx_personen","CREATE TABLE tfx_personen ("
                   "int_personenid "+primary()+" NOT NULL,"
                   "var_vorname "+varchar(150)+","
                   "var_nachname "+varchar(150)+","
                   "var_adresse "+varchar(200)+","
                   "var_plz "+varchar(5)+","
                   "var_ort "+varchar(150)+","
                   "var_telefon "+varchar(25)+","
                   "var_fax "+varchar(25)+","
                   "var_email "+varchar(200)+","
                   "CONSTRAINT pky_personenid PRIMARY KEY (int_personenid)"
                   ")");

    QList<column> tfx_vereine;
    QList<cons> ctfx_vereine;
    column col8_1 = {"int_vereineid","integer",0,"NO","nextval('tfx_vereine_int_vereineid_seq'::regclass)",""};
    column col8_2 = {"int_personenid","integer",0,"YES","",""};
    column col8_3 = {"var_name",varchar(150,false),150,"YES","",""};
    column col8_4 = {"int_start_ort","smallint",0,"YES","0",""};
    column col8_5 = {"var_website",varchar(200,false),200,"YES","",""};
    column col8_6 = {"int_gaueid","integer",0,"NO",1,""};
    tfx_vereine << col8_1 << col8_2 << col8_3 << col8_4 << col8_5 << col8_6;
    cons con8_1 = {"pky_vereineid","PRIMARY KEY","int_vereineid","","","",""};
    cons con8_2 = {"fky_gaueid","FOREIGN KEY","int_gaueid","tfx_gaue","int_gaueid","RESTRICT","RESTRICT"};
    cons con8_3 = {"fky_personenid","FOREIGN KEY","int_personenid","tfx_personen","int_personenid","RESTRICT","RESTRICT"};
    ctfx_vereine << con8_1 << con8_2 << con8_3;
    tabellenNamen.append("tfx_vereine");
    tabellen.insert("tfx_vereine",tfx_vereine);
    constraints.insert("tfx_vereine",ctfx_vereine);
    creator.insert("tfx_vereine","CREATE TABLE tfx_vereine ("
                   "int_vereineid "+primary()+" NOT NULL,"
                   "int_personenid integer,"
                   "var_name "+varchar(150)+","
                   "int_start_ort smallint DEFAULT 0,"
                   "var_website "+varchar(200)+","
                   "int_gaueid integer NOT NULL DEFAULT 1,"
                   "CONSTRAINT pky_vereineid PRIMARY KEY (int_vereineid),"
                   "CONSTRAINT fky_gaueid FOREIGN KEY (int_gaueid) REFERENCES tfx_gaue (int_gaueid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_personenid FOREIGN KEY (int_personenid) REFERENCES tfx_personen (int_personenid) ON UPDATE RESTRICT ON DELETE RESTRICT"
                   ")");

    QList<column> tfx_gruppen;
    QList<cons> ctfx_gruppen;
    column col9_1 = {"int_gruppenid","integer",0,"NO","nextval('tfx_gruppen_int_gruppenid_seq'::regclass)",""};
    column col9_2 = {"int_vereineid","integer",0,"NO","",""};
    column col9_3 = {"var_name",varchar(150,false),150,"YES","",""};
    tfx_gruppen << col9_1 << col9_2 << col9_3;
    cons con9_1 = {"pky_gruppenid","PRIMARY KEY","int_gruppenid","","","",""};
    ctfx_gruppen << con9_1;
    tabellenNamen.append("tfx_gruppen");
    tabellen.insert("tfx_gruppen",tfx_gruppen);
    constraints.insert("tfx_gruppen",ctfx_gruppen);
    creator.insert("tfx_gruppen","CREATE TABLE tfx_gruppen ("
                   "int_gruppenid "+primary()+" NOT NULL,"
                   "int_vereineid integer NOT NULL,"
                   "var_name "+varchar(150)+","
                   "CONSTRAINT pky_gruppenid PRIMARY KEY (int_gruppenid)"
                   ")");

    QList<column> tfx_teilnehmer;
    QList<cons> ctfx_teilnehmer;
    column col11_1 = {"int_teilnehmerid","integer",0,"NO","nextval('tfx_teilnehmer_int_teilnehmerid_seq'::regclass)",""};
    column col11_2 = {"int_vereineid","integer",0,"NO","",""};
    column col11_3 = {"var_vorname",varchar(150,false),150,"YES","",""};
    column col11_4 = {"var_nachname",varchar(150,false),150,"YES","",""};
    column col11_5 = {"int_geschlecht","smallint",0,"NO","",""};
    column col11_6 = {"dat_geburtstag","date",0,"YES","",""};
    column col11_7 = {"bool_nur_jahr","boolean",0,"YES","'true'",""};
    column col11_8 = {"int_startpassnummer","integer",0,"YES",0,""};
    tfx_teilnehmer << col11_1 << col11_2 << col11_3 << col11_4 << col11_5 << col11_6 << col11_7 << col11_8;
    cons con11_1 = {"pky_teilnehmerid","PRIMARY KEY","int_teilnehmerid","","","",""};
    cons con11_2 = {"fky_vereineid","FOREIGN KEY","int_vereineid","tfx_vereine","int_vereineid","RESTRICT","RESTRICT"};
    ctfx_teilnehmer << con11_1 << con11_2;
    tabellenNamen.append("tfx_teilnehmer");
    tabellen.insert("tfx_teilnehmer",tfx_teilnehmer);
    constraints.insert("tfx_teilnehmer",ctfx_teilnehmer);
    creator.insert("tfx_teilnehmer","CREATE TABLE tfx_teilnehmer ("
                   "int_teilnehmerid "+primary()+" NOT NULL,"
                   "int_vereineid integer NOT NULL,"
                   "var_vorname "+varchar(150)+","
                   "var_nachname "+varchar(150)+","
                   "int_geschlecht smallint NOT NULL,"
                   "dat_geburtstag date,"
                   "bool_nur_jahr boolean DEFAULT 'true',"
                   "int_startpassnummer integer DEFAULT 0,"
                   "CONSTRAINT pky_teilnehmerid PRIMARY KEY (int_teilnehmerid),"
                   "CONSTRAINT fky_vereineid FOREIGN KEY (int_vereineid) REFERENCES tfx_vereine (int_vereineid) ON UPDATE RESTRICT ON DELETE RESTRICT"
                   ")");

    QList<column> tfx_konten;
    QList<cons> ctfx_konten;
    column col12_1 = {"int_kontenid","integer",0,"NO","nextval('tfx_konten_int_kontenid_seq'::regclass)",""};
    column col12_2 = {"var_name",varchar(150,false),150,"YES","",""};
    column col12_3 = {"var_kontonummer",varchar(10,false),10,"YES","",""};
    column col12_4 = {"var_blz",varchar(8,false),8,"YES","",""};
    column col12_5 = {"var_bank",varchar(150,false),150,"YES","",""};
    column col12_6 = {"var_inhabe",varchar(150,false),150,"YES","",""};
    tfx_konten << col12_1 << col12_2 << col12_3 << col12_4 << col12_5 << col12_6;
    cons con12_1 = {"pky_kontenid","PRIMARY KEY","int_kontenid","","","",""};
    ctfx_konten << con12_1;
    tabellenNamen.append("tfx_konten");
    tabellen.insert("tfx_konten",tfx_konten);
    constraints.insert("tfx_konten",ctfx_konten);
    creator.insert("tfx_konten","CREATE TABLE tfx_konten ("
                   "int_kontenid "+primary()+" NOT NULL,"
                   "var_name "+varchar(150)+","
                   "var_kontonummer "+varchar(10)+","
                   "var_blz "+varchar(8)+","
                   "var_bank "+varchar(150)+","
                   "var_inhabe "+varchar(150)+","
                   "CONSTRAINT pky_kontenid PRIMARY KEY (int_kontenid)"
                   ")");

    QList<column> tfx_status;
    QList<cons> ctfx_status;
    column col27_1 = {"int_statusid","integer",0,"NO","nextval('tfx_status_int_statusid_seq'::regclass)",""};
    column col27_2 = {"var_name",varchar(150,false),150,"YES","",""};
    column col27_3 = {"ary_colorcode",varchar(25,false),25,"YES","{0,0,0}",""};
    column col27_4 = {"bol_bogen","boolean",0,"YES","'true'",""};
    column col27_5 = {"bol_karte","boolean",0,"YES","'true'",""};
    tfx_status << col27_1 << col27_2 << col27_3 << col27_4 << col27_5;
    cons con27_1 = {"pky_statusid","PRIMARY KEY","int_statusid","","","",""};
    ctfx_status << con27_1;
    tabellenNamen.append("tfx_status");
    tabellen.insert("tfx_status",tfx_status);
    constraints.insert("tfx_status",ctfx_status);
    QStringList lst3;
    lst3 << "INSERT INTO tfx_status (var_name, ary_colorcode, bol_bogen, bol_karte) VALUES ('kein Status', '{255,255,255}', 'true', 'true')";
    lst3 << "INSERT INTO tfx_status (var_name, ary_colorcode, bol_bogen, bol_karte) VALUES ('Meldung erfasst', '{249,105,5}', 'true', 'true')";
    lst3 << "INSERT INTO tfx_status (var_name, ary_colorcode, bol_bogen, bol_karte) VALUES ('Riegen eingeteilt', '{251,170,12}', 'true', 'false')";
    lst3 << "INSERT INTO tfx_status (var_name, ary_colorcode, bol_bogen, bol_karte) VALUES ('Riegenbogen gedruckt', '{251,249,42}', 'true', 'false')";
    lst3 << "INSERT INTO tfx_status (var_name, ary_colorcode, bol_bogen, bol_karte) VALUES ('Wettkampfkarte gedruckt', '{251,249,42}', 'false', 'true')";
    lst3 << "INSERT INTO tfx_status (var_name, ary_colorcode, bol_bogen, bol_karte) VALUES ('Wettkampf gestartet', '{108,208,34}', 'true', 'true')";
    lst3 << "INSERT INTO tfx_status (var_name, ary_colorcode, bol_bogen, bol_karte) VALUES ('Riegenbogen im Wettkampfbüro', '{0,255,0}', 'true', 'false')";
    lst3 << "INSERT INTO tfx_status (var_name, ary_colorcode, bol_bogen, bol_karte) VALUES ('Wettkampfkarte im Wettkampfbüro', '{0,255,0}', 'false', 'true')";
    lst3 << "INSERT INTO tfx_status (var_name, ary_colorcode, bol_bogen, bol_karte) VALUES ('Leistungen erfasst', '{4,172,39}', 'true', 'true')";
    lst3 << "INSERT INTO tfx_status (var_name, ary_colorcode, bol_bogen, bol_karte) VALUES ('Urkunde gedruckt', '{5,201,252}', 'false', 'true')";
    entries.insert("tfx_status",lst3);
    creator.insert("tfx_status","CREATE TABLE tfx_status ("
                   "int_statusid "+primary()+" NOT NULL,"
                   "var_name "+varchar(150)+","
                   "ary_colorcode "+varchar(25)+" DEFAULT '{0,0,0}',"
                   "bol_bogen boolean DEFAULT 'true',"
                   "bol_karte boolean DEFAULT 'true',"
                   "CONSTRAINT pky_statusid PRIMARY KEY (int_statusid)"
                   ")");

    QList<column> tfx_wettkampforte;
    QList<cons> ctfx_wettkampforte;
    column col28_1 = {"int_wettkampforteid","integer",0,"NO","nextval('tfx_wettkampforte_int_wettkampforteid_seq'::regclass)",""};
    column col28_2 = {"var_name",varchar(150,false),150,"YES","",""};
    column col28_3 = {"var_adresse",varchar(200,false),200,"YES","",""};
    column col28_4 = {"var_plz",varchar(5,false),5,"YES","",""};
    column col28_5 = {"var_ort",varchar(150,false),150,"YES","",""};
    tfx_wettkampforte << col28_1 << col28_2 << col28_3 << col28_4 << col28_5;
    cons con28_1 = {"pky_wettkampforteid","PRIMARY KEY","int_wettkampforteid","","","",""};
    ctfx_wettkampforte << con28_1;
    tabellenNamen.append("tfx_wettkampforte");
    tabellen.insert("tfx_wettkampforte",tfx_wettkampforte);
    constraints.insert("tfx_wettkampforte",ctfx_wettkampforte);
    creator.insert("tfx_wettkampforte","CREATE TABLE tfx_wettkampforte ("
                   "int_wettkampforteid "+primary()+" NOT NULL,"
                   "var_name "+varchar(150)+","
                   "var_adresse "+varchar(200)+","
                   "var_plz "+varchar(5)+","
                   "var_ort "+varchar(150)+","
                   "CONSTRAINT pky_wettkampforteid PRIMARY KEY (int_wettkampforteid)"
                   ")");

    QList<column> tfx_veranstaltungen;
    QList<cons> ctfx_veranstaltungen;
    column col13_1 = {"int_veranstaltungenid","integer",0,"NO","nextval('tfx_veranstaltungen_int_veranstaltungenid_seq'::regclass)",""};
    column col13_2 = {"int_wettkampforteid","integer",0,"NO","",""};
    column col13_3 = {"int_meldung_an","integer",0,"YES","",""};
    column col13_4 = {"int_ansprechpartner","integer",0,"YES","",""};
    column col13_5 = {"int_kontenid","integer",0,"YES","",""};
    column col13_6 = {"int_hauptwettkampf","integer",0,"YES","",""};
    column col13_7 = {"var_name",varchar(250,false),250,"YES","",""};
    column col13_8 = {"int_runde","smallint",0,"YES","1",""};
    column col13_9 = {"dat_von","date",0,"NO","",""};
    column col13_10 = {"dat_bis","date",0,"NO","",""};
    column col13_11 = {"dat_meldeschluss","date",0,"YES","",""};
    column col13_12 = {"bol_rundenwettkampf","boolean",0,"YES","'false'",""};
    column col13_13 = {"var_veranstalter",varchar(150,false),150,"YES","",""};
    column col13_14 = {"int_edv","smallint",0,"YES",0,""};
    column col13_15 = {"int_helfer","smallint",0,"YES",0,""};
    column col13_16 = {"int_kampfrichter","smallint",0,"YES",0,""};
    column col13_17 = {"var_meldung_website",varchar(200,false),200,"YES","",""};
    column col13_18 = {"var_verwendungszweck",varchar(150,false),150,"YES","",""};
    column col13_19 = {"rel_meldegeld","real",0,"YES",0,""};
    column col13_20 = {"rel_nachmeldung","real",0,"YES",0,""};
    column col13_21 = {"bol_faellig_nichtantritt","boolean",0,"YES","'false'",""};
    column col13_22 = {"bol_ummeldung_moeglich","boolean",0,"YES","'false'",""};
    column col13_23 = {"bol_nachmeldung_moeglich","boolean",0,"YES","'false'",""};
    column col13_24 = {"txt_meldung_an","text",0,"YES","",""};
    column col13_25 = {"txt_startberechtigung","text",0,"YES","",""};
    column col13_26 = {"txt_teilnahmebedingungen","text",0,"YES","",""};
    column col13_27 = {"txt_siegerauszeichnung","text",0,"YES","",""};
    column col13_28 = {"txt_kampfrichter","text",0,"YES","",""};
    column col13_29 = {"txt_hinweise","text",0,"YES","",""};
    tfx_veranstaltungen << col13_1 << col13_2 << col13_3 << col13_4 << col13_5 << col13_6 << col13_7 << col13_8 << col13_9 << col13_10 << col13_11 << col13_12 << col13_13 << col13_14 << col13_15 << col13_16 << col13_17 << col13_18 << col13_19 << col13_20 << col13_21 << col13_22 << col13_23 << col13_24 << col13_25 << col13_26 << col13_27 << col13_28 << col13_29;
    cons con13_1 = {"pky_veranstaltungenid","PRIMARY KEY","int_veranstaltungenid","","","",""};
    cons con13_2 = {"fky_ansprechpartner","FOREIGN KEY","int_ansprechpartner","tfx_personen","int_personenid","RESTRICT","RESTRICT"};
    cons con13_3 = {"fky_hauptwettkampf","FOREIGN KEY","int_hauptwettkampf","tfx_veranstaltungen","int_veranstaltungenid","RESTRICT","RESTRICT"};
    cons con13_4 = {"fky_kontenid","FOREIGN KEY","int_kontenid","tfx_konten","int_kontenid","RESTRICT","RESTRICT"};
    cons con13_5 = {"fky_meldung_an","FOREIGN KEY","int_meldung_an","tfx_personen","int_personenid","RESTRICT","RESTRICT"};
    cons con13_6 = {"fky_wettkampforteid","FOREIGN KEY","int_wettkampforteid","tfx_wettkampforte","int_wettkampforteid","RESTRICT","RESTRICT"};
    ctfx_veranstaltungen << con13_1 << con13_2 << con13_3 << con13_4 << con13_5 << con13_6;
    tabellenNamen.append("tfx_veranstaltungen");
    tabellen.insert("tfx_veranstaltungen",tfx_veranstaltungen);
    constraints.insert("tfx_veranstaltungen",ctfx_veranstaltungen);
    creator.insert("tfx_veranstaltungen","CREATE TABLE tfx_veranstaltungen ("
                   "int_veranstaltungenid "+primary()+" NOT NULL,"
                   "int_wettkampforteid integer NOT NULL,"
                   "int_meldung_an integer,"
                   "int_ansprechpartner integer,"
                   "int_kontenid integer,"
                   "int_hauptwettkampf integer,"
                   "var_name "+varchar(250)+","
                   "int_runde smallint DEFAULT 1,"
                   "dat_von date NOT NULL,"
                   "dat_bis date NOT NULL,"
                   "dat_meldeschluss date,"
                   "bol_rundenwettkampf boolean DEFAULT 'false',"
                   "var_veranstalter "+varchar(150)+","
                   "int_edv smallint DEFAULT 0,"
                   "int_helfer smallint DEFAULT 0,"
                   "int_kampfrichter smallint DEFAULT 0,"
                   "var_meldung_website "+varchar(200)+","
                   "var_verwendungszweck "+varchar(150)+","
                   "rel_meldegeld real DEFAULT 0,"
                   "rel_nachmeldung real DEFAULT 0,"
                   "bol_faellig_nichtantritt boolean DEFAULT 'false',"
                   "bol_ummeldung_moeglich boolean DEFAULT 'false',"
                   "bol_nachmeldung_moeglich boolean DEFAULT 'false',"
                   "txt_meldung_an text,"
                   "txt_startberechtigung text,"
                   "txt_teilnahmebedingungen text,"
                   "txt_siegerauszeichnung text,"
                   "txt_kampfrichter text,"
                   "txt_hinweise text,"
                   "CONSTRAINT pky_veranstaltungenid PRIMARY KEY (int_veranstaltungenid),"
                   "CONSTRAINT fky_ansprechpartner FOREIGN KEY (int_ansprechpartner) REFERENCES tfx_personen (int_personenid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_hauptwettkampf FOREIGN KEY (int_hauptwettkampf) REFERENCES tfx_veranstaltungen (int_veranstaltungenid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_kontenid FOREIGN KEY (int_kontenid) REFERENCES tfx_konten (int_kontenid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_meldung_an FOREIGN KEY (int_meldung_an) REFERENCES tfx_personen (int_personenid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_wettkampforteid FOREIGN KEY (int_wettkampforteid) REFERENCES tfx_wettkampforte (int_wettkampforteid) ON UPDATE RESTRICT ON DELETE RESTRICT"
                   ")");

    QList<column> tfx_wettkaempfe;
    QList<cons> ctfx_wettkaempfe;
    column col14_1 = {"int_wettkaempfeid","integer",0,"NO","nextval('tfx_wettkaempfe_int_wettkaempfeid_seq'::regclass)",""};
    column col14_2 = {"int_veranstaltungenid","integer",0,"NO","",""};
    column col14_3 = {"int_bereicheid","integer",0,"NO","",""};
    column col14_4 = {"int_typ","smallint",0,"YES",0,""};
    column col14_5 = {"var_nummer",varchar(5,false),5,"YES","",""};
    column col14_6 = {"var_name",varchar(150,false),150,"YES","",""};
    column col14_7 = {"yer_von","smallint",0,"NO","",""};
    column col14_8 = {"yer_bis","smallint",0,"YES","",""};
    column col14_9 = {"int_qualifikation","smallint",0,"YES",0,""};
    column col14_10 = {"int_wertungen","smallint",0,"YES","",""};
    column col14_11 = {"bol_streichwertung","boolean",0,"YES","'false'",""};
    column col14_12 = {"bol_ak_anzeigen","boolean",0,"YES","'false'",""};
    column col14_13 = {"bol_wahlwettkampf","boolean",0,"YES","'false'",""};
    column col14_14 = {"int_durchgang","smallint",0,"YES",1,""};
    column col14_15 = {"int_bahn","smallint",0,"YES",1,""};
    column col14_16 = {"tim_startzeit","time without time zone",0,"YES","",""};
    column col14_17 = {"tim_einturnen","time without time zone",0,"YES","",""};
    column col14_18 = {"bol_info_anzeigen","boolean",0,"YES","'false'",""};
    column col14_19 = {"bol_kp","boolean",0,"YES","'false'",""};
    column col14_20 = {"bol_sortasc","boolean",0,"YES","'false'",""};
    column col14_21 = {"bol_mansort","boolean",0,"YES","'false'",""};
    column col14_22 = {"bol_gerpkt","boolean",0,"YES","'false'",""};
    column col14_23 = {"int_anz_streich","smallint",0,"YES","0",""};
    tfx_wettkaempfe << col14_1 << col14_2 << col14_3 << col14_4 << col14_5 << col14_6 << col14_7 << col14_8 << col14_9 << col14_10 << col14_11 << col14_12 << col14_13 << col14_14 << col14_15 << col14_16 << col14_17 << col14_18 << col14_19 << col14_20 << col14_21 << col14_22 << col14_23;
    cons con14_1 = {"pky_wettkaempfeid","PRIMARY KEY","int_wettkaempfeid","","","",""};
    cons con14_2 = {"fky_bereicheid","FOREIGN KEY","int_bereicheid","tfx_bereiche","int_bereicheid","RESTRICT","RESTRICT"};
    cons con14_3 = {"fky_veranstaltungenid","FOREIGN KEY","int_veranstaltungenid","tfx_veranstaltungen","int_veranstaltungenid","RESTRICT","CASCADE"};
    ctfx_wettkaempfe << con14_1 << con14_2 << con14_3;
    tabellenNamen.append("tfx_wettkaempfe");
    tabellen.insert("tfx_wettkaempfe",tfx_wettkaempfe);
    constraints.insert("tfx_wettkaempfe",ctfx_wettkaempfe);
    creator.insert("tfx_wettkaempfe","CREATE TABLE tfx_wettkaempfe ("
                   "int_wettkaempfeid "+primary()+" NOT NULL,"
                   "int_veranstaltungenid integer NOT NULL,"
                   "int_bereicheid integer NOT NULL,"
                   "int_typ smallint DEFAULT 0,"
                   "var_nummer "+varchar(5)+","
                   "var_name "+varchar(150)+","
                   "yer_von smallint NOT NULL,"
                   "yer_bis smallint,"
                   "int_qualifikation smallint DEFAULT 0,"
                   "int_wertungen smallint,"
                   "bol_streichwertung boolean DEFAULT 'false',"
                   "bol_ak_anzeigen boolean DEFAULT 'false',"
                   "bol_wahlwettkampf boolean DEFAULT 'false',"
                   "int_durchgang smallint DEFAULT 1,"
                   "int_bahn smallint DEFAULT 1,"
                   "tim_startzeit time without time zone,"
                   "tim_einturnen time without time zone,"
                   "bol_info_anzeigen boolean DEFAULT 'false',"
                   "bol_kp boolean DEFAULT 'false',"
                   "bol_sortasc boolean DEFAULT 'false',"
                   "bol_mansort boolean DEFAULT 'false',"
                   "bol_gerpkt boolean DEFAULT 'false',"
                   "int_anz_streich smallint DEFAULT 0,"
                   "CONSTRAINT pky_wettkaempfeid PRIMARY KEY (int_wettkaempfeid),"
                   "CONSTRAINT fky_bereicheid FOREIGN KEY (int_bereicheid) REFERENCES tfx_bereiche (int_bereicheid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_veranstaltungenid FOREIGN KEY (int_veranstaltungenid) REFERENCES tfx_veranstaltungen (int_veranstaltungenid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_wettkaempfe_x_disziplinen;
    QList<cons> ctfx_wettkaempfe_x_disziplinen;
    column col15_1 = {"int_wettkaempfe_x_disziplinenid","integer",0,"NO","nextval('tfx_wettkaempfe_x_disziplinen_int_wettkaempfe_x_disziplinen_seq'::regclass)",""};
    column col15_2 = {"int_wettkaempfeid","integer",0,"NO","",""};
    column col15_3 = {"int_disziplinenid","integer",0,"NO","",""};
    column col15_4 = {"var_ausschreibung",varchar(100,false),100,"YES","",""};
    column col15_5 = {"int_sortierung","smallint",0,"YES","",""};
    column col15_6 = {"bol_kp","boolean",0,"YES","'false'",""};
    column col15_7 = {"rel_max","real",0,"YES","0",""};
    tfx_wettkaempfe_x_disziplinen << col15_1 << col15_2 << col15_3 << col15_4 << col15_5 << col15_6 << col15_7;
    cons con15_1 = {"pky_wettkaempfe_x_disziplinenid","PRIMARY KEY","int_wettkaempfe_x_disziplinenid","","","",""};
    cons con15_2 = {"fky_disziplinenid","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","CASCADE"};
    cons con15_3 = {"fky_disziplinenid","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","RESTRICT"};
    cons con15_4 = {"fky_wettkaempfeid","FOREIGN KEY","int_wettkaempfeid","tfx_wettkaempfe","int_wettkaempfeid","RESTRICT","CASCADE"};
    ctfx_wettkaempfe_x_disziplinen << con15_1 << con15_2 << con15_3 << con15_4;
    tabellenNamen.append("tfx_wettkaempfe_x_disziplinen");
    tabellen.insert("tfx_wettkaempfe_x_disziplinen", tfx_wettkaempfe_x_disziplinen);
    constraints.insert("tfx_wettkaempfe_x_disziplinen",ctfx_wettkaempfe_x_disziplinen);
    creator.insert("tfx_wettkaempfe_x_disziplinen","CREATE TABLE tfx_wettkaempfe_x_disziplinen ("
                   "int_wettkaempfe_x_disziplinenid "+primary()+" NOT NULL,"
                   "int_wettkaempfeid integer NOT NULL,"
                   "int_disziplinenid integer NOT NULL,"
                   "var_ausschreibung "+varchar(100)+","
                   "int_sortierung smallint,"
                   "bol_kp boolean DEFAULT 'false',"
                   "rel_max real DEFAULT 0,"
                   "CONSTRAINT pky_wettkaempfe_x_disziplinenid PRIMARY KEY (int_wettkaempfe_x_disziplinenid),"
                   "CONSTRAINT fky_disziplinenid FOREIGN KEY (int_disziplinenid) REFERENCES tfx_disziplinen (int_disziplinenid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_wettkaempfeid FOREIGN KEY (int_wettkaempfeid) REFERENCES tfx_wettkaempfe (int_wettkaempfeid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_wettkaempfe_dispos;
    QList<cons> ctfx_wettkaempfe_dispos;
    column col34_1 = {"int_wettkaempfe_disposid","integer",0,"NO","nextval('tfx_wettkaempfe_dispos_int_wettkaempfe_dispos_seq'::regclass)",""};
    column col34_2 = {"int_wettkaempfe_x_disziplinenid","integer",0,"NO","",""};
    column col34_3 = {"int_sortx","smallint",0,"YES","0",""};
    column col34_4 = {"int_sorty","smallint",0,"YES","0",""};
    column col34_5 = {"int_kp","smallint",0,"YES","0",""};
    tfx_wettkaempfe_dispos << col34_1 << col34_2 << col34_3 << col34_4 << col34_5;
    cons con34_1 = {"pky_wettkaempfe_disposid","PRIMARY KEY","int_wettkaempfe_disposid","","","",""};
    cons con34_2 = {"fky_wettkaempfe_x_disziplinenid","FOREIGN KEY","int_wettkaempfe_x_disziplinenid","tfx_wettkaempfe_x_disziplinen","int_wettkaempfe_x_disziplinenid","RESTRICT","CASCADE"};
    ctfx_wettkaempfe_dispos << con34_1 << con34_2;
    tabellenNamen.append("tfx_wettkaempfe_dispos");
    tabellen.insert("tfx_wettkaempfe_dispos", tfx_wettkaempfe_dispos);
    constraints.insert("tfx_wettkaempfe_dispos",ctfx_wettkaempfe_dispos);
    creator.insert("tfx_wettkaempfe_dispos","CREATE TABLE tfx_wettkaempfe_dispos ("
                   "int_wettkaempfe_disposid "+primary()+" NOT NULL,"
                   "int_wettkaempfe_x_disziplinenid integer NOT NULL,"
                   "int_sortx smallint DEFAULT 0,"
                   "int_sorty smallint DEFAULT 0,"
                   "int_kp smallint DEFAULT 0,"
                   "CONSTRAINT pky_wettkaempfe_disposid PRIMARY KEY (int_wettkaempfe_disposid),"
                   "CONSTRAINT fky_wettkaempfe_x_disziplinenid FOREIGN KEY (int_wettkaempfe_x_disziplinenid) REFERENCES tfx_wettkaempfe_x_disziplinen (int_wettkaempfe_x_disziplinenid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_mannschaften;
    QList<cons> ctfx_mannschaften;
    column col16_1 = {"int_mannschaftenid","integer",0,"NO","nextval('tfx_mannschaften_int_mannschaftenid_seq'::regclass)",""};
    column col16_2 = {"int_wettkaempfeid","integer",0,"NO","",""};
    column col16_3 = {"int_vereineid","integer",0,"NO","",""};
    column col16_4 = {"int_nummer","smallint",0,"YES",1,""};
    column col16_5 = {"var_riege",varchar(5,false),5,"YES",1,""};
    column col16_6 = {"int_startnummer","integer",0,"YES","",""};
    tfx_mannschaften << col16_1 << col16_2 << col16_3 << col16_4 << col16_5 << col16_6;
    cons con16_1 = {"pky_mannschaftenid","PRIMARY KEY","int_mannschaftenid","","","",""};
    cons con16_2 = {"fky_vereineid","FOREIGN KEY","int_vereineid","tfx_vereine","int_vereineid","RESTRICT","RESTRICT"};
    cons con16_3 = {"fky_wettkaempfeid","FOREIGN KEY","int_wettkaempfeid","tfx_wettkaempfe","int_wettkaempfeid","RESTRICT","CASCADE"};
    ctfx_mannschaften << con16_1 << con16_2 << con16_3;
    tabellenNamen.append("tfx_mannschaften");
    tabellen.insert("tfx_mannschaften",tfx_mannschaften);
    constraints.insert("tfx_mannschaften",ctfx_mannschaften);
    creator.insert("tfx_mannschaften","CREATE TABLE tfx_mannschaften ("
                   "int_mannschaftenid "+primary()+" NOT NULL,"
                   "int_wettkaempfeid integer NOT NULL,"
                   "int_vereineid integer NOT NULL,"
                   "int_nummer smallint DEFAULT 1,"
                   "var_riege "+varchar(5)+" DEFAULT 1,"
                   "int_startnummer integer,"
                   "CONSTRAINT pky_mannschaftenid PRIMARY KEY (int_mannschaftenid),"
                   "CONSTRAINT fky_vereineid FOREIGN KEY (int_vereineid) REFERENCES tfx_vereine (int_vereineid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_wettkaempfeid FOREIGN KEY (int_wettkaempfeid) REFERENCES tfx_wettkaempfe (int_wettkaempfeid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_mannschaften_abzug;
    QList<cons> ctfx_mannschaften_abzug;
    column col17_1 = {"int_mannschaften_abzugid","integer",0,"NO","nextval('tfx_mannschaften_abzug_int_mannschaften_abzugid_seq'::regclass)",""};
    column col17_2 = {"var_name",varchar(100,false),100,"YES","",""};
    column col17_3 = {"rel_abzug","real",0,"YES","",""};
    tfx_mannschaften_abzug << col17_1 << col17_2 << col17_3;
    cons con17_1 = {"pky_mannschaften_abzugid","PRIMARY KEY","int_mannschaften_abzugid","","","",""};
    ctfx_mannschaften_abzug << con17_1;
    tabellenNamen.append("tfx_mannschaften_abzug");
    tabellen.insert("tfx_mannschaften_abzug",tfx_mannschaften_abzug);
    constraints.insert("tfx_mannschaften_abzug",ctfx_mannschaften_abzug);
    creator.insert("tfx_mannschaften_abzug","CREATE TABLE tfx_mannschaften_abzug ("
                   "int_mannschaften_abzugid "+primary()+" NOT NULL,"
                   "var_name "+varchar(100)+","
                   "rel_abzug real,"
                   "CONSTRAINT pky_mannschaften_abzugid PRIMARY KEY (int_mannschaften_abzugid)"
                   ")");

    QList<column> tfx_man_x_man_ab;
    QList<cons> ctfx_man_x_man_ab;
    column col18_1 = {"int_man_x_man_abid","integer",0,"NO","nextval('tfx_man_x_man_ab_int_man_x_man_abid_seq'::regclass)",""};
    column col18_2 = {"int_mannschaftenid","integer",0,"NO","",""};
    column col18_3 = {"int_mannschaften_abzugid","integer",0,"NO","",""};
    tfx_man_x_man_ab << col18_1 << col18_2 << col18_3;
    cons con18_1 = {"pky_man_x_man_abid","PRIMARY KEY","int_man_x_man_abid","","","",""};
    cons con18_2 = {"fky_mannschaften_abzugid","FOREIGN KEY","int_mannschaften_abzugid","tfx_mannschaften_abzug","int_mannschaften_abzugid","RESTRICT","RESTRICT"};
    cons con18_3 = {"fky_mannschaftenid","FOREIGN KEY","int_mannschaftenid","tfx_mannschaften","int_mannschaftenid","RESTRICT","CASCADE"};
    ctfx_man_x_man_ab << con18_1 << con18_2 << con18_3;
    tabellenNamen.append("tfx_man_x_man_ab");
    tabellen.insert("tfx_man_x_man_ab",tfx_man_x_man_ab);
    constraints.insert("tfx_man_x_man_ab",ctfx_man_x_man_ab);
    creator.insert("tfx_man_x_man_ab","CREATE TABLE tfx_man_x_man_ab ("
                   "int_man_x_man_abid "+primary()+" NOT NULL,"
                   "int_mannschaftenid integer NOT NULL,"
                   "int_mannschaften_abzugid integer NOT NULL,"
                   "CONSTRAINT pky_man_x_man_abid PRIMARY KEY (int_man_x_man_abid),"
                   "CONSTRAINT fky_mannschaften_abzugid FOREIGN KEY (int_mannschaften_abzugid) REFERENCES tfx_mannschaften_abzug (int_mannschaften_abzugid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_mannschaftenid FOREIGN KEY (int_mannschaftenid) REFERENCES tfx_mannschaften (int_mannschaftenid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_man_x_teilnehmer;
    QList<cons> ctfx_man_x_teilnehmer;
    column col19_1 = {"int_man_x_teilnehmerid","integer",0,"NO","nextval('tfx_man_x_teilnehmer_int_man_x_teilnehmerid_seq'::regclass)",""};
    column col19_2 = {"int_mannschaftenid","integer",0,"NO","",""};
    column col19_3 = {"int_teilnehmerid","integer",0,"NO","",""};
    column col19_4 = {"int_runde","smallint",0,"YES","",""};
    tfx_man_x_teilnehmer << col19_1 << col19_2 << col19_3 << col19_4;
    cons con19_1 = {"pky_man_x_teilnehmerid","PRIMARY KEY","int_man_x_teilnehmerid","","","",""};
    cons con19_2 = {"fky_mannschaftenid","FOREIGN KEY","int_mannschaftenid","tfx_mannschaften","int_mannschaftenid","RESTRICT","CASCADE"};
    cons con19_3 = {"fky_teilnehmerid","FOREIGN KEY","int_teilnehmerid","tfx_teilnehmer","int_teilnehmerid","RESTRICT","RESTRICT"};
    ctfx_man_x_teilnehmer << con19_1 << con19_2 << con19_3;
    tabellenNamen.append("tfx_man_x_teilnehmer");
    tabellen.insert("tfx_man_x_teilnehmer",tfx_man_x_teilnehmer);
    constraints.insert("tfx_man_x_teilnehmer",ctfx_man_x_teilnehmer);
    creator.insert("tfx_man_x_teilnehmer","CREATE TABLE tfx_man_x_teilnehmer ("
                   "int_man_x_teilnehmerid "+primary()+" NOT NULL,"
                   "int_mannschaftenid integer NOT NULL,"
                   "int_teilnehmerid integer NOT NULL,"
                   "int_runde smallint,"
                   "CONSTRAINT pky_man_x_teilnehmerid PRIMARY KEY (int_man_x_teilnehmerid),"
                   "CONSTRAINT fky_mannschaftenid FOREIGN KEY (int_mannschaftenid) REFERENCES tfx_mannschaften (int_mannschaftenid) ON UPDATE RESTRICT ON DELETE CASCADE,"
                   "CONSTRAINT fky_teilnehmerid FOREIGN KEY (int_teilnehmerid) REFERENCES tfx_teilnehmer (int_teilnehmerid) ON UPDATE RESTRICT ON DELETE RESTRICT"
                   ")");

    QList<column> tfx_gruppen_x_teilnehmer;
    QList<cons> ctfx_gruppen_x_teilnehmer;
    column col20_1 = {"int_gruppen_x_teilnehmerid","integer",0,"NO","nextval('tfx_gruppen_x_teilnehmer_int_gruppen_x_teilnehmerid_seq'::regclass)",""};
    column col20_2 = {"int_gruppenid","integer",0,"NO","",""};
    column col20_3 = {"int_teilnehmerid","integer",0,"NO","",""};
    tfx_gruppen_x_teilnehmer << col20_1 << col20_2 << col20_3;
    cons con20_1 = {"pky_gruppen_x_teilnehmerid","PRIMARY KEY","int_gruppen_x_teilnehmerid","","","",""};
    cons con20_2 = {"fky_gruppenid","FOREIGN KEY","int_gruppenid","tfx_gruppen","int_gruppenid","RESTRICT","CASCADE"};
    cons con20_3 = {"fky_teilnehmerid","FOREIGN KEY","int_teilnehmerid","tfx_teilnehmer","int_teilnehmerid","RESTRICT","RESTRICT"};
    ctfx_gruppen_x_teilnehmer << con20_1 << con20_2 << con20_3;
    tabellenNamen.append("tfx_gruppen_x_teilnehmer");
    tabellen.insert("tfx_gruppen_x_teilnehmer",tfx_gruppen_x_teilnehmer);
    constraints.insert("tfx_gruppen_x_teilnehmer",ctfx_gruppen_x_teilnehmer);
    creator.insert("tfx_gruppen_x_teilnehmer","CREATE TABLE tfx_gruppen_x_teilnehmer ("
                   "int_gruppen_x_teilnehmerid "+primary()+" NOT NULL,"
                   "int_gruppenid integer NOT NULL,"
                   "int_teilnehmerid integer NOT NULL,"
                   "CONSTRAINT pky_gruppen_x_teilnehmerid PRIMARY KEY (int_gruppen_x_teilnehmerid),"
                   "CONSTRAINT fky_gruppenid FOREIGN KEY (int_gruppenid) REFERENCES tfx_gruppen (int_gruppenid) ON UPDATE RESTRICT ON DELETE CASCADE,"
                   "CONSTRAINT fky_teilnehmerid FOREIGN KEY (int_teilnehmerid) REFERENCES tfx_teilnehmer (int_teilnehmerid) ON UPDATE RESTRICT ON DELETE RESTRICT"
                   ")");

    QList<column> tfx_riegen_x_disziplinen;
    QList<cons> ctfx_riegen_x_disziplinen;
    column col21_1 = {"int_riegen_x_disziplinenid","integer",0,"NO","nextval('tfx_riegen_x_disziplinen_int_riegen_x_disziplinenid_seq'::regclass)",""};
    column col21_2 = {"int_veranstaltungenid","integer",0,"NO","",""};
    column col21_3 = {"int_disziplinenid","integer",0,"NO","",""};
    column col21_4 = {"int_statusid","integer",0,"NO","",""};
    column col21_5 = {"var_riege",varchar(5,false),5,"YES","",""};
    column col21_6 = {"int_runde","smallint",0,"YES","",""};
    column col21_7 = {"bol_erstes_geraet","boolean",0,"YES","'false'",""};
    tfx_riegen_x_disziplinen << col21_1 << col21_2 << col21_3 << col21_4 << col21_5 << col21_6 << col21_7;
    cons con21_1 = {"pky_riegen_x_disziplinenid","PRIMARY KEY","int_riegen_x_disziplinenid","","","",""};
    cons con21_2 = {"fky_disziplinenid","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","CASCADE"};
    cons con21_3 = {"fky_disziplinenid","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","RESTRICT"};
    cons con21_4 = {"fky_statusid","FOREIGN KEY","int_statusid","tfx_status","int_statusid","RESTRICT","RESTRICT"};
    cons con21_5 = {"fky_veranstaltungenid","FOREIGN KEY","int_veranstaltungenid","tfx_veranstaltungen","int_veranstaltungenid","RESTRICT","CASCADE"};
    ctfx_riegen_x_disziplinen << con21_1 << con21_2 << con21_3 << con21_4 << con21_5;
    tabellenNamen.append("tfx_riegen_x_disziplinen");
    tabellen.insert("tfx_riegen_x_disziplinen",tfx_riegen_x_disziplinen);
    constraints.insert("tfx_riegen_x_disziplinen",ctfx_riegen_x_disziplinen);
    creator.insert("tfx_riegen_x_disziplinen","CREATE TABLE tfx_riegen_x_disziplinen ("
                   "int_riegen_x_disziplinenid "+primary()+" NOT NULL,"
                   "int_veranstaltungenid integer NOT NULL,"
                   "int_disziplinenid integer NOT NULL,"
                   "int_statusid integer NOT NULL,"
                   "var_riege "+varchar(5)+","
                   "int_runde smallint,"
                   "bol_erstes_geraet boolean DEFAULT 'false',"
                   "CONSTRAINT pky_riegen_x_disziplinenid PRIMARY KEY (int_riegen_x_disziplinenid),"
                   "CONSTRAINT fky_disziplinenid FOREIGN KEY (int_disziplinenid) REFERENCES tfx_disziplinen (int_disziplinenid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_statusid FOREIGN KEY (int_statusid) REFERENCES tfx_status (int_statusid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_veranstaltungenid FOREIGN KEY (int_veranstaltungenid) REFERENCES tfx_veranstaltungen (int_veranstaltungenid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_wertungen;
    QList<cons> ctfx_wertungen;
    column col22_1 = {"int_wertungenid","integer",0,"NO","nextval('tfx_wertungen_int_wertungenid_seq'::regclass)",""};
    column col22_2 = {"int_wettkaempfeid","integer",0,"NO","",""};
    column col22_3 = {"int_teilnehmerid","integer",0,"YES","",""};
    column col22_4 = {"int_gruppenid","integer",0,"YES","",""};
    column col22_5 = {"int_mannschaftenid","integer",0,"YES","",""};
    column col22_6 = {"int_statusid","integer",0,"NO","",""};
    column col22_7 = {"int_runde","smallint",0,"YES",1,""};
    column col22_8 = {"int_startnummer","integer",0,"YES","",""};
    column col22_9 = {"bol_ak","boolean",0,"YES","'false'",""};
    column col22_10 = {"bol_startet_nicht","boolean",0,"YES","'false'",""};
    column col22_11 = {"var_riege",varchar(5,false),5,"YES","",""};
    column col22_12 = {"var_comment",varchar(150,false),150,"YES","",""};
    tfx_wertungen << col22_1 << col22_2 << col22_3 << col22_4 << col22_5 << col22_6 << col22_7 << col22_8 << col22_9 << col22_10 << col22_11 << col22_12;
    cons con22_1 = {"pky_wertungenid","PRIMARY KEY","int_wertungenid","","","",""};
    cons con22_2 = {"fky_gruppenid","FOREIGN KEY","int_gruppenid","tfx_gruppen","int_gruppenid","RESTRICT","CASCADE"};
    cons con22_3 = {"fky_mannschaftenid","FOREIGN KEY","int_mannschaftenid","tfx_mannschaften","int_mannschaftenid","RESTRICT","CASCADE"};
    cons con22_4 = {"fky_statusid","FOREIGN KEY","int_statusid","tfx_status","int_statusid","RESTRICT","RESTRICT"};
    cons con22_5 = {"fky_teilnehmerid","FOREIGN KEY","int_teilnehmerid","tfx_teilnehmer","int_teilnehmerid","RESTRICT","RESTRICT"};
    cons con22_6 = {"fky_wettkaempfeid","FOREIGN KEY","int_wettkaempfeid","tfx_wettkaempfe","int_wettkaempfeid","RESTRICT","CASCADE"};
    ctfx_wertungen << con22_1 << con22_2 << con22_3 << con22_4 << con22_5 << con22_6;
    tabellenNamen.append("tfx_wertungen");
    tabellen.insert("tfx_wertungen",tfx_wertungen);
    constraints.insert("tfx_wertungen",ctfx_wertungen);
    creator.insert("tfx_wertungen","CREATE TABLE tfx_wertungen ("
                   "int_wertungenid "+primary()+" NOT NULL,"
                   "int_wettkaempfeid integer NOT NULL,"
                   "int_teilnehmerid integer,"
                   "int_gruppenid integer,"
                   "int_mannschaftenid integer,"
                   "int_statusid integer NOT NULL,"
                   "int_runde smallint DEFAULT 1,"
                   "int_startnummer integer,"
                   "bol_ak boolean DEFAULT 'false',"
                   "bol_startet_nicht boolean DEFAULT 'false',"
                   "var_riege "+varchar(5)+","
                   "var_comment "+varchar(150)+","
                   "CONSTRAINT pky_wertungenid PRIMARY KEY (int_wertungenid),"
                   "CONSTRAINT fky_gruppenid FOREIGN KEY (int_gruppenid) REFERENCES tfx_gruppen (int_gruppenid) ON UPDATE RESTRICT ON DELETE CASCADE,"
                   "CONSTRAINT fky_mannschaftenid FOREIGN KEY (int_mannschaftenid) REFERENCES tfx_mannschaften (int_mannschaftenid) ON UPDATE RESTRICT ON DELETE CASCADE,"
                   "CONSTRAINT fky_statusid FOREIGN KEY (int_statusid) REFERENCES tfx_status (int_statusid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_teilnehmerid FOREIGN KEY (int_teilnehmerid) REFERENCES tfx_teilnehmer (int_teilnehmerid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_wettkaempfeid FOREIGN KEY (int_wettkaempfeid) REFERENCES tfx_wettkaempfe (int_wettkaempfeid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_wertungen_details;
    QList<cons> ctfx_wertungen_details;
    column col23_1 = {"int_wertungen_detailsid","integer",0,"NO","nextval('tfx_wertungen_details_int_wertungen_detailsid_seq'::regclass)",""};
    column col23_2 = {"int_wertungenid","integer",0,"NO","",""};
    column col23_3 = {"int_disziplinenid","integer",0,"NO","",""};
    column col23_4 = {"int_versuch","smallint",0,"YES","",""};
    column col23_5 = {"rel_leistung","real",0,"YES","",""};
    column col23_6 = {"int_kp","smallint",0,"YES",0,""};
    tfx_wertungen_details << col23_1 << col23_2 << col23_3 << col23_4 << col23_5 << col23_6;
    cons con23_1 = {"pky_wertungen_detailsid","PRIMARY KEY","int_wertungen_detailsid","","","",""};
    cons con23_2 = {"fky_disziplinenid","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","CASCADE"};
    cons con23_3 = {"fky_disziplinenid","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","RESTRICT"};
    cons con23_4 = {"fky_wertungenid","FOREIGN KEY","int_wertungenid","tfx_wertungen","int_wertungenid","RESTRICT","CASCADE"};
    ctfx_wertungen_details << con23_1 << con23_2 << con23_3 << con23_4;
    tabellenNamen.append("tfx_wertungen_details");
    tabellen.insert("tfx_wertungen_details",tfx_wertungen_details);
    constraints.insert("tfx_wertungen_details",ctfx_wertungen_details);
    creator.insert("tfx_wertungen_details","CREATE TABLE tfx_wertungen_details ("
                   "int_wertungen_detailsid "+primary()+" NOT NULL,"
                   "int_wertungenid integer NOT NULL,"
                   "int_disziplinenid integer NOT NULL,"
                   "int_versuch smallint,"
                   "rel_leistung real,"
                   "int_kp smallint DEFAULT 0,"
                   "CONSTRAINT pky_wertungen_detailsid PRIMARY KEY (int_wertungen_detailsid),"
                   "CONSTRAINT fky_disziplinenid FOREIGN KEY (int_disziplinenid) REFERENCES tfx_disziplinen (int_disziplinenid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_wertungenid FOREIGN KEY (int_wertungenid) REFERENCES tfx_wertungen (int_wertungenid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_wertungen_x_disziplinen;
    QList<cons> ctfx_wertungen_x_disziplinen;
    column col24_1 = {"int_wertungen_x_disziplinenid","integer",0,"NO","nextval('tfx_wertungen_x_disziplinen_int_wertungen_x_disziplinenid_seq'::regclass)",""};
    column col24_2 = {"int_wertungenid","integer",0,"NO","",""};
    column col24_3 = {"int_disziplinenid","integer",0,"NO","",""};
    tfx_wertungen_x_disziplinen << col24_1 << col24_2 << col24_3;
    cons con24_1 = {"pky_wertungen_x_disziplinenid","PRIMARY KEY","int_wertungen_x_disziplinenid","","","",""};
    cons con24_2 = {"fky_disziplinenid","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","CASCADE"};
    cons con24_3 = {"fky_disziplinenid","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","RESTRICT"};
    cons con24_4 = {"fky_wertungenid","FOREIGN KEY","int_wertungenid","tfx_wertungen","int_wertungenid","RESTRICT","CASCADE"};
    ctfx_wertungen_x_disziplinen << con24_1 << con24_2 << con24_3 << con24_4;
    tabellenNamen.append("tfx_wertungen_x_disziplinen");
    tabellen.insert("tfx_wertungen_x_disziplinen",tfx_wertungen_x_disziplinen);
    constraints.insert("tfx_wertungen_x_disziplinen",ctfx_wertungen_x_disziplinen);
    creator.insert("tfx_wertungen_x_disziplinen","CREATE TABLE tfx_wertungen_x_disziplinen ("
                   "int_wertungen_x_disziplinenid "+primary()+" NOT NULL,"
                   "int_wertungenid integer NOT NULL,"
                   "int_disziplinenid integer NOT NULL,"
                   "CONSTRAINT pky_wertungen_x_disziplinenid PRIMARY KEY (int_wertungen_x_disziplinenid),"
                   "CONSTRAINT fky_disziplinenid FOREIGN KEY (int_disziplinenid) REFERENCES tfx_disziplinen (int_disziplinenid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_wertungenid FOREIGN KEY (int_wertungenid) REFERENCES tfx_wertungen (int_wertungenid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_jury_results;
    QList<cons> ctfx_jury_results;
    column col25_1 = {"int_juryresultsid","integer",0,"NO","nextval('tfx_jury_results_int_jury_resultsid_seq'::regclass)",""};
    column col25_2 = {"int_wertungenid","integer",0,"YES","",""};
    column col25_3 = {"int_disziplinen_felderid","integer",0,"YES","",""};
    column col25_4 = {"int_versuch","smallint",0,"YES","",""};
    column col25_5 = {"rel_leistung","real",0,"YES","",""};
    column col25_6 = {"int_kp","smallint",0,"YES",0,""};
    tfx_jury_results << col25_1 << col25_2 << col25_3 << col25_4 << col25_5 << col25_6;
    cons con25_1 = {"pky_juryresultsid","PRIMARY KEY","int_juryresultsid","","","",""};
    cons con25_2 = {"fky_disziplinen_felderid","FOREIGN KEY","int_disziplinen_felderid","tfx_disziplinen_felder","int_disziplinen_felderid","RESTRICT","RESTRICT"};
    cons con25_3 = {"fky_wertungenid","FOREIGN KEY","int_wertungenid","tfx_wertungen","int_wertungenid","RESTRICT","CASCADE"};
    ctfx_jury_results << con25_1 << con25_2 << con25_3;
    tabellenNamen.append("tfx_jury_results");
    tabellen.insert("tfx_jury_results",tfx_jury_results);
    constraints.insert("tfx_jury_results",ctfx_jury_results);
    creator.insert("tfx_jury_results","CREATE TABLE tfx_jury_results ("
                   "int_juryresultsid "+primary()+" NOT NULL,"
                   "int_wertungenid integer,"
                   "int_disziplinen_felderid integer,"
                   "int_versuch smallint,"
                   "rel_leistung real,"
                   "int_kp smallint DEFAULT 0,"
                   "CONSTRAINT pky_juryresultsid PRIMARY KEY (int_juryresultsid),"
                   "CONSTRAINT fky_disziplinen_felderid FOREIGN KEY (int_disziplinen_felderid) REFERENCES tfx_disziplinen_felder (int_disziplinen_felderid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT fky_wertungenid FOREIGN KEY (int_wertungenid) REFERENCES tfx_wertungen (int_wertungenid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_quali_leistungen;
    QList<cons> ctfx_quali_leistungen;
    column col26_1 = {"int_quali_leistungenid","integer",0,"NO","nextval('tfx_quali_leistungen_int_quali_leistungenid_seq'::regclass)",""};
    column col26_2 = {"int_wertungenid","integer",0,"NO","",""};
    column col26_3 = {"int_disziplinenid","integer",0,"NO","",""};
    column col26_4 = {"rel_leistung","real",0,"YES","",""};
    tfx_quali_leistungen << col26_1 << col26_2 << col26_3 << col26_4;
    cons con26_1 = {"pky_quali_leistungenid","PRIMARY KEY","int_quali_leistungenid","","","",""};
    cons con26_2 = {"fky_disziplinenid","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","CASCADE"};
    cons con26_3 = {"fky_disziplinenid","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","RESTRICT"};
    cons con26_4 = {"int_wertungenid","FOREIGN KEY","int_wertungenid","tfx_wertungen","int_wertungenid","RESTRICT","CASCADE"};
    ctfx_quali_leistungen << con26_1 << con26_2 << con26_3 << con26_4;
    tabellenNamen.append("tfx_quali_leistungen");
    tabellen.insert("tfx_quali_leistungen",tfx_quali_leistungen);
    constraints.insert("tfx_quali_leistungen",ctfx_quali_leistungen);
    creator.insert("tfx_quali_leistungen","CREATE TABLE tfx_quali_leistungen ("
                   "int_quali_leistungenid "+primary()+" NOT NULL,"
                   "int_wertungenid integer NOT NULL,"
                   "int_disziplinenid integer NOT NULL,"
                   "rel_leistung real,"
                   "CONSTRAINT pky_quali_leistungenid PRIMARY KEY (int_quali_leistungenid),"
                   "CONSTRAINT fky_disziplinenid FOREIGN KEY (int_disziplinenid) REFERENCES tfx_disziplinen (int_disziplinenid) ON UPDATE RESTRICT ON DELETE RESTRICT,"
                   "CONSTRAINT int_wertungenid FOREIGN KEY (int_wertungenid) REFERENCES tfx_wertungen (int_wertungenid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_layouts;
    QList<cons> ctfx_layouts;
    column col29_1 = {"int_layoutid","integer",0,"NO","nextval('tfx_layouts_int_layoutid_seq'::regclass)",""};
    column col29_2 = {"var_name",varchar(100,false),100,"YES","",""};
    column col29_3 = {"txt_comment","text",0,"YES","",""};
    tfx_layouts << col29_1 << col29_2 << col29_3;
    cons con29_1 = {"pky_layoutid","PRIMARY KEY","int_layoutid","","","",""};
    ctfx_layouts << con29_1;
    tabellenNamen.append("tfx_layouts");
    tabellen.insert("tfx_layouts",tfx_layouts);
    constraints.insert("tfx_layouts",ctfx_layouts);
    creator.insert("tfx_layouts","CREATE TABLE tfx_layouts ("
                   "int_layoutid "+primary()+" NOT NULL,"
                   "var_name "+varchar(100)+","
                   "txt_comment text,"
                   "CONSTRAINT pky_layoutid PRIMARY KEY (int_layoutid)"
                   ")");

    QList<column> tfx_layout_felder;
    QList<cons> ctfx_layout_felder;
    column col30_1 = {"int_layout_felderid","integer",0,"NO","nextval('tfx_layout_felder_int_layout_felderid_seq'::regclass)",""};
    column col30_2 = {"int_layoutid","integer",0,"NO","",""};
    column col30_3 = {"int_typ","smallint",0,"YES","",""};
    column col30_4 = {"var_font",varchar(150,false),150,"YES","",""};
    column col30_5 = {"rel_x","real",0,"YES","",""};
    column col30_6 = {"rel_y","real",0,"YES","",""};
    column col30_7 = {"rel_w","real",0,"YES","",""};
    column col30_8 = {"rel_h","real",0,"YES","",""};
    column col30_9 = {"var_value",varchar(200,false),200,"YES","",""};
    column col30_10 = {"int_align","smallint",0,"YES","0",""};
    column col30_11 = {"int_layer","smallint",0,"YES","",""};
    tfx_layout_felder << col30_1 << col30_2 << col30_3 << col30_4 << col30_5 << col30_6 << col30_7 << col30_8 << col30_9 << col30_10 << col30_11;
    cons con30_1 = {"pky_layout_felderid","PRIMARY KEY","int_layout_felderid","","","",""};
    cons con30_2 = {"fky_layoutid","FOREIGN KEY","int_layoutid","tfx_layouts","int_layoutid","RESTRICT","CASCADE"};
    ctfx_layout_felder << con30_1 << con30_2;
    tabellenNamen.append("tfx_layout_felder");
    tabellen.insert("tfx_layout_felder",tfx_layout_felder);
    constraints.insert("tfx_layout_felder",ctfx_layout_felder);
    creator.insert("tfx_layout_felder","CREATE TABLE tfx_layout_felder ("
                   "int_layout_felderid "+primary()+" NOT NULL,"
                   "int_layoutid integer NOT NULL,"
                   "int_typ smallint,"
                   "var_font "+varchar(150)+","
                   "rel_x real,"
                   "rel_y real,"
                   "rel_w real,"
                   "rel_h real,"
                   "var_value "+varchar(200)+","
                   "int_align smallint DEFAULT 0,"
                   "int_layer smallint,"
                   "CONSTRAINT pky_layout_felderid PRIMARY KEY (int_layout_felderid),"
                   "CONSTRAINT fky_layoutid FOREIGN KEY (int_layoutid) REFERENCES tfx_layouts (int_layoutid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_disziplinen_gruppen;
    QList<cons> ctfx_disziplinen_gruppen;
    column col31_1 = {"int_disziplinen_gruppenid","integer",0,"NO","nextval('tfx_disziplinen_gruppen_int_disziplinen_gruppenid_seq'::regclass)",""};
    column col31_2 = {"var_name",varchar(100,false),100,"YES","",""};
    column col31_3 = {"txt_comment","text",0,"YES","",""};
    tfx_disziplinen_gruppen << col31_1 << col31_2 << col31_3;
    cons con31_1 = {"pky_disziplinen_gruppenid","PRIMARY KEY","int_layoutid","","","",""};
    ctfx_disziplinen_gruppen << con31_1;
    tabellenNamen.append("tfx_disziplinen_gruppen");
    tabellen.insert("tfx_disziplinen_gruppen",tfx_disziplinen_gruppen);
    constraints.insert("tfx_disziplinen_gruppen",ctfx_disziplinen_gruppen);
    creator.insert("tfx_disziplinen_gruppen","CREATE TABLE tfx_disziplinen_gruppen ("
                   "int_disziplinen_gruppenid "+primary()+" NOT NULL,"
                   "var_name "+varchar(100)+","
                   "txt_comment text,"
                   "CONSTRAINT pky_disziplinen_gruppenid PRIMARY KEY (int_disziplinen_gruppenid)"
                   ")");

    QList<column> tfx_disgrp_x_disziplinen;
    QList<cons> ctfx_disgrp_x_disziplinen;
    column col32_1 = {"int_disgrp_x_disziplinenid","integer",0,"NO","nextval('tfx_disgrp_x_disziplinen_int_disgrp_x_disziplinen_seq'::regclass)",""};
    column col32_2 = {"int_disziplinen_gruppenid","integer",0,"NO","",""};
    column col32_3 = {"int_disziplinenid","integer",0,"NO","",""};
    column col32_4 = {"int_pos","smallint",0,"YES","",""};
    tfx_disgrp_x_disziplinen << col32_1 << col32_2 << col32_3 << col32_4;
    cons con32_1 = {"pky_disgrp_x_disziplinen","PRIMARY KEY","int_disgrp_x_disziplinenid","","","",""};
    cons con32_2 = {"fky_disziplinen_gruppenid","FOREIGN KEY","int_disziplinen_gruppenid","tfx_disziplinen_gruppen","int_disziplinen_gruppenid","RESTRICT","CASCADE"};
    cons con32_3 = {"fky_disziplinen","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","CASCADE"};
    ctfx_disgrp_x_disziplinen << con32_1 << con32_2 << con32_3;
    tabellenNamen.append("tfx_disgrp_x_disziplinen");
    tabellen.insert("tfx_disgrp_x_disziplinen",tfx_disgrp_x_disziplinen);
    constraints.insert("tfx_disgrp_x_disziplinen",ctfx_disgrp_x_disziplinen);
    creator.insert("tfx_disgrp_x_disziplinen","CREATE TABLE tfx_disgrp_x_disziplinen ("
                   "int_disgrp_x_disziplinenid "+primary()+" NOT NULL,"
                   "int_disziplinen_gruppenid integer NOT NULL,"
                   "int_disziplinenid integer NOT NULL,"
                   "int_pos smallint,"
                   "CONSTRAINT pky_disgrp_x_disziplinen PRIMARY KEY (int_disgrp_x_disziplinenid),"
                   "CONSTRAINT fky_disziplinen_gruppenid FOREIGN KEY (int_disziplinen_gruppenid) REFERENCES tfx_disziplinen_gruppen (int_disziplinen_gruppenid) ON UPDATE RESTRICT ON DELETE CASCADE,"
                   "CONSTRAINT fky_disziplinen FOREIGN KEY (int_disziplinenid) REFERENCES tfx_disziplinen (int_disziplinenid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    QList<column> tfx_startreihenfolge;
    QList<cons> ctfx_startreihenfolge;
    column col35_1 = {"int_startreihenfolgeid","integer",0,"NO","nextval('tfx_startreihenfolge_int_startreihenfolgeid_seq'::regclass)",""};
    column col35_2 = {"int_wertungenid","integer",0,"NO","",""};
    column col35_3 = {"int_disziplinenid","integer",0,"NO","",""};
    column col35_4 = {"int_pos","smallint",0,"YES","",""};
    column col35_5 = {"int_kp","smallint",0,"YES",0,""};
    tfx_startreihenfolge << col35_1 << col35_2 << col35_3 << col35_4 << col35_5;
    cons con35_1 = {"pky_startreihenfolge","PRIMARY KEY","int_startreihenfolgeid","","","",""};
    cons con35_2 = {"fky_wertungenid","FOREIGN KEY","int_wertungenid","tfx_wertungen","int_wertungenid","RESTRICT","CASCADE"};
    cons con35_3 = {"fky_disziplinen","FOREIGN KEY","int_disziplinenid","tfx_disziplinen","int_disziplinenid","RESTRICT","CASCADE"};
    ctfx_startreihenfolge << con35_1 << con35_2 << con35_3;
    tabellenNamen.append("tfx_startreihenfolge");
    tabellen.insert("tfx_startreihenfolge",tfx_startreihenfolge);
    constraints.insert("tfx_startreihenfolge",ctfx_startreihenfolge);
    creator.insert("tfx_startreihenfolge","CREATE TABLE tfx_startreihenfolge ("
                   "int_startreihenfolgeid "+primary()+" NOT NULL,"
                   "int_wertungenid integer NOT NULL,"
                   "int_disziplinenid integer NOT NULL,"
                   "int_pos smallint,"
                   "int_kp smallint DEFAULT 0,"
                   "CONSTRAINT pky_startreihenfolge PRIMARY KEY (int_startreihenfolgeid),"
                   "CONSTRAINT fky_wertungenid FOREIGN KEY (int_wertungenid) REFERENCES tfx_wertungen (int_wertungenid) ON UPDATE RESTRICT ON DELETE CASCADE,"
                   "CONSTRAINT fky_disziplinen FOREIGN KEY (int_disziplinenid) REFERENCES tfx_disziplinen (int_disziplinenid) ON UPDATE RESTRICT ON DELETE CASCADE"
                   ")");

    check=true;
    for (int i=0;i<tabellenNamen.size();i++) {
        if (!check) continue;
        QString tabelle = tabellenNamen.at(i);
        emit newStatus("Überprüfe Tabelle "+tabelle,"processing.png");
        QSqlQuery checkExistsQuery;
        if (_global::getDBTyp() == 0) {
            checkExistsQuery.prepare("SELECT tablename FROM pg_tables WHERE tablename=?");
        } else if (_global::getDBTyp() == 1) {
            checkExistsQuery.prepare("SELECT name FROM sqlite_master WHERE name=? AND type='table'");
        }
        checkExistsQuery.bindValue(0,tabelle);
        checkExistsQuery.exec();
        if (_global::querySize(checkExistsQuery)>0) {
            QSqlQuery columns;
            if (_global::getDBTyp() == 0) {
                columns.prepare("SELECT column_name,data_type,is_nullable,column_default,character_maximum_length FROM information_schema.columns WHERE table_name=? ORDER BY ordinal_position");
                columns.bindValue(0,tabelle);
            } else if (_global::getDBTyp() == 1) {
                columns.prepare("PRAGMA table_info('"+tabelle+"')");
            }
            columns.exec();
            QList<column> spaltenListe = tabellen[tabelle];
            for (int j=0;j<spaltenListe.size();j++) {
                QString type = spaltenListe.at(j).typ;
                if (spaltenListe.at(j).length>0 && _global::getDBTyp()!=1) type += "("+QString::number(spaltenListe.at(j).length)+")";
                QString null="";
                if (spaltenListe.at(j).nullable == "NO") null = " NOT NULL";
                QString defaultValue="";
                if (spaltenListe.at(j).def != "") defaultValue = " DEFAULT "+spaltenListe.at(j).def.toString();

                if (columns.next()) {
                    QString type2;
                    QString columnName;
                    if (_global::getDBTyp() == 0) {
                        columnName = columns.value(0).toString();
                        type2 = columns.value(1).toString();
                        if (columns.value(4).toInt()>0) type2 += "("+columns.value(4).toString()+")";
                    } else if (_global::getDBTyp() == 1) {
                        columnName = columns.value(1).toString();
                        type2 = columns.value(2).toString();
                    }
                    if (type2 != type) {
                        emit newStatus("Spalte hat falschen Typ","warnung.png");
                        emit newStatus("Versuche Spalte umzuwandeln.","warnung.png");
                        if (spaltenListe.at(j).def != "") {
                            QSqlQuery defaultAlter("ALTER TABLE "+tabelle+" ALTER COLUMN "+columnName+" SET DEFAULT "+spaltenListe.at(j).def.toString());
                        } else {
                            QSqlQuery defaultAlter("ALTER TABLE "+tabelle+" ALTER COLUMN "+columnName+" DROP DEFAULT");
                        }
                        QSqlQuery alterTable("ALTER TABLE "+tabelle+" ALTER "+columnName+" TYPE "+type+spaltenListe.at(j).use);
                        if (alterTable.numRowsAffected()!=-1) {
                            emit newStatus("Spalte erfolgreich umgewandelt","haken.png");
                        } else {
                            emit newStatus("Es ist ein Fehler aufgetreten.","error.png");
                            emit newStatus("Fehler: "+alterTable.lastError().text(),"error.png");
                            check=false;
                            break;
                        }
                    }
                    if (columnName != spaltenListe.at(j).name) {
                        emit newStatus("Spalte hat falschen Namen","warnung.png");
                        emit newStatus("Versuche Spalte umzubennen.","warnung.png");
                        QSqlQuery alterTable("ALTER TABLE "+tabelle+" RENAME "+columnName+" TO "+spaltenListe.at(j).name);
                        if (alterTable.numRowsAffected()!=-1) {
                            emit newStatus("Spalte erfolgreich umbenannt","haken.png");
                        } else {
                            emit newStatus("Es ist ein Fehler aufgetreten.","error.png");
                            emit newStatus("Fehler: "+alterTable.lastError().text(),"error.png");
                            check=false;
                            break;
                        }
                    }
                    if (_global::getDBTyp() == 0 && columns.value(4).toInt() != spaltenListe.at(j).length) {
                        emit newStatus("Spalte hat falsche Länge","warnung.png");
                        emit newStatus("Versuche Spaltenlänge zu ändern.","warnung.png");
                        QSqlQuery alterTable("ALTER TABLE "+tabelle+" ALTER "+columnName+" TYPE "+type+spaltenListe.at(j).use);
                        if (alterTable.numRowsAffected()!=-1) {
                            emit newStatus("Spaltenlänge erfolgreich geändert","haken.png");
                        } else {
                            emit newStatus("Es ist ein Fehler aufgetreten.","error.png");
                            emit newStatus("Fehler: "+alterTable.lastError().text(),"error.png");
                            check=false;
                            break;
                        }
                    }
                } else {
                    emit newStatus("Spalte "+tabelle+" - "+spaltenListe.at(j).name+" fehlt in der Datenbank","warnung.png");
                    emit newStatus("Spalte wird zur Tabelle hinzugefügt...","warnung.png");
                    QSqlQuery insertColumn("ALTER TABLE "+tabelle+" ADD COLUMN "+spaltenListe.at(j).name+" "+type+null+defaultValue);
                    if (insertColumn.numRowsAffected()!=-1) {
                        emit newStatus("Spalte erfolgreich hinzugefügt","haken.png");
                    } else {
                        emit newStatus("Es ist ein Fehler aufgetreten.","error.png");
                        emit newStatus("Fehler: "+insertColumn.lastError().text(),"error.png");
                        check=false;
                        break;
                    }
                }
            }
            emit newStatus("Überprüfe Schlüsselfelder","processing.png");
            if (check) {
                QList<cons> keyListe = constraints[tabelle];
                for (int j=0;j<keyListe.size();j++) {
                    if (keyListe.at(j).typ == "PRIMARY KEY" && _global::getDBTyp()==1) continue;
                    QSqlQuery keyQuery;
                    if (_global::getDBTyp() == 0) {
                        keyQuery.prepare("SELECT DISTINCT tc.constraint_name, tc.constraint_type, ccu.table_name AS references_table, kcu.column_name, ccu.column_name AS references_field, rc.update_rule AS on_update, rc.delete_rule AS on_delete, tc.table_name FROM information_schema.table_constraints tc LEFT JOIN information_schema.key_column_usage kcu ON tc.constraint_catalog = kcu.constraint_catalog AND tc.constraint_schema = kcu.constraint_schema AND tc.constraint_name = kcu.constraint_name LEFT JOIN information_schema.referential_constraints rc ON tc.constraint_catalog = rc.constraint_catalog AND tc.constraint_schema = rc.constraint_schema AND tc.constraint_name = rc.constraint_name LEFT JOIN information_schema.constraint_column_usage ccu ON rc.unique_constraint_catalog = ccu.constraint_catalog AND rc.unique_constraint_schema = ccu.constraint_schema AND rc.unique_constraint_name = ccu.constraint_name WHERE tc.table_name=? ORDER BY tc.table_name, tc.constraint_type DESC");
                        keyQuery.bindValue(0,tabelle);
                    } else {
                        keyQuery.prepare("PRAGMA foreign_key_list('"+tabelle+"')");
                    }
                    keyQuery.exec();
                    bool found=false;
                    while (keyQuery.next()) {
                        if (keyListe.at(j).ftable==keyQuery.value(2).toString() && keyListe.at(j).fcolumn == keyQuery.value(4).toString()) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        emit newStatus("Schlüssel "+keyListe.at(j).name+" fehlt.","warnung.png");
                        emit newStatus("Versuche Schlüssel zu erstellen.","warnung.png");
                        QSqlQuery addKey("ALTER TABLE "+tabelle+" ADD CONSTRAINT "+keyListe.at(j).name+" "+keyListe.at(j).typ+" ("+keyListe.at(j).column+") REFERENCES "+keyListe.at(j).ftable+" ("+keyListe.at(j).fcolumn+") ON UPDATE "+keyListe.at(j).onUpdate+" ON DELETE "+keyListe.at(j).onDelete);
                        if (addKey.numRowsAffected()!=-1) {
                            emit newStatus("Schlüssel erfolgreich hinzugefügt","haken.png");
                        } else {
                            emit newStatus("Es ist ein Fehler aufgetreten.","error.png");
                            emit newStatus("Fehler: "+addKey.lastError().text(),"error.png");
                            check=false;
                            break;
                        }
                    }
                }
                if (check) emit newStatus("Überprüfung der Schlüssel erfolgreich.","haken.png");
            }
            if (check) emit newStatus("Überprüfung von Tabelle "+tabelle+" erfolgreich.","haken.png");
        } else {
            emit newStatus("Die Tabelle existiert nicht","warnung.png");
            emit newStatus("Tabelle wird zur Datenbank hinzugefügt...","warnung.png");
            QSqlQuery createTable(creator[tabelle]);
            if (createTable.numRowsAffected() == -1) {
                emit newStatus("Es ist ein Fehler aufgetreten.","error.png");
                emit newStatus("Fehler: "+createTable.lastError().text(),"error.png");
                emit newStatus("Bitte kontaktieren Sie den Support!","error.png");
                check = false;
            } else {
                emit newStatus("Tabelle "+tabelle+" erfolgreich erstellt.","haken.png");
                if (entries.value(tabelle).size()>0) {
                    emit newStatus("Einträge erstellen","processing.png");
                    for (int k=0;k<entries.value(tabelle).size();k++) {
                        QSqlQuery entry(entries.value(tabelle).at(k));
                    }
                    emit newStatus("Einträge wurden erstellt.","haken.png");
                }
            }
        }
    }
    if (check) emit newStatus("Die Datenbank ist aktuell.","haken.png");
    quit();
}

bool CheckTable_Thread::getCheck() {
    return check;
}

QString CheckTable_Thread::varchar(int length, bool showLength) {
    if (_global::getDBTyp() == 0) {
        QString ret = "character varying";
        if (showLength) ret += "("+QString::number(length)+")";
        return ret;
    } else if (_global::getDBTyp() == 1) {
        return "varchar";
    }
    return "varchar";
}

QString CheckTable_Thread::primary() {
    if (_global::getDBTyp() == 0) {
        return "serial";
    } else if (_global::getDBTyp() == 1) {
        return "integer";
    }
    return "integer";
}
