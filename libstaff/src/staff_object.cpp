#include "staff_object.h"
#include "staff_object_p.h"

#include "staff_base_unit.h"
#include "staff_soldier.h"
#include "staff_commander.h"
#include "staff_division.h"

#include "staff_base_unit_p.h"
#include "staff_soldier_p.h"
#include "staff_commander_p.h"
#include "staff_division_p.h"


// [ INIT/DESTROY ]: {{{

StaffObject::StaffObject()
{
    d_ptr = nullptr;
}


StaffObject::StaffObject(const StaffObject &other)
{
    d_ptr = other.d_ptr;
    if( d_ptr != nullptr ) d_ptr->ref.ref();
}


StaffObject &StaffObject::operator=(const StaffObject &other)
{
    if( d_ptr == other.d_ptr ) return(*this);

    if( d_ptr != nullptr && !d_ptr->ref.deref() ) delete d_ptr;

    if( other.d_ptr != nullptr ) other.d_ptr->ref.ref();
    d_ptr = other.d_ptr;

    return(*this);
}


StaffObject::StaffObject(StaffObjectPrivate *dd)
{
    d_ptr = dd;
    if( d_ptr != nullptr ) d_ptr->ref.ref();
}


StaffObject::~StaffObject()
{
    if( d_ptr != nullptr && !d_ptr->ref.deref() )
        delete d_ptr;
}


StaffObject StaffObject::clone(bool deep) const
{
    if( d_ptr == nullptr )
        return StaffObject();

    return StaffObject(d_ptr->clone(deep));
}


StaffObject StaffObject::copy() const
{
    if( d_ptr == nullptr )
        return StaffObject();

    return StaffObject(((StaffObjectPrivate*)d_ptr)->copy());
}


void StaffObject::create()
{
    if( d_ptr == nullptr ) {
        d_ptr = new StaffObjectPrivate();
        d_ptr->ref.ref();
    }
}


void StaffObject::release()
{
    if( d_ptr != nullptr ) {
#if QT_VERSION >= 0x050000
        if( !d_ptr->ref.deref() || d_ptr->ref.load() <= 1 )
#else
        if( !d_ptr->ref.deref() || int(d_ptr->ref) <= 1 )
#endif
            delete d_ptr;
    }
}

// }}} [ INIT/DESTROY ]
// [ OBJECT PROPERTIES ]: {{{

StaffObject::StaffType StaffObject::staffType() const
{
    if( d_ptr == nullptr )
        return StaffObject::ObjectType;

    return d_ptr->staffType();
}


