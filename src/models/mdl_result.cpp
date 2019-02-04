#include <QSqlQuery>
#include "header/mdl_result.h"
#include "../global/header/_global.h"

int QErgebnisTableModel::rowCount(const QModelIndex &) const {
    return list.size();
}

int QErgebnisTableModel::columnCount(const QModelIndex &) const {
    if (list.size() > 0) {
        if (details) {
            return list.at(0).size()-1;
        } else if (!showHeader) {
            return 5;
        } else {
            return 4;
        }
    }
    return 0;
}

QVariant QErgebnisTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DisplayRole) {
        if (details) {
            return list.at(index.row()).at(index.column());
        } else {
            if (!showHeader) {
                if (index.column()==2) {
                    QString value = list.at(index.row()).at(index.column());
                    QStringList values = value.split(". Mannschaft");
                    if (!showHeader && values.size()>0) {
                        return "";
                    } else {
                        return list.at(index.row()).at(index.column());
                    }
                } else if (index.column()==3) {
                    return list.at(index.row()).at(list.at(0).size()-2);
                } else if (index.column()==4) {
                    return list.at(index.row()).at(list.at(0).size()-1);
                } else {
                    return list.at(index.row()).at(index.column());
                }
            } else {
                if (index.column()==3) {
                    return list.at(index.row()).at(list.at(0).size()-1);
                } else {
                    return list.at(index.row()).at(index.column());
                }
            }
        }
    }
    return QVariant();
}

QVariant QErgebnisTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();
    if (!showHeader)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        int s;
        if (wktyp == 0 || wktyp == 2) {
            s = 4;
            switch (section) {
            case 0: return "Platz"; break;
            case 1: return "Name"; break;
            case 2: return "Verein"; break;
            case 3: return "Jg."; break;
            }
        } else {
            s = 3;
            switch (section) {
            case 0: return "Platz"; break;
            case 1: return "Verein"; break;
            case 2: return "Mannschaft"; break;
            }
        }
        if (section < headers.size()+s) {
            return headers.at(section-s);
        } else {
            return "Gesamt";
        }
    }
    return QVariant();
}

void QErgebnisTableModel::setList(const QList<QStringList> &lst, QString n, int h, int typ, bool d, bool head) {
    list = lst;
    wktyp = typ;
    hwk = h;
    nr = n;
    details = d;
    showHeader = head;
    headers.clear();
    QSqlQuery dis;
    dis.prepare("SELECT var_kurz1, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid WHERE int_veranstaltungenid=? AND var_nummer=? ORDER BY int_sortierung");
    dis.bindValue(0,hwk);
    dis.bindValue(1, nr);
    dis.exec();
    while (dis.next()) {
        QString  name = dis.value(0).toString();
        if (dis.value(1).toInt() == 1) {
            headers.append(name+" (P)");
            headers.append(name+" (K)");
        } else {
            headers.append(name);
        }
    }
    this->reset();
}
