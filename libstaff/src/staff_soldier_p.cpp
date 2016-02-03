#include "staff_soldier_p.h"

#include "staff_commander_p.h"


// [ INIT/DESTROY ]: {{{

StaffSoldierPrivate::StaffSoldierPrivate():
    StaffBaseUnitPrivate()
  , m_commander(nullptr)
{

}

StaffSoldierPrivate::StaffSoldierPrivate(StaffSoldierPrivate *other, bool deep, bool copyLinks):
    StaffBaseUnitPrivate(other, deep)
{
    if( copyLinks ) m_commander = other->getCommander();
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


void StaffSoldierPrivate::setCommander(StaffObjectPrivate *commander) //todo it is StaffCommanderPrivate
{
    if( !commander || commander == m_commander ) return;
    m_commander = commander;
}


// }}} [ INIT/DESTROY ]
