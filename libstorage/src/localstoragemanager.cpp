#include "localstoragemanager.h"
#include <QSqlQuery>
#include <QSqlDriver>
#include <QVariant>
#include <QDebug>

const QString LOCALSTORAGE_SYSTEM_TABLE_NAME = "localstorage_system_table_list";

// [ INIT/DESTROY ]: {{{

LocalStorageManager::LocalStorageManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("localstorage.db");
    bool db_ok = db.open();
    if (!db_ok)
        qFatal("LocalStorage: can not open database file, it is corrupt or has permission problem, or something.");

}

LocalStorageManager::~LocalStorageManager()
{

}

// }}}

bool LocalStorageManager::initializeVariantTable(QString name, QVariantMap blueprint)
{
    Q_ASSERT (variants.contains(name) == false);
    bool mustCreateTable = false;

    //Сначала проверяю, существует ли такая таблица и какой у неё blueprint;
    DictionaryTable* tableDict = getDictionary(LOCALSTORAGE_SYSTEM_TABLE_NAME);

    QVariant tableType = tableDict->read(name,true);

    if (tableType.isValid() == false) {
        //Таблица не существует - создаю;
        mustCreateTable = true;
    }

    //Если таблица существует то проверяю тип  и blueprint
    else if (tableType.toString() == QString("dict")) {
        qCritical("Requested a variable table =%s= that is already created "
                  "as a dictionary table", qPrintable(name) );
        return false;
    }
    else if (tableType.toMap() != blueprint) {
        //TODO Обработать случай, когда новый blueprint расширяет старый
        qCritical("Requested a variable table =%s= that already exists in "
                  "database, but with a different template. " ,
                  qPrintable(name));
        return false;
    }
    else {
        //Таблица существует и валидна - ничего не делаю
    }
#ifdef QT4_ETU
    VariantTable* t (new VariantTable(name,db,blueprint));
#else
    std::shared_ptr<VariantTable> t (new VariantTable(name,db,blueprint));
#endif
    variants.insert(name, t);
    if (mustCreateTable) {
        t->createTable();
        tableDict->insert(name,QVariant(blueprint),true);
    }
    return true;
}


bool LocalStorageManager::tableExists(QString tablename)
{
    QSqlQuery query(db);
    query.prepare("SELECT name FROM sqlite_master "
                  "WHERE type='table' AND name=:tablename;");
    query.bindValue(":tablename", QVariant(tablename));
    query.exec();
    switch (query.size()) {
    case -1:
        qFatal("LocalStorage: Query size not supported! Everything goes wrong!");
    case 0:
        return false;
    default:
        return true;
    }

}


bool LocalStorageManager::clearTable(const QString &tablename)
{
    if( variants.contains(tablename) ) {
#ifdef QT4_ETU
        VariantTable* t = variants[tablename];
#else
        std::shared_ptr<VariantTable> t = variants[tablename];
#endif
        t->clearTable();
        return( true );
    }

    return( false );
}

bool LocalStorageManager::removeTable(const QString &tablename)
{
    if( variants.contains(tablename) ) {
#ifdef QT4_ETU
        VariantTable* t = variants[tablename];
        DictionaryTable* tableDict = getDictionary(LOCALSTORAGE_SYSTEM_TABLE_NAME);
#else
        std::shared_ptr<VariantTable> t = variants[tablename];
        auto tableDict = getDictionary(LOCALSTORAGE_SYSTEM_TABLE_NAME);
#endif
        t->dropTable();
        variants.remove(tablename);
        tableDict->removeRecord(tablename);

        return( true );
    }

    return( false );
}


QVariantMap LocalStorageManager::getBlueprintByName(QString name)
{
#ifdef QT4_ETU
    DictionaryTable* tableDict = getDictionary(LOCALSTORAGE_SYSTEM_TABLE_NAME);
#else
    auto tableDict = getDictionary(LOCALSTORAGE_SYSTEM_TABLE_NAME);
#endif
    return tableDict->read(name).toMap();

}

#ifdef QT4_ETU
DictionaryTable* LocalStorageManager::getDictionary(QString name)
{
    /* if (variants.contains(name))
        throw std::logic_error("Requested a Localstorage dictionary by the name of VariantMap");*/
    if (!dictionaries.contains(name)) {
        DictionaryTable* t = (new DictionaryTable(name,db));
        dictionaries.insert(name, t);
        DictionaryTable* tableDict = getDictionary(LOCALSTORAGE_SYSTEM_TABLE_NAME);
        tableDict->insert(name,"dict");
    }
    return dictionaries[name];
}
#else
std::shared_ptr<DictionaryTable> LocalStorageManager::getDictionary(QString name)
{
   /* if (variants.contains(name))
        throw std::logic_error("Requested a Localstorage dictionary by the name of VariantMap");*/
    if (!dictionaries.contains(name)) {
        std::shared_ptr<DictionaryTable> t(new DictionaryTable(name,db));
        dictionaries.insert(name, t);
        auto tableDict = getDictionary(LOCALSTORAGE_SYSTEM_TABLE_NAME);
        tableDict->insert(name,"dict");
    }
    return dictionaries[name];
}
#endif

#ifdef QT4_ETU
VariantTable* LocalStorageManager::getVariantTable(
#else
std::shared_ptr<VariantTable> LocalStorageManager::getVariantTable(
#endif
        QString name, QVariantMap blueprint)
{
    if (!variants.contains(name)) {
        initializeVariantTable(name, blueprint);
    }
    return variants[name];
}

#ifdef QT4_ETU
VariantTable* LocalStorageManager::getVariantTable(QString name)
#else
std::shared_ptr<VariantTable> LocalStorageManager::getVariantTable(QString name)
#endif
{
    if(!variants.contains(name)) {
        qCritical("Requested a new VariantTable without providing blueprint");
    }
    return variants[name];
}

// [ CLEAR DATA ]: {{{

void LocalStorageManager::clearAllData()
{
    if( variants.isEmpty() ) return;

    QStringList keys = variants.keys();

    foreach (const QString &key, keys) {
        removeTable(key);
    }
}

// }}}
