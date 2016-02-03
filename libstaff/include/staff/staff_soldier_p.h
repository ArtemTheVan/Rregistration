#ifndef STAFF_SOLDIER_P_H
#define STAFF_SOLDIER_P_H

#include "staff_soldier.h"
#include "staff_base_unit_p.h"


class StaffCommanderPrivate;

class StaffSoldierPrivate: public StaffBaseUnitPrivate
{
public:
    StaffSoldierPrivate();
    StaffSoldierPrivate(StaffSoldierPrivate* other, bool deep, bool copyLinks = false);
    virtual ~StaffSoldierPrivate();

    StaffSoldierPrivate *clone(bool deep = true);
    StaffSoldierPrivate *copy();

public:
    StaffObject::StaffType staffType() const { return StaffObject::SoldierType; }

    void setCommander(StaffObjectPrivate* commander); //todo it is StaffCommanderPrivate
    StaffObjectPrivate* getCommander() //todo it is StaffCommanderPrivate
    {
        return m_commander;
    }

private:
    StaffObjectPrivate*   m_commander; //todo it is StaffCommanderPrivate
};

#endif // STAFF_SOLDIER_P_H
