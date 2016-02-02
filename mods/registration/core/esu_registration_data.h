#ifndef ESU_REGISTRATION_DATA
#define ESU_REGISTRATION_DATA

#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>
#include <QtNetwork/qhostaddress.h>


#include <ctime>


namespace registration {
    enum RequestType { UnknownRequest, RegistrationRequest, ChangeProfileRequest, UpdateProfileRequest };
    enum UpdateFlags { NoFlags = 0x00, RoleFlag = 0x00000001, AddressFlag = 0x00000002, NameFlag = 0x00000004 };
}

struct RegistrationPackageNET;
struct RegistrationPackageDataNET;


struct RegistrationUserInfo
{
public:
    RegistrationUserInfo()
    { rank = 0; birthDate = -1; time = std::time(0); request = registration::UnknownRequest; }

    RegistrationUserInfo(const QString& _role, const QHostAddress& _address, const QString& _name, int _time = 0):
        role(_role), address(_address), name(_name), time(_time), request(registration::UnknownRequest)
      , updateFlags(registration::NoFlags)
    { birthDate = -1; }

    RegistrationUserInfo(const QString& _role, const QString& _address, const QString& _name, int _time = 0):
        role(_role), address(QHostAddress(_address)), name(_name), time(_time), request(registration::UnknownRequest)
      , updateFlags(registration::NoFlags)
    { birthDate = -1; }

    RegistrationUserInfo(const RegistrationUserInfo& o):
        role(o.role), address(o.address), name(o.name), time(o.time), request(o.request)
      , rank(o.rank), duty(o.duty), birthDate(o.birthDate), updateFlags(o.updateFlags)
      , message(o.message), timeString(o.timeString)
      , dutyName(o.dutyName), rankName(o.rankName), streetAddress(o.streetAddress)
      , militarySpeciality(o.militarySpeciality)
      , ui(o.ui)
    { }

    RegistrationUserInfo(const RegistrationPackageNET& p);
    RegistrationUserInfo(const RegistrationPackageDataNET& p);

public:
    inline bool isEmpty() const {
        return( role.isEmpty() && address.isNull() && name.isEmpty() );
    }

    inline bool isValid() const {
        return( !isEmpty() );
    }

    inline bool hasUserRole() const { return( !role.isEmpty() ); }
    inline bool hasUserName() const { return( !name.isEmpty() ); }
    inline bool hasUserSpeciality() const { return( !militarySpeciality.isEmpty() ); }
    inline bool hasStreetAddress() const { return( !streetAddress.isEmpty() ); }
    inline bool hasUserRank() const { return( rank >= 0 ); }
    inline bool hasUserBirthday() const { return( birthDate > 0 ); }

public:
    inline void setName( const QString& firstName, const QString& lastName, const QString& patronymic )
    {
        name = (QString("%1 %2 %3").arg(lastName).arg(firstName).arg(patronymic)).trimmed();
    }

    inline void setTime(int _time = 0) {
        if( _time <= 0 ) {
            time = std::time(0);
        } else time = _time;

        timeString = QDateTime::fromTime_t(time).toString("dd.MM.yyyy hh:mm");
    }

    inline void updateTime() {
        time = std::time(0);
        timeString = QDateTime::fromTime_t(time).toString("dd.MM.yyyy hh:mm");
    }

    inline void updateTimeString() {
        timeString = QDateTime::fromTime_t(time).toString("dd.MM.yyyy hh:mm");
    }

    inline QString formattedTimeString() const {
        QString s = QDateTime::fromTime_t(time).toString("dd.MM.yyyy\nhh:mm");
        return( s );
    }

    void clear() {
        role.clear(); name.clear(); address.clear();
        rank = duty = birthDate = time = 0; request = registration::UnknownRequest;
        updateFlags = registration::NoFlags;
        message.clear(); timeString.clear();

        dutyName.clear();
        rankName.clear();
        streetAddress.clear();
        militarySpeciality.clear();

        ui.checked = false;
    }

    void clearAdditionalData() {
        rank = duty = birthDate = 0; request = registration::UnknownRequest;
        updateFlags = registration::NoFlags;
        message.clear();

        dutyName.clear();
        rankName.clear();
        streetAddress.clear();
        militarySpeciality.clear();

        ui.checked = false;
    }

public:
    inline RegistrationUserInfo& operator =(const RegistrationUserInfo& o) {
        role = o.role; address = o.address; name = o.name;
        time = o.time; request = o.request;

        timeString = QDateTime::fromTime_t(time).toString("dd.MM.yyyy hh:mm");

        rank = o.rank;
        duty = o.duty;
        birthDate = o.birthDate;
        updateFlags = o.updateFlags;
        message = o.message;
        timeString = o.timeString;

        dutyName = o.dutyName;
        rankName = o.rankName;
        streetAddress = o.streetAddress;
        militarySpeciality = o.militarySpeciality;

        ui.checked = o.ui.checked;

        return(*this);
    }

    RegistrationUserInfo& operator =(const RegistrationPackageNET& p);
    RegistrationUserInfo& operator =(const RegistrationPackageDataNET& p);

    friend inline bool operator ==(const RegistrationUserInfo& left, const RegistrationUserInfo& right) {
        return( left.role == right.role && left.address == right.address );
    }

    friend inline bool operator !=(const RegistrationUserInfo& left, const RegistrationUserInfo& right) {
        return !( left == right );
    }

    inline QString addressString() const { return( address.toString() ); }

    inline QString birthDateString() const {
        if( birthDate <= 0 ) return(QString());
        else return( QDateTime::fromTime_t(birthDate).toString("dd.MM.yyyy") );
    }

public:
    QString role;
    QHostAddress address;
    QString name;
    int time;
    registration::RequestType request;

    int rank;
    int duty;
    int birthDate;    
    int updateFlags;

    QString message;
    QString timeString;

    QString dutyName;
    QString rankName;
    QString streetAddress;
    QString militarySpeciality;

    struct {
        bool checked;
    } ui;
};

#endif // ESU_REGISTRATION_DATA