void StaffObject::setNull()
{
    CHECK_D(d_ptr);

#if QT_VERSION >= 0x050000
    if( d_ptr->ref.load() > 1 ) {
#else
    if( int(d_ptr->ref) > 1 ) {
#endif
        d_ptr->ref.deref();
        d_ptr = nullptr;
#if QT_VERSION >= 0x050000
    } else if( d_ptr->ref.load() == 1 ) {
#else
    } else if( int(d_ptr->ref) == 1 ) {
#endif
        delete d_ptr;
        d_ptr = nullptr;
    }
}


void StaffObject::clear()
{
    CHECK_D(d_ptr);
    Q_D(StaffObject);

    d->clear();
}

// [ CONVERT ]:

//StaffObject StaffObject::toObject() const
//{
//    if( d_ptr != nullptr && !d_ptr->isObject() )
//        return( StaffObject(static_cast<StaffObjectPrivate*>(d_ptr)) );

//    return(*this);
//}


//StaffBaseUnit StaffObject::toBaseUnit() const
//{
//    if( d_ptr != nullptr && (d_ptr->isBaseUnit() || d_ptr->isCommander() || d_ptr->isSoldier()) )
//        return( StaffBaseUnit(static_cast<StaffBaseUnitPrivate*>(d_ptr)) );

//    return( StaffBaseUnit() );
//}


//StaffSoldier StaffObject::toSoldier() const
//{
//    if( d_ptr != nullptr && d_ptr->isSoldier() )
//        return( StaffSoldier(static_cast<StaffSoldierPrivate*>(d_ptr)) );

//    return( StaffSoldier() );
//}


//StaffCommander StaffObject::toCommander() const
//{
//    if( d_ptr != nullptr && d_ptr->isCommander() )
//        return( StaffCommander(static_cast<StaffCommanderPrivate*>(d_ptr)) );

//    return( StaffCommander() );
//}


//StaffDivision StaffObject::toDivision() const
//{
//    if( d_ptr != nullptr && d_ptr->isDivision() )
//        return( StaffDivision(static_cast<StaffDivisionPrivate*>(d_ptr)) );

//    return( StaffDivision() );
//}

// }}} [ OBJECT PROPERTIES ]
// [ PROPERTIES ]: {{{

// [ GET ]: {{

QString StaffObject::nameBase() const
{
    CHECK_D_RET(d_ptr, QString());

    return(d_ptr->nameBase);
}


QString StaffObject::name() const
{
    CHECK_D_RET(d_ptr, QString());

    return(d_ptr->name);
}


QString StaffObject::nameFull() const
{
    CHECK_D_RET(d_ptr, QString());

    return(d_ptr->nameFull);
}


QString StaffObject::nameFullPart() const
{
    CHECK_D_RET(d_ptr, QString());

    return(d_ptr->nameFullPart);
}


QString StaffObject::nameDecription() const
{
    CHECK_D_RET(d_ptr, QString());

    return(d_ptr->nameDescription);
}


QString StaffObject::decription() const
{
    CHECK_D_RET(d_ptr, QString());

    return(d_ptr->description);
}


QString StaffObject::note() const
{
    CHECK_D_RET(d_ptr, QString());

    return(d_ptr->note);
}


int StaffObject::number() const
{
    CHECK_D_RET(d_ptr, 0);

    return(d_ptr->number);
}

QString StaffObject::key() const
{
    return(nameFull());
}

// }} [ GET ]
// [ SET ]: {{

void StaffObject::setDecription(const QString &decription)
{
    CHECK_D(d_ptr);

    if( d_ptr->description != decription )
        d_ptr->description = decription;
}


void StaffObject::setNote(const QString &note)
{
    CHECK_D(d_ptr);

    if( d_ptr->note != note )
        d_ptr->note = note;
}

// }} [ SET ]

// }}} [ PROPERTIES ]
// [ CHECK PROPERTIES ]: {{{

bool StaffObject::isNull() const
{
    return( d_ptr == nullptr );
}


bool StaffObject::isObject() const
{
    CHECK_D_RET(d_ptr, false);

    return( d_ptr->isObject() );
}


bool StaffObject::isBaseUnit() const
{
    CHECK_D_RET(d_ptr, false);

    return( d_ptr->isBaseUnit() );
}


bool StaffObject::isSoldier() const
{
    CHECK_D_RET(d_ptr, false);

    return( d_ptr->isSoldier() );
}


bool StaffObject::isCommander() const
{
    CHECK_D_RET(d_ptr, false);

    return( d_ptr->isCommander() );
}


bool StaffObject::isDivision() const
{
    CHECK_D_RET(d_ptr, false);

    return( d_ptr->isDivision() );
}

// }}} [ CHECK PROPERTIES ]
// [ OPERATORS ] {{{

bool StaffObject::operator ==(const StaffObject &other) const
{
//    if( d_ptr != other.d_ptr ) {
//        if( d_ptr->staffType() == other.staffType() )
//            if( d_ptr->name == other.name() )
//                return( d_ptr->nameFull == other.nameFull() );
//    }

    return( d_ptr == other.d_ptr );
}


bool StaffObject::operator !=(const StaffObject &other) const
{
    return(d_ptr != other.d_ptr);
}

// }}} [ OPERATORS ]
