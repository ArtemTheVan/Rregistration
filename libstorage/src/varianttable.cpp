#include "varianttable.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>


void VariantTable::insert(QVariantMap value)
{
    QStringList col_names;
    QStringList col_marks;
    QVariantList col_values;
    QString keyVal;

    foreach (const QString &key, value.keys()) {
        if (!blueprint.contains(key)) {
            qCritical("Localstorage:  variant table %s. Tried to insert an object that does not match the blueprint.",
                      qPrintable(name));
            return;
        }
        col_names.append(key); //TODO type check
        keyVal = blueprint.value(key).toString();

        if( keyVal == "binary" || keyVal.contains("BLOB", Qt::CaseInsensitive) ) {
            QVariant data (writeSerialized(value.value(key)));
            col_values.append(data);
        }
        else col_values.append(value.value(key));
        col_marks.append("?");
    }

    QString cmd("INSERT OR REPLACE INTO %1 (%2) VALUES (%3)");
    cmd =  cmd.arg(name)
            .arg(col_names.join(", "))
            .arg(col_marks.join(", "));

    QSqlQuery query(db);
    query.prepare(cmd);
    foreach (QVariant col_value, col_values) {
        query.addBindValue(col_value);
    }
    qDebug() << cmd << query.exec();
}


bool VariantTable::updateRecord(const QVariantMap &value, const QString &condition)
{
    QVariantList col_values;

    if( condition.trimmed().isEmpty() ) return( false );

    QString cmd = QString("UPDATE %1 SET ").arg(name);
    QString val_str, valueKey;

    foreach (const QString& key, value.keys()) {
        if (!blueprint.contains(key)) {
            qCritical("Localstorage:  variant table %s. Tried to insert an object that does not match the blueprint.",
                      qPrintable(name));
            return( false );
        }
        val_str = QString("%1=?, ").arg(key);
        valueKey = blueprint.value(key).toString();

        if( valueKey.contains("binary", Qt::CaseInsensitive) || valueKey.contains("BLOB", Qt::CaseInsensitive) )
        {
            QVariant data (writeSerialized(value.value(key)));
            col_values.append(data);
        }
        else col_values.append(value.value(key));
        cmd.append(val_str);
    }
    cmd.remove( cmd.size() - 2, 2 );
    cmd.append(" WHERE " + condition + ";");

    QSqlQuery query(db);
    query.prepare(cmd);
    foreach (QVariant col_value, col_values) {
        query.addBindValue(col_value);
    }

    qDebug() << cmd;
    bool r = query.exec();
    return( r );
}


QList<QVariantMap> VariantTable::select(QString condition)
{
    QString valueKey;
    QList<QVariantMap> result;
    QStringList col_names = blueprint.keys();
    if (condition.trimmed().isEmpty() == false)
        condition.prepend("WHERE ");
    QString cmd("SELECT %2 FROM %1 %3 ");
    cmd = cmd.arg(name,col_names.join(", "),condition);

    qDebug() << cmd;
    QSqlQuery query(db);
    query.exec(cmd);

    while (query.next())
    {
        QVariantMap entry;
        foreach (QString col_name, col_names)
        {
            valueKey = blueprint.value(col_name).toString();
            QVariant actual_value;

            if( valueKey.contains("binary", Qt::CaseInsensitive) || valueKey.contains("BLOB", Qt::CaseInsensitive) )
            {
#if QT_VERSION >= 0x050000
                actual_value = readSerialized(QVariant(query.value(col_name)).toByteArray());
#else
                actual_value = readSerialized(QVariant(query.record().indexOf(col_name)).toByteArray());
#endif
            } else
            {
#if QT_VERSION >= 0x050000
                actual_value = query.value(col_name);
#else
                actual_value = query.record().indexOf(col_name);
#endif
            }
            entry.insert(col_name, actual_value);
        }
        result.append(entry);
    }
    return result;
}


bool VariantTable::containsRecord(const QString &condition)
{
    if( condition.trimmed().isEmpty() ) return( false );

    QStringList col_names = blueprint.keys();
    QString cmd("SELECT %2 FROM %1 WHERE %3 ");
    cmd = cmd.arg(name, col_names.join(", "), condition);
    qDebug() << cmd;
    QSqlQuery query(db);
    query.exec(cmd);

    if( query.size() > 0 )
        return( true );

    return( false );
}


bool VariantTable::remove(QString condition)
{
    if (condition.trimmed().isEmpty() == false)
        condition.prepend("WHERE ");
    QString cmd ("DELETE FROM %1 %2");
    cmd = cmd.arg(name, condition);
    QSqlQuery query(db);

    return( query.exec(cmd) );
}



VariantTable::VariantTable(QString nname, QSqlDatabase ndb, QVariantMap nblueprint):
    AbstractLocalStorageTable(nname, ndb)
{
    blueprint = nblueprint;
}

void VariantTable::createTable()
{
    QString keyVal;
    QStringList keys = blueprint.keys();

    foreach (QString key, keys)
    {
        keyVal = blueprint.value(key).toString();

        if( keyVal.contains("unique", Qt::CaseInsensitive) ) {
            key = key + " UNIQUE";
        }
        else if( keyVal.contains("primary", Qt::CaseInsensitive) ) {
            key = key + " NOT NULL PRIMARY KEY";
        }
        else if( keyVal.contains("binary", Qt::CaseInsensitive) )
            key = key + " BLOB";
    }
    QString base_command = QString("CREATE TABLE %1 (%2)").arg(name);
    QString fields = keys.join(", ");
    QString command = base_command.arg(fields);
    qDebug() << command;

    QSqlQuery query(db);
    query.exec(command);

}


void VariantTable::dropTable()
{
    QString command = QString("DROP TABLE %1").arg(name);
    QSqlQuery query(db);
    query.exec(command);
}


void VariantTable::clearTable()
{
    QString command = QString("TRUNCATE TABLE %1").arg(name);
    QSqlQuery query(db);
    query.exec(command);
}
