#include <QApplication>

#include "global1.h"

#include <staff/esu_staff_system.h>
#include <registration/esu_registration.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    esuStaff->loadData();
//    esuReg->ui()->createModels();

    esuReg->initialization();

    StaffDivision root = esuStaff->data().rootDivision();
    qDebug() << root.nameFull();

    esuStaff->printDivisions();

    foreach (StaffDivision div, root.divisions()) {
        //        qDebug() << div.nameFull();
        qDebug() <<"-"<< div.name();

        foreach (StaffDivision div2, div.divisions()) {
            //            qDebug() << div2.nameFull();
            qDebug() <<"--"<< div2.name();
            //            qDebug() <<"--"<< div2.nameBase();
            qDebug() <<"--"<< div2.nameDecription();
            //            qDebug() <<"--"<< div2.decription();
            //            qDebug() <<"--"<< div2.note();

//            qDebug() <<"--"<< div2.toBaseUnit().dutyName();
//            qDebug() <<"--"<< div2.toCommander().dutyName();
//            //            qDebug() <<"--"<< div2.toDivision().dutyName();
//            qDebug() <<"--"<< div2.toSoldier().dutyName();
//            //            qDebug() <<"--"<< div2.toObject().dutyName();
        }
    }

    return a.exec();
}
