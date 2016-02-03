#ifndef DICTIONARYTABLE_H
#define DICTIONARYTABLE_H

#include <QString>
#include <QSqlDatabase>
#include <QVariant>
#include <QCache>

#include "abstractlocalstoragetable.h"

/* Инструкция:
 * Чтобы использовать таблицу надо получить указатель на нее вот так:
 *      auto table_ptr = LocalStorageManager::inst().getDictionary("table");
 * где  table - уникальное имя таблицы. Имена таблиц разных типов не должны совпадать.
 * Имя должно быть одним словом из маленьких англ. букв, цифр и _  начинаясь с буквы.
 * В дальнейшем указатель можно передавать и копировать, но лучше просто получить снова так как он кешируется.
 *
 * Для гарантии надежности или для скорости если собираешься добавить подряд более трех значений,
 * нужно использовать метод lock(). Вызови его до внесения изменений и все изменения будут храниться в памяти.
 * Дальше нужно вызвать либо commit() чтобы сохранить изменения на диск, либо rollback() чтобы отменить их.
 * Если забудешь одно из двух, будут невообразимые глюки.
 *
 * Чтобы поместить значение в таблицу используй insert(), чтобы прочитать - read().
 * Параметр binary должен быть false для строк, чисел и QByteArray, true для массивов, структур и подобных случаев.
 * Если insert()  использовался с binary = true, то и read() тоже должен быть таким, иначе при чтении получишь мусор.
 *
 */

class DictionaryTable : public AbstractLocalStorageTable
{
    friend class LocalStorageManager;
public:

    void insert(QString key, QVariant value, bool binary = false); //Добавить в таблицу, заменить имеющееся значение
    void removeRecord(QString key);

    QVariant read(QString key, bool binary = false); //Получить значение по ключу

    QString searchByValue (QVariant value); //Найти ключ по значению. Медленно и не факт, что найдет.
    QStringList multiSearchByValue (QVariant value); //Найти все ключи по значению. Ещё медленнее
    void remove(QString key); //Удалить запись по ключу
    QStringList fetchKeyList(); //Список всех ключей в таблице
    void clearAll(bool recreate=false); // Стереть все записи из таблицы.
    void setMemoryCacheSize(int items);
    void clearMemoryCache();

private:
    DictionaryTable(QString name, QSqlDatabase ndb);
    DictionaryTable (const DictionaryTable&);
    DictionaryTable& operator=(const DictionaryTable&);
    void createTable();

private:
    QCache<QString, QVariant> memoryCache;
};

#endif // DICTIONARYTABLE_H
