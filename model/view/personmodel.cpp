#include "personmodel.h"
#include "model/entity/person.h"
#include "model/entitymanager.h"
#include "model/repository/personrepository.h"

PersonModel::PersonModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{}

int PersonModel::rowCount(const QModelIndex &) const
{
    return m_persons.size();
}

int PersonModel::columnCount(const QModelIndex &) const
{
    return 9;
}

QVariant PersonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Vorname");
        case 1:
            return tr("Nachname");
        case 2:
            return tr("Adresse");
        case 3:
            return tr("PLZ");
        case 4:
            return tr("Ort");
        case 5:
            return tr("Telefon");
        case 6:
            return tr("Fax");
        case 7:
            return tr("Email");
        }
    }
    return QVariant();
}

QVariant PersonModel::data(const QModelIndex &index, int role) const
{
    Person *person = m_persons.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return person->firstName();
        case 1:
            return person->lastName();
        case 2:
            return person->address();
        case 3:
            return person->zip();
        case 4:
            return person->city();
        case 5:
            return person->phoneNumber();
        case 6:
            return person->faxNumber();
        case 7:
            return person->email();
        case 8:
            return QString("%1 %2").arg(person->firstName(), person->lastName());
        }
    } else if (role == Qt::UserRole) {
        return QVariant::fromValue(person);
    }
    return QVariant();
}

void PersonModel::fetchPersons()
{
    beginResetModel();
    m_persons = m_em->personRepository()->loadAll();
    endResetModel();
}
