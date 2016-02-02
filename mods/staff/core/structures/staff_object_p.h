#ifndef STAFF_OBJECT_P_H
#define STAFF_OBJECT_P_H

#include "staff_object.h"


class ESUStaffEngine;

class StaffObjectPrivate
{
public:
    StaffObjectPrivate();
    StaffObjectPrivate(StaffObjectPrivate* other, bool deep, bool copyLinks = false);
    virtual ~StaffObjectPrivate();

    virtual StaffObjectPrivate *clone(bool deep = true);
    virtual StaffObjectPrivate *copy();

public:
    virtual StaffObject::StaffType staffType() const { return StaffObject::ObjectType; }

    inline bool isObject() const    { return staffType() == StaffObject::ObjectType;    }    
    inline bool isSoldier() const   { return staffType() == StaffObject::SoldierType;   }
    inline bool isCommander() const { return staffType() == StaffObject::CommanderType; }
    inline bool isDivision() const  { return staffType() == StaffObject::DivisionType;  }
    inline bool isBaseUnit() const
    { return (staffType() == StaffObject::BaseUnitType ||
              staffType() == StaffObject::SoldierType ||
              staffType() == StaffObject::CommanderType);  }

public:
    virtual void clear();

public:
    QAtomicInt      ref;
    ESUStaffEngine *engine;

    QString         nameBase;
    QString         name;
    QString         nameFull;
    QString         nameFullPart;
    QString         nameDescription;
    QString         description;
    QString         note;
    int             number;
};

#endif // STAFF_OBJECT_P_H
