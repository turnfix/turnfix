#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <QObject>

class DBTable;

class BankAccount : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString number READ number WRITE setNumber)
    Q_PROPERTY(QString routingCode READ routingCode WRITE setRoutingCode)
    Q_PROPERTY(QString institution READ institution WRITE setInstitution)
    Q_PROPERTY(QString accountHolder READ accountHolder WRITE setAccountHolder)

public:
    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    QString number() const;
    void setNumber(const QString &number);

    QString routingCode() const;
    void setRoutingCode(const QString &routingCode);

    QString institution() const;
    void setInstitution(const QString &institution);

    QString accountHolder() const;
    void setAccountHolder(const QString &accountHolder);

    static const DBTable *mapping();

private:
    int m_id = 0;
    QString m_name;
    QString m_number;
    QString m_routingCode;
    QString m_institution;
    QString m_accountHolder;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(BankAccount *);

#endif // BANKACCOUNT_H
