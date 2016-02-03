#include "staff_commander_p.h"



// [ INIT/DESTROY ]: {{{

StaffCommanderPrivate::StaffCommanderPrivate():
    StaffBaseUnitPrivate()
{
    m_rank = staff::SergeantRank;
    m_category = getRankCategoryByRank(m_rank);
}


StaffCommanderPrivate::StaffCommanderPrivate(StaffCommanderPrivate* other, bool deep):
    StaffBaseUnitPrivate(other, deep)
{
    //
}


StaffCommanderPrivate::~StaffCommanderPrivate()
{
    //
}


StaffCommanderPrivate *StaffCommanderPrivate::clone(bool deep)
{
    StaffCommanderPrivate* p = new StaffCommanderPrivate(this, deep);
    p->ref.deref();
    return p;
}


StaffCommanderPrivate *StaffCommanderPrivate::copy()
{
    StaffCommanderPrivate* p = new StaffCommanderPrivate(this, false);
    p->ref.deref();
    return p;
}

// }}} [ INIT/DESTROY ]
