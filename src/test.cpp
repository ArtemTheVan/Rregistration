//todo this macros for qt4 and etu
//#ifdef QT4_ETU

#include <staff/staff_system.h>
#include <registration/registration.h>
#include <registration/registration_engine.h>
#include <netdevice/netdevice.h>
#include "test.h"


void Test::showCurrentMessage()
{
    QString msg = esuReg->currentMessage();

    qDebug() <<__PRETTY_FUNCTION__<<msg;
}

void Test::run(int id, int val)
{
    qDebug() <<__PRETTY_FUNCTION__;
    qDebug() <<id;
    qDebug() <<val;

    if(id!=3)
        return;
    if(val!=1)
        return;

    NetDevice *netdev = NetDevice::GetNetDevice();

    NetRegistrator *netReg = netdev->getRegistrator(0);

    if(netReg==NULL)
        return;

    qDebug() <<__PRETTY_FUNCTION__<<" selfIp "<< netReg->selfIp;

    esuStaff->loadData();
    //    esuReg->ui()->createModels();

    //    QString selfIp = "FE80::280:E102:1A:CA33%radio0";
    QString selfIp = netReg->selfIp;//"FE80::280:E102:1A:CA33";
    esuReg->initialization(selfIp); //передать в аргументе свой ip

    StaffDivision root = esuStaff->data().rootDivision();
    //    void emitsendRegistrationMsg(RegistrationPackageNET, const QStringList &addrs=QStringList());

    qDebug() << root.nameFull();

    esuReg->setChoosedRole("Н.1-МСО.1-МСВ");
    esuReg->setChoosedName("Петрович");
    esuReg->registerUser();

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

    //    esuReg->registerUser();

}

//#endif
