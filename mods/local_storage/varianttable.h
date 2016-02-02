#ifndef VARIANTTABLE_H
#define VARIANTTABLE_H

#include <QString>
#include <QSqlDatabase>
#include <QVariant>

#include "abstractlocalstoragetable.h"

/* Инструкция:
 * Этот тип таблиц позволяет хранить структуры типа такой:
 * tree_object { x = 60898756534, y = 90765432321, id = 42, type = "елка", tags = ['зеленая','шишки','новый год'] }
 * Для работы с таблицей активно используется тип словаря QVariantMap который равен QMap< QString, QVariant>
 *
 * Перед созданием таблицы нужно создать её чертёж (blueprint). Чертеж таблицы это QVariantMap в котором
 * ключи задают имена полей, а значения - это строки, описывающие данные поля. Значения строк
 *  - "" (пустая строка) - обычное поле - число, строка, QByteArray.
 *  - "unique" - это поле имеет различное значение у каждого объекта
 *  - "binary" - это поле хранить запакованным. Обязательно для массивов, вложенных структур и подобного.
 * Для указанного примера blueprint создается так
 * QVariantMap bp;
 * bp["x"] = "";
 * bp["y"] = "";
 * bp["id"] = "unique";
 * bp["type"]="";
 * bp["tags"]="binary";
 *
 * Чтобы использовать таблицу надо получить указатель на нее вот так:
 *      auto table_ptr = LocalStorageManager::inst().getVariantTable("objects", blueprint);
 * где  objects - уникальное имя таблицы. Имена разных не должны совпадать.
 * Имя должно быть одним словом из маленьких англ. букв, цифр и _  начинаясь с буквы.
 * В дальнейшем указатель можно передавать и копировать, но лучше просто получить снова так как он кешируется.
 * Если достоверно известно, что эту таблицу с этим именем уже вызывали или она есть в базе, то параметр blueprint можно
 * опустить. Если он всё же присутствует, то будет сравнен с тем, что хранится в базе, и в случае несовпадения выдаст ошибку.
 * Если попытаться открыть таблицу не с тем blueprint, с которым она создана, возникнет ошибка. Можно воспользоваться
 * функцией LocalStorageManager::inst().getBlueprintByName() чтобы получить blueprint таблицы не открывая её.
 *
 * Чтобы добавить значение в таблицу, снова используй QVariantMap. Помещение в поле значения пустой QVariant()
 * позволяет явным образом указать, что поле не используется. (QVariant() не равен нулю и пустой строке.)
 * QVariantMap value;
 * value["x"] = 60898756534;
 * value["y"] = 90765432321;
 * value["id"] = 42;
 * value["tags"]= QList();
 *
 * Затем используй insert(). Если таблица содержит unique поля и добавляемое значение конфликтует с имеющимся,
 *  то старое значение будет заменено новым. Иначе значение будет добавлено в конец таблицы.
 *
 * Для получения значений используется функция select(). Она получает в качестве аргумента условие, записанное текстовой
 * строкой. Поддерживаются операторы == != < >  >= <= AND OR BETWEEN IN NOT. Поддерживаются скобки. Примеры условий
 * x >= 100 AND y <= 100
 * (x = 7 AND x = 8) OR x = 9
 * type IS NULL //NULL значит пустой QVariant() а не ноль.
 * x IS NOT NULL
 * id IN (12,13,14)
 * id NOT IN (12,14)
 * (x BETWEEN 10 AND 30) AND (y BETWEEN 40 AND 60)
 * type LIKE '%а' // вернет "елка" "пихта" "липа". Используются % как много символов и _ как один символ.
 * Функция возвращает массив результатов, которые подходят под все условия. Под условие "" (пустая строка или пробел)
 * считаются подходящими все элементы. Нельзя искать по полям объявленным как binary - результат непредсказуем.
 *
 * Для удаления значений есть функция remove(). Она принимает такие же условия как и select.
 *
 * Для гарантии надежности или для скорости если собираешься добавить подряд более трех значений,
 * нужно использовать метод lock(). Вызови его до внесения изменений и все изменения будут храниться в памяти.
 * Дальше нужно вызвать либо commit() чтобы сохранить изменения на диск, либо rollback() чтобы отменить их.
 * Если забудешь одно из двух, будут невообразимые глюки.
 *
 */

class VariantTable : public AbstractLocalStorageTable
{
    friend class LocalStorageManager;

public:
    QVariantMap getBlueprint() {return blueprint;}

    void insert(QVariantMap value);    
    bool updateRecord(const QVariantMap& value, const QString& condition);

    QList<QVariantMap> select(QString condition);

    bool containsRecord(const QString& condition);
    bool remove(QString condition);

private:
    VariantTable(QString nname, QSqlDatabase ndb, QVariantMap nblueprint);
    VariantTable(const VariantTable&);
    VariantTable& operator=(const VariantTable&);

    void createTable(); //Создает соответствующую таблицу в базе данных.
    void dropTable();   // Удаляет таблицу из БД
    void clearTable();  // Удаляет все записи из таблицы

private:
    QVariantMap blueprint;
};

#endif // VARIANTTABLE_H
