#ifndef LOCALSTORAGEMANAGER_H
#define LOCALSTORAGEMANAGER_H

#include <QSqlDatabase>
#include <memory>
#include <dictionarytable.h>
#include <varianttable.h>


#define esuDb LocalStorageManager::inst()

class LocalStorageManager
{
public:
    static LocalStorageManager& inst() {
        static LocalStorageManager singleton;
        return singleton;
    }

    void createDict(QString name);    

    bool tableExists(QString tablename );
    bool clearTable(const QString& tablename);
    bool removeTable(const QString& tablename);

    QVariantMap getBlueprintByName(QString name);
    std::shared_ptr<DictionaryTable> getDictionary(QString name);
    std::shared_ptr<VariantTable> getVariantTable(QString name, QVariantMap blueprint);
    std::shared_ptr<VariantTable> getVariantTable(QString name);

public:
    void clearAllData();

private:
    LocalStorageManager(); //запрещаем все конструкторы ибо нефиг
    ~LocalStorageManager();
    LocalStorageManager(const LocalStorageManager&);
    LocalStorageManager& operator=(const LocalStorageManager&);

    bool initializeVariantTable(QString name, QVariantMap blueprint);

    QSqlDatabase db;
    QHash<QString,std::shared_ptr<DictionaryTable>> dictionaries;
    QHash<QString,std::shared_ptr<VariantTable>> variants;

};

#endif // LOCALSTORAGEMANAGER_H
