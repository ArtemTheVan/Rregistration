#include "abstractlocalstoragetable.h"

#include <QVariant>
#include <QByteArray>

QByteArray AbstractLocalStorageTable::serializeData;
QDataStream AbstractLocalStorageTable::dsr(&AbstractLocalStorageTable::serializeData,QIODevice::ReadOnly);
QDataStream AbstractLocalStorageTable::dsw(&AbstractLocalStorageTable::serializeData,QIODevice::WriteOnly);


AbstractLocalStorageTable::AbstractLocalStorageTable(QString nname, QSqlDatabase ndb)
{
    name = nname;
    db=ndb;
}

QVariant AbstractLocalStorageTable::readSerialized(QByteArray data)
{
    QDataStream dsr(&data,QIODevice::ReadOnly);
    QVariant result;
    dsr >> result;
    return result;
}

QByteArray AbstractLocalStorageTable::writeSerialized(QVariant data)
{
    QByteArray result;
    QDataStream dsw(&result,QIODevice::WriteOnly);
    dsw << data;
    return result;
}


