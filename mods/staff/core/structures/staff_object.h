#ifndef STAFF_OBJECT_H
#define STAFF_OBJECT_H

#include <QtGlobal>
#include <QtCore/qstring.h>
#include <QtCore/qlist.h>

#include "staff_globals.h"

class StaffObject;
class StaffObjectPrivate;

class StaffBaseUnit;
class StaffSoldier;
class StaffCommander;
class StaffDivision;


typedef QList<StaffObject> StaffObjectList;


class StaffObject
{
public:
    enum StaffType
    {
        ObjectType=0,
        BaseUnitType=1,
        SoldierType=2,
        CommanderType=3,
        DivisionType=4
    };

public:
    StaffObject();
    StaffObject(const StaffObject &other);
    StaffObject &operator=(const StaffObject &other);
    virtual ~StaffObject();

    StaffObject clone(bool deep = true) const;
    StaffObject copy() const;

    virtual void create();
    virtual void release();

public:
    // [ OBJECT PROPERTIES ]:
    StaffType staffType() const;
    void setNull();
    void clear();

    // [ CONVERT ]:
    StaffObject toObject() const;
    StaffBaseUnit toBaseUnit() const;
    StaffSoldier toSoldier() const;
    StaffCommander toCommander() const;
    StaffDivision toDivision() const;    

public:
    // [ PROPERTIES ]:
    virtual QString nameBase() const;
    virtual QString name() const;
    virtual QString nameFull() const;
    virtual QString nameFullPart() const;
    virtual QString nameDecription() const;
    virtual QString decription() const;
    virtual QString note() const;
    virtual int number() const;    
    virtual QString key() const;


    virtual void setDecription(const QString& decription);
    virtual void setNote(const QString& note);

public:
    // [ CHECK PROPERTIES ]:
    bool isNull() const;    
    bool isObject() const;
    bool isBaseUnit() const;
    bool isSoldier() const;
    bool isCommander() const;
    bool isDivision() const;

public:
    // [ OPERATORS ]
    virtual bool operator ==(const StaffObject &other) const;
    virtual bool operator !=(const StaffObject &other) const;

protected:
    StaffObjectPrivate* d_ptr;
    StaffObject(StaffObjectPrivate* dd);

private:
    Q_DECLARE_PRIVATE_D(StaffObject::d_ptr, StaffObject)

    Q_FRIEND(ESUStaffEngine)
    Q_FRIEND(ESUStaffSystemPrivate)
    Q_FRIEND(ESUStaffStructure)
};

#endif // STAFF_OBJECT_H
