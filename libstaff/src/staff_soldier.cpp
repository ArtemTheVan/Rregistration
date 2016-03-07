#include "staff_soldier.h"
#include "staff_soldier_p.h"

#include "staff_commander.h"


// [ INIT/DESTROY ]: {{{

StaffSoldier::StaffSoldier():
    StaffBaseUnit()
{

}


StaffSoldier::StaffSoldier(const StaffSoldier &other):
     StaffBaseUnit(other)
{

}


StaffSoldier::StaffSoldier(StaffSoldierPrivate *dd):
    StaffBaseUnit(dd)
{

}


StaffSoldier &StaffSoldier::operator=(const StaffSoldier &other)
{
    return (StaffSoldier&) StaffBaseUnit::operator=(other);
}


StaffSoldier::~StaffSoldier()
{
    //
}


StaffSoldier StaffSoldier::clone(bool deep)
{
    CHECK_D_RET(d_ptr, StaffSoldier());
    Q_D(StaffSoldier);

    return StaffSoldier(((StaffSoldierPrivate*)d_ptr)->clone(deep));
}


StaffSoldier StaffSoldier::copy()
{
    CHECK_D_RET(d_ptr, StaffSoldier());    

    return StaffSoldier(((StaffSoldierPrivate*)d_ptr)->copy());
}


void StaffSoldier::create()
{
    if( d_ptr == nullptr ) {
        d_ptr = (StaffObjectPrivate*)(new StaffSoldierPrivate());
    }
}


//bool StaffSoldier::operator ==(const StaffSoldier &other)
//{
//    CHECK_D_RET(d_ptr, false);
//    Q_D(StaffSoldier);

//    if( StaffObject::operator==(other) ) {
//        // TODO: Сделать правильное сравнение
//        return( true );
//    }

//    return( false );
//}

//bool StaffSoldier::operator !=(const StaffSoldier &other)
//{
//    return( !(*this == other) );
//}

// }}} [ INIT/DESTROY ]
// [ PROPERTIES ]: {{{

StaffCommander StaffSoldier::commander() const
{
    CHECK_D_RET(d_ptr, StaffCommander());
    Q_D(const StaffSoldier);

//    return(StaffCommander((StaffCommanderPrivate*)d->getCommander())); //todo
    return(StaffCommander());
}

// }}} [ PROPERTIES ]
// [ CHECK PROPERTIES ]: {{{

bool StaffSoldier::hasCommander() const
{
    CHECK_D_RET(d_ptr, false);
    Q_D(const StaffSoldier);

//    return(d->getCommander() != nullptr); //todo
    return(false); //todo
}

// }}} [ CHECK PROPERTIES ]
