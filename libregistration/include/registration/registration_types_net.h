#ifndef ESU_REGISTRATION_TYPES_NET
#define ESU_REGISTRATION_TYPES_NET

#ifndef ESU_NET_PROTO_SERVER

#include "global1.h"
#include <QtCore/qlist.h>
#include <QtCore/qhash.h>
#include <QtNetwork/qhostaddress.h>
#include <QQueue>

#include "registration_data.h"


struct RegistrationPackageDataNET
{
public:
    RegistrationPackageDataNET() {
        enable = false;
        registered = false;
        birthDate = rank = time = -1;
    }

    RegistrationPackageDataNET(const RegistrationUserInfo& o) {
        *this = o;
    }

    RegistrationPackageDataNET& operator =(const RegistrationUserInfo& o) {
        enable = true;
        role = o.role;
        netAddress = o.address;
        name = o.name;
        time = o.time;

        rank = o.rank;
        birthDate = o.birthDate;
        streetAddress = o.streetAddress;
        militarySpeciality = o.militarySpeciality;

        return(*this);
    }

public:
    bool            enable;
    QString         role;               // Должность по таблице иерархии
    QHostAddress    netAddress;
    bool            registered;         // Статус регистрации
    QString         militarySpeciality; // ВУС
    QString         name;               // ФИО
    int             birthDate;          // Дата рождения
    QString         streetAddress;      // Адрес (место жительства)
    int             rank;               // Звание
    int             time;

};


struct RegistrationPackageNET
{
public:
    RegistrationPackageNET(int msg = 0, int _error = -1, int _success = -1):
        msgType(msg), error(_error), success(_success)
    {
        __init();

        userData.enable = false;
        userData.registered = false;
        records.enable = false;
        userData.time = 0;
    }

    RegistrationPackageNET(const RegistrationPackageNET& p) {
        *this = p;
    }

    RegistrationPackageNET& operator =(const RegistrationPackageNET& p) {
        if( this == &p ) return(*this);

        msgType = p.msgType; error = p.error; success = p.success;
        sender = p.sender; address = p.address;

        if( !p.data.isEmpty() ) data = p.data;
        if( !p.message.isEmpty() ) message = p.message;

        if( p.userData.enable ) {
            userData.enable = true;
            userData.role = p.userData.role;
            userData.registered = p.userData.registered;
            userData.name = p.userData.name;

            userData.militarySpeciality = p.userData.militarySpeciality;
            userData.birthDate = p.userData.birthDate;

            userData.netAddress = p.userData.netAddress;
            userData.streetAddress = p.userData.streetAddress;
            userData.rank = p.userData.rank;

            userData.time = p.userData.time;
        }

        if( p.records.enable ) {
            records.enable = true;
            records.recordsList = p.records.recordsList;
        }

        return( *this );
    }

protected:
    void __init()
    {
        userData.enable = false;
        userData.registered = false;
        userData.birthDate = -1;
        userData.rank = -1;
        userData.time = -1;
    }

public:
    inline bool hasError() const { return( error > -1 ); }

    inline bool isSuccess() const { return(success == 1); }
    inline void setSuccess(bool _success = true) {
        success = ( _success )?(1):(0);
    }

    inline bool hasMessage() const { return( !message.isEmpty() ); }
    inline bool hasUserData() const { return( userData.enable ); }

    inline bool hasUserRole() const { return( !userData.role.isEmpty() ); }
    inline bool hasUserName() const { return( !userData.name.isEmpty() ); }
    inline bool hasUserSpeciality() const { return( !userData.militarySpeciality.isEmpty() ); }
    inline bool hasStreetAddress() const { return( !userData.streetAddress.isEmpty() ); }
    inline bool hasUserRank() const { return( userData.rank >= 0 ); }
    inline bool hasUserBirthday() const { return( userData.birthDate > 0 ); }

    inline void addRecord(const RegistrationPackageDataNET& record) {
        records.enable = true;
        records.recordsList.append( record );
    }

    inline void setUserData(bool state = true) {
        userData.enable = state;
    }

    inline void setUserData(const QString& _role, const QString& _name = QString(), bool _registered = false, int _time = -1 ) {
        userData.enable = true;
        userData.role = _role;
        userData.name = _name;
        userData.registered = _registered;
        if( _time <= 0 ) _time = std::time(nullptr);
    }

    inline void setRawData( const QByteArray& _data ) {
        data = _data;
    }

public:
    int	msgType;  // Тип сообщения (перечисление)
    int error;    // Код ошибки (0 - нет ошибок; -1 - поле не используется)
    int success;  // Флаг успеха операции(например, при успешной регистрации): 1 - успех, 0 - неудача, -1 - поле неиспользуется
    QByteArray data; // Raw-данные
    QString message;

    RegistrationPackageDataNET userData;

    struct {
        bool enable;
        QList<RegistrationPackageDataNET> recordsList;

    } records;

    quint32     sender;
    QString     address;
};

typedef QQueue<RegistrationPackageNET> RegistrationDataQueueNET;

Q_DECLARE_METATYPE(RegistrationPackageNET)

#endif // ESU_NET_PROTO_SERVER

#endif // ESU_REGISTRATION_TYPES_NET


