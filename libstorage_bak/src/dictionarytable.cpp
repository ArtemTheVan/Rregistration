#include "dictionarytable.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDataStream>
#include <QStringList>


DictionaryTable::DictionaryTable(QString nname, QSqlDatabase ndb) :
    AbstractLocalStorageTable(nname, ndb)
{
    createTable();
}

void DictionaryTable::createTable()
{
    QSqlQuery query(db);
    QString cmd = QString("CREATE TABLE IF NOT EXISTS %1 (key PRIMARY KEY NOT NULL, value BLOB)").arg(name);
    query.exec(cmd);
}

QVariant DictionaryTable::read(QString key, bool binary)
{
    QVariant* cached = memoryCache.object(key);
    if (cached != 0) return *cached;
    QSqlQuery query(db);
    query.setForwardOnly(true);
    query.prepare(QString("SELECT value FROM %1 WHERE key = ?").arg(name));
    query.bindValue(0, key);
    query.exec();

    bool found = query.next();
    if (!found) return QVariant();

    if (binary) cached = new QVariant(readSerialized(query.value(0).toByteArray()));
    else cached = new QVariant(query.value(0));
    memoryCache.insert(key, cached);
    return *cached;
}

QString DictionaryTable::searchByValue(QVariant value)
{
    QSqlQuery query(db);
    query.setForwardOnly(true);
    query.prepare(QString("SELECT key FROM %1 WHERE value = ?").arg(name));
    query.bindValue(0, value);
    query.exec();
    bool found= query.next();
    if (found) {
        QString key = query.value(0).toString();
        memoryCache.insert(key, new QVariant(value));
        return key;
    }
    else return QString();
}

QStringList DictionaryTable::multiSearchByValue(QVariant value)
{
    QSqlQuery query(db);
    query.setForwardOnly(true);
    query.prepare(QString("SELECT key FROM %1 WHERE value = ?").arg(name));
    query.bindValue(0, value);
    query.exec();
    QStringList result;
    while (query.next()) {
        QString key = query.value(0).toString();
        result.append(key);
        memoryCache.insert(key, new QVariant(value));
    }
    return result;

}

void DictionaryTable::remove(QString key)
{
    memoryCache.remove(key);
    QSqlQuery query(db);
    query.prepare(QString("DELETE FROM %1 WHERE key = ?").arg(name));
    query.bindValue(0, key);
    query.exec();
}

QStringList DictionaryTable::fetchKeyList()
{
    QSqlQuery query(db);
    query.prepare(QString("SELECT key from %1").arg(name));
    query.exec();
    QStringList result;
    while (query.next()) { result.append(query.value(0).toString()); }
    return result;
}

void DictionaryTable::clearAll(bool recreate)
{
    clearMemoryCache();
    QSqlQuery query(db);
    if (recreate) {
        query.exec(QString("DROP TABLE %1").arg(name));
        createTable();
    } else
        query.exec(QString("DELETE FROM %1").arg(name));
}

void DictionaryTable::setMemoryCacheSize(int items)
{
    memoryCache.setMaxCost(items);
}

void DictionaryTable::clearMemoryCache()
{
    memoryCache.clear();
}

void DictionaryTable::insert(QString key, QVariant value, bool binary)
{
    memoryCache.insert(key, new QVariant(value));
    QSqlQuery query(db);
    query.prepare(QString("INSERT OR REPLACE INTO %1 VALUES (?, ?)").arg(name));
    query.bindValue(0, key);
    if (binary) {
        QByteArray data = writeSerialized(value);
        query.bindValue(1, data, QSql::In | QSql::Binary);
    } else {
        query.bindValue(1, value);
    }
    query.exec();
}

void DictionaryTable::removeRecord(QString key)
{
    if( memoryCache.contains(key) ) {
        memoryCache.remove(key);

        QSqlQuery query(db);
        query.prepare(QString("DELETE FROM %1 WHERE key = ?").arg(name));
        query.bindValue(0, key);
        query.exec();
    }
}
