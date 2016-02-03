#ifndef STAFF_SOLDIER_H
#define STAFF_SOLDIER_H

#include "staff_base_unit.h"
//#include "staff_object.h"

#include "staff_commander.h"
//#include "staff_soldier_p.h"

//class StaffBaseUnit; //todo wtf!
//class StaffSoldier;
class StaffSoldierPrivate;
//class StaffCommander;

class StaffSoldier : public StaffBaseUnit
{
public:
    StaffSoldier();
    StaffSoldier(const StaffSoldier &other);
    StaffSoldier &operator=(const StaffSoldier &other);
    ~StaffSoldier();

    StaffSoldier clone(bool deep = true);
    StaffSoldier copy();
    void create();

//    bool operator ==(const StaffSoldier& other);
//    bool operator !=(const StaffSoldier& other);

public:
    // [ PROPERTIES ]:
    StaffCommander commander() const;

//    inline StaffObject::StaffType staffType() const { return(/*StaffObject::SoldierType*/StaffType(2)); } //todo

public:
    // [ CHECK PROPERTIES ]:
    bool hasCommander() const;

protected:
    StaffSoldier(StaffSoldierPrivate* dd);

private:
    Q_DECLARE_PRIVATE_D(StaffObject::d_ptr, StaffSoldier)    

    friend class StaffObject;
    friend class StaffDivision;
    friend class StaffCommander;

    Q_FRIEND(StaffDivisionPrivate)
    Q_FRIEND(ESUStaffEngine)
    Q_FRIEND(ESUStaffSystemPrivate)
};

typedef QList<StaffSoldier> StaffSoldierList;


#endif // STAFF_SOLDIER_H
