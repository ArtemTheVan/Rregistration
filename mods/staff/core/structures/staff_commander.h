#ifndef STAFF_COMMANDER_H
#define STAFF_COMMANDER_H

#include "staff_base_unit.h"


class StaffSoldier;
class StaffDivision;
class StaffCommanderPrivate;


class StaffCommander : public StaffBaseUnit
{
public:
    StaffCommander();
    StaffCommander(const StaffCommander &other);
    StaffCommander &operator=(const StaffCommander &other);
    ~StaffCommander();

    StaffCommander clone(bool deep = true);
    StaffCommander copy();
    void create();

//    bool operator ==(const StaffCommander& other);
//    bool operator !=(const StaffCommander& other);

public:
    inline StaffObject::StaffType staffType() const { return(CommanderType); }

protected:
    StaffCommander(StaffCommanderPrivate* dd);

private:
    Q_DECLARE_PRIVATE_D(StaffObject::d_ptr, StaffCommander)

    friend class StaffObject;
    friend class StaffSoldier;
    friend class StaffDivision;

    friend class StaffDivisionPrivate;

    Q_FRIEND(ESUStaffEngine)
    Q_FRIEND(ESUStaffSystemPrivate)
};

#endif // STAFF_COMMANDER_H
