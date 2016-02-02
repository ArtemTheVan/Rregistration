#include "staff_commander.h"
#include "staff_commander_p.h"


// [ INIT/DESTROY ]: {{{

StaffCommander::StaffCommander():
    StaffBaseUnit()
{
    //
}


StaffCommander::StaffCommander(const StaffCommander &other):
     StaffBaseUnit(other)
{

}


StaffCommander::StaffCommander(StaffCommanderPrivate *dd):
    StaffBaseUnit(dd)
{

}


StaffCommander &StaffCommander::operator=(const StaffCommander &other)
{
    return (StaffCommander&) StaffBaseUnit::operator=(other);
}


StaffCommander::~StaffCommander()
{
    //
}


StaffCommander StaffCommander::clone(bool deep)
{
    CHECK_D_RET(d_ptr, StaffCommander());
    Q_D(StaffCommander);

    return StaffCommander(d->clone(deep));
}


StaffCommander StaffCommander::copy()
{
    CHECK_D_RET(d_ptr, StaffCommander());
    Q_D(StaffCommander);

    return StaffCommander(d->copy());
}


void StaffCommander::create()
{
    if( d_ptr == nullptr ) {
        d_ptr = (StaffObjectPrivate*)(new StaffCommanderPrivate());

        setDutyName("К");
    }
}


//bool StaffCommander::operator ==(const StaffCommander &other)
//{
//    CHECK_D_RET(d_ptr, false);
//    Q_D(StaffCommander);

//    // TODO: Сделать правильное сравнение
//    if( StaffObject::operator==(other) ) {
//        return( true );
//    }

//    return( false );
//}


//bool StaffCommander::operator !=(const StaffCommander &other)
//{
//    return( !(*this == other) );
//}

// }}} [ INIT/DESTROY ]
