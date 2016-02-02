#ifndef ESU_REGISTRATION_ENGINE_TYPES
#define ESU_REGISTRATION_ENGINE_TYPES

#include <QtCore>
#include <QtNetwork/qhostaddress.h>

#include "esu_registration_data.h"
#include "esu_registration_types_net.h"


typedef QList<RegistrationUserInfo> RegistrationTableData;


struct RegistrarInfoSpec
{
public:
    RegistrarInfoSpec(const QHostAddress& addr, const QString& registrarName = QString()):
        address(addr), role(registrarName), enabled(false), main(false)
    { }

    RegistrarInfoSpec(const QString& addr = QString(), const QString& registrarName = QString()):
        address(QHostAddress(addr)), role(registrarName), enabled(false), main(false)
    { }

    RegistrarInfoSpec(const RegistrarInfoSpec& o) {
        if( this == &o ) return;
        address = o.address;
        role = o.role;
        enabled = o.enabled;
        main = o.main;
    }

    ~RegistrarInfoSpec()
    { }

public:
    inline RegistrarInfoSpec& operator =(const RegistrarInfoSpec& o) {
        if( this == &o ) return(*this);
        address = o.address;
        role = o.role;
        enabled = o.enabled;
        main = o.main;
        return( *this );
    }

    friend inline bool operator ==(const RegistrarInfoSpec& l, const RegistrarInfoSpec& r) {
        return( l.address == r.address );
    }

    friend inline bool operator !=(const RegistrarInfoSpec& l, const RegistrarInfoSpec& r) {
        return( l.address != r.address );
    }

public:
    QHostAddress address;
    QString role;
    bool enabled;
    bool main;
};

#endif // ESU_REGISTRATION_ENGINE_TYPES

