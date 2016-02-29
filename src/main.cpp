#include <QApplication>
#include <QStringList>

//#define QT4_ETU
//#define NEW_PROTOBUF

#include "global1.h"
#include "test.h"
#include <registration/registration.h>
#include <registration/registration_engine.h>
#include <netdevice/netdevice.h>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );

#ifdef QT4_ETU
    esuReg->setConfigurationFilePath("./../../conf/registration_config.xml");
#endif

    Test test;

    QString PHOTO_DIR = "";
    QStringList netIfaces;
    netIfaces<<"radio0";
    NetDevice *netdev = NetDevice::GetNetDevice(PHOTO_DIR, &netIfaces);
    //    NetDevice *netdev = new NetDevice("123");
    QObject::connect(netdev, SIGNAL(connectionChanged(int, int)), &test, SLOT(run(int,int)));
    QObject::connect(netdev, SIGNAL(onReceivedData(RegistrationPackageNET)), esuReg->engine(), SLOT(onReceivedData(RegistrationPackageNET)));

    QObject::connect(esuReg->engine(), SIGNAL(emitsendRegistrationMsg(RegistrationPackageNET, QStringList)), netdev, SLOT(emitsendRegistrationMsg(RegistrationPackageNET, QStringList)));
    QObject::connect(esuReg->engine()->tableManager(), SIGNAL(emitsendRegistrationMsg(RegistrationPackageNET, QStringList)), netdev, SLOT(emitsendRegistrationMsg(RegistrationPackageNET, QStringList)));
    QObject::connect(esuReg, SIGNAL(currentMessageChanged()), &test, SLOT(showCurrentMessage()));

    netdev->ctlIface(true, NetDevice::R_Module, "/dev/ttyS0", "115200");


    return a.exec();
}

