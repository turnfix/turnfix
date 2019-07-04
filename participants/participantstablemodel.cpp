#include "participantstablemodel.h"
#include "src/global/header/_global.h"

ParticipantsTableModel::ParticipantsTableModel(Event *event)
{
    this->event = event;
}

void ParticipantsTableModel::updateType(Type type)
{
    this->type = type;

    QSqlQuery query;
    if (type == Individual) {
        query.prepare("SELECT tfx_wertungen.int_startnummer, tfx_teilnehmer.var_nachname || ', ' || tfx_teilnehmer.var_vorname || CASE WHEN tfx_wertungen.bol_ak THEN ' (AK)' ELSE '' END, "+_global::date("dat_geburtstag",2)+", CASE WHEN tfx_teilnehmer.int_geschlecht=0 THEN 'w' ELSE 'm' END, tfx_vereine.var_name, tfx_wettkaempfe.var_nummer, tfx_wertungen.var_riege, tfx_wertungen.int_wertungenid FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) INNER JOIN tfx_vereine USING (int_vereineid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE tfx_wettkaempfe.int_veranstaltungenid=? AND int_runde=? AND tfx_wertungen.int_gruppenid IS NULL AND tfx_wertungen.int_mannschaftenid IS NULL ORDER BY tfx_wettkaempfe.var_nummer, "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, tfx_teilnehmer.var_nachname, tfx_teilnehmer.var_vorname");
        query.bindValue(0, this->event->getMainEventId());
        query.bindValue(1, this->event->getRound());
        query.exec();
    } else if (type == Team) {
        query.prepare("SELECT tfx_mannschaften.int_startnummer, tfx_vereine.var_name, tfx_mannschaften.int_nummer || '. Mannschaft', tfx_wettkaempfe.var_nummer, tfx_mannschaften.var_riege, tfx_mannschaften.int_mannschaftenid FROM tfx_mannschaften INNER JOIN tfx_vereine USING (int_vereineid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_mannschaften.int_wettkaempfeid WHERE tfx_wettkaempfe.int_veranstaltungenid=? ORDER BY tfx_wettkaempfe.var_nummer, "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, tfx_mannschaften.int_nummer");
        query.bindValue(0,this->event->getMainEventId());
    } else if (type == Group) {
        query.prepare("SELECT tfx_wertungen.int_startnummer, tfx_gruppen.var_name, tfx_vereine.var_name, tfx_wettkaempfe.var_nummer, tfx_wertungen.var_riege, tfx_gruppen.int_gruppenid FROM tfx_wertungen INNER JOIN tfx_gruppen USING (int_gruppenid) INNER JOIN tfx_vereine USING (int_vereineid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE tfx_wettkaempfe.int_veranstaltungenid=? AND int_runde=? ORDER BY tfx_wettkaempfe.var_nummer, "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, tfx_gruppen.var_name");
        query.bindValue(0, this->event->getMainEventId());
        query.bindValue(1, this->event->getRound());
    }
    query.exec();

    this->setQuery(query);
}

int ParticipantsTableModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int ParticipantsTableModel::columnCount(const QModelIndex &parent) const
{
    if (this->type == Individual) {
        return 7;
    }
    return 5;
}

QVariant ParticipantsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QString headersIndividual[7] = {"StNr.", "Name", "Geb.", "m/w", "Verein", "WK", "Riege"};
    QString headersTeam[5] = {"StNr.", "Verein", "Mannschaft", "WK", "Riege"};
    QString headersGroup[5] = {"StNr.", "Gruppe", "Verein", "WK", "Riege"};

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (this->type) {
        case Individual:
            return headersIndividual[section];
        case Team:
            return headersTeam[section];
        case Group:
            return headersGroup[section];
        }
    }
    return QVariant();
}
