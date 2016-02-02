#include "staff_soldier_p.h"

#include "staff_commander_p.h"


// [ INIT/DESTROY ]: {{{

StaffSoldierPrivate::StaffSoldierPrivate():
    StaffBaseUnitPrivate()
  , commander(nullptr)
{

}

StaffSoldierPrivate::StaffSoldierPrivate(StaffSoldierPrivate *other, bool deep, bool copyLinks):
    StaffBaseUnitPrivate(other, deep)
{
    if( copyLinks ) commander = other->commander;
}


StaffSoldierPrivate::~StaffSoldierPrivate()
{

}


StaffSoldierPrivate *StaffSoldierPrivate::clone(bool deep)
{
    StaffSoldierPrivate* p = new StaffSoldierPrivate(this, deep);
    p->ref.deref();
    return p;
}


StaffSoldierPrivate *StaffSoldierPrivate::copy()
{
    StaffSoldierPrivate* p = new StaffSoldierPrivate(this, false, false);
    p->ref.deref();
    return p;
}

// }}} [ INIT/DESTROY ]
