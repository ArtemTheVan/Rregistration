#ifndef ABSTRACTLOCALSTORAGETABLE_H
#define ABSTRACTLOCALSTORAGETABLE_H

#include <QSqlDatabase>
#include <QDataStream>

class AbstractLocalStorageTable
{
public:
    bool lock()
    {return db.transaction();}
    bool commit()
    {return db.commit();}
    bool rollback()
    {return db.rollback();}

protected:
    AbstractLocalStorageTable(QString nname, QSqlDatabase ndb);

    QString name;
    QSqlDatabase db;

    static QByteArray serializeData;
    static QDataStream dsw;
    static QDataStream dsr;

    static QVariant readSerialized(QByteArray data);
    static QByteArray writeSerialized(QVariant data);
};
#endif // ABSTRACTLOCALSTORAGETABLE_H
