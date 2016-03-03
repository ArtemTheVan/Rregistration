#include <QCoreApplication>
#include <QDebug>
#include <QList>
#include <QString>
#include "localstoragemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /*auto dragons = LocalStorageManager::inst().getDictionary("dragons");

     Here be dragons. Примеры использования localstorage

    dragons->insert("blue", "stupid",true);
    dragons->insert("orange", 42, true);
    dragons->insert("black",36,true);
    dragons->remove("blue");

    auto flagons = LocalStorageManager::inst().getDictionary("dragons");

    flagons->insert("blue", 7, true);
    flagons->insert("brown", "an ass");
    qDebug() << dragons->read("blue",true) << dragons->read("orange",true);*/

    // Примеры использования VariantTable
    QVariantMap dragonBlueprint;
    dragonBlueprint["color"] = "binary";
    dragonBlueprint["name"] = "unique";
    dragonBlueprint["IQ"] = "binary";
    dragonBlueprint["egg"] = "binary";



    auto v2dragons = LocalStorageManager::inst().getVariantTable("vdragons",dragonBlueprint);

    QVariantMap black;
    black["color"] = "black";
    black["name"] = "Бывалый";
    black["IQ"] = 50;

    QVariantMap blackegg;
    blackegg["shape"] = "round";

    black["egg"] = blackegg;

    QVariantMap blue;
    blue["color"] = "blue";
    blue["name"] = "Пуся";
    blue["IQ"] = 30;

    v2dragons->insert(black);
    v2dragons->insert(blue);

    QList<QVariantMap> black1 = v2dragons->select("name = 'Бывалый'");
    QVariantMap blackegg2 = black1[0].value("egg").toMap();
    auto blue1 = v2dragons->select("name <> 'Бывалый'");

    qDebug() << blackegg2;


    //return a.exec();
}
