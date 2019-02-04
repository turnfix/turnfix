#include <QDebug>
#include <QRegExp>
#include <QSqlError>
#include "db.h"
#include "tfsqlquery.h"
#include "model/settings/tfsettings.h"

bool TFSqlQuery::prepare(const QString &query)
{
    QString executeQuery = parseQuery(query);
    qDebug() << executeQuery;
    return QSqlQuery::prepare(executeQuery);
}

bool TFSqlQuery::exec()
{
    bool successful;

    successful = QSqlQuery::exec();

    if (!successful)
        DB::getInstance()->emitError(this->lastError().text());

    return successful;
}

bool TFSqlQuery::exec(const QString &query)
{
    bool successful;

    QString executeQuery = parseQuery(query);
    qDebug() << executeQuery;
    successful = QSqlQuery::exec(executeQuery);

    if (!successful)
        DB::getInstance()->emitError(this->lastError().text());

    return successful;
}

QString TFSqlQuery::parseQuery(QString query)
{
    QString replace;
    TFSettings *settings;

    settings = TFSettings::getInstance();

    /*========================================*
     *  Convert special commands to MultiSQL  *
     *  Commands                              *
     *========================================*/

    /*  Check if table exists                 */
    /*  TFSQL Command: SELECT TABLE WITH NAME */
    switch (settings->getDbTyp())
    {
    case DB::PostgreSQL: replace="SELECT tablename FROM pg_tables WHERE tablename="; break;
    case DB::SQLite: replace="SELECT name FROM sqlite_master WHERE type='table' AND name="; break;
    }
    query.replace("SELECT TABLE WITH NAME ",replace);

    /*  Get column information                  */
    /*  TFSQL Command: SELECT COLUMN INFOS FROM */
    switch (settings->getDbTyp())
    {
    case DB::PostgreSQL: replace="SELECT (ordinal_position-1), column_name,data_type,is_nullable,column_default,character_maximum_length FROM information_schema.columns WHERE table_name=\\1 ORDER BY ordinal_position"; break;
    case DB::SQLite: replace="PRAGMA table_info(\\1)"; break;
    }
    query.replace(QRegExp("SELECT COLUMN INFOS FROM ('\\S+')"),replace);

    /*  Get constraint information                */
    /*  TFSQL Command: SELECT CONTRAINT INFO FROM */
    switch (settings->getDbTyp())
    {
    case DB::PostgreSQL: replace="SELECT DISTINCT ordinal_position, 0, ccu.table_name AS references_table, kcu.column_name, ccu.column_name AS references_field, rc.update_rule AS on_update, rc.delete_rule AS on_delete, match_option FROM information_schema.table_constraints tc LEFT JOIN information_schema.key_column_usage kcu ON tc.constraint_catalog = kcu.constraint_catalog AND tc.constraint_schema = kcu.constraint_schema AND tc.constraint_name = kcu.constraint_name LEFT JOIN information_schema.referential_constraints rc ON tc.constraint_catalog = rc.constraint_catalog AND tc.constraint_schema = rc.constraint_schema AND tc.constraint_name = rc.constraint_name LEFT JOIN information_schema.constraint_column_usage ccu ON rc.unique_constraint_catalog = ccu.constraint_catalog AND rc.unique_constraint_schema = ccu.constraint_schema AND rc.unique_constraint_name = ccu.constraint_name WHERE tc.table_name=\\1 AND tc.constraint_type='FOREIGN KEY'"; break;
    case DB::SQLite: replace="PRAGMA foreign_key_list(\\1)"; break;
    }
    query.replace(QRegExp("SELECT CONTRAINT INFO FROM ('\\S+')"),replace);

    /*  Varchar                         */
    /*  TFSQL Command: varchar(length)  */
    switch (settings->getDbTyp())
    {
    case DB::PostgreSQL: replace="character varying(\\1)"; break;
    case DB::SQLite: replace="varchar"; break;
    }
    query.replace(QRegExp("varchar\\((\\d+)\\)"),replace);

    /*  Create PK field                 */
    /*  TFSQL Command: PKFIELD */
    switch (settings->getDbTyp())
    {
    case DB::PostgreSQL: replace="serial"; break;
    case DB::SQLite: replace="integer"; break;
    }
    query.replace("PKFIELD",replace);

    return query;
}

QString TFSqlQuery::intListToString(QList<int> list)
{
    QString listString;
    for (int i=0;i<list.size();i++) {
        listString += QString::number(list.at(i));
        if (i<list.size()-1) listString += ",";
    }
    return listString;
}
