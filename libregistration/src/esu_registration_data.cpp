#include "esu_registration_data.h"

#ifdef ESU_NET_SYSTEM
#include "esu_net_system_types.h"
#else
#include "esu_registration_types_net.h"
#endif



RegistrationUserInfo::RegistrationUserInfo(const RegistrationPackageNET &p)
{
    *this = p;
}


RegistrationUserInfo::RegistrationUserInfo(const RegistrationPackageDataNET &p)
{
    *this = p;
}



RegistrationUserInfo& RegistrationUserInfo::operator =(const RegistrationPackageNET& p)
{
    message = p.message;
    address = QHostAddress(p.address);

    role = p.userData.role;
    name = p.userData.name;

    time = p.userData.time;
    timeString = QDateTime::fromTime_t(time).toString("dd.MM.yyyy hh:mm");

    rank = p.userData.rank;
    birthDate = p.userData.birthDate;

    streetAddress = p.userData.streetAddress;
    militarySpeciality = p.userData.militarySpeciality;

    return(*this);
}


RegistrationUserInfo& RegistrationUserInfo::operator =(const RegistrationPackageDataNET& p)
{
    address = p.netAddress;
    role = p.role;
    name = p.name;
    setTime(p.time);
    rank = p.rank;
    birthDate = p.birthDate;
    streetAddress = p.streetAddress;
    militarySpeciality = p.militarySpeciality;

    return(*this);
}

