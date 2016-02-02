#ifndef STAFF_COMMANDER_P_H
#define STAFF_COMMANDER_P_H

#include "staff_commander.h"
#include "staff_base_unit_p.h"


class StaffCommanderPrivate: public StaffBaseUnitPrivate
{
public:
    StaffCommanderPrivate();
    StaffCommanderPrivate(StaffCommanderPrivate* other, bool deep);
    virtual ~StaffCommanderPrivate();

    StaffCommanderPrivate *clone(bool deep = true);
    StaffCommanderPrivate *copy();

public:
    StaffObject::StaffType staffType() const { return StaffObject::CommanderType; }

public:
    // Data
};

#endif // STAFF_COMMANDER_P_H
