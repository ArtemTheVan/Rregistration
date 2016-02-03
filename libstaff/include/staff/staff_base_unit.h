#ifndef STAFF_BASE_UNIT_H
#define STAFF_BASE_UNIT_H

#include <QtGlobal>
#include <QtCore>
#include <QtCore/qstring.h>
#include <QtCore/qlist.h>
#include <QtCore/qhash.h>
#include <QtNetwork/qhostaddress.h>

#include "staff_globals.h"
#include "staff_types.h"
#include "staff_object.h"


class StaffObject;
class StaffDivision;
class StaffBaseUnitPrivate;


class StaffBaseUnit: public StaffObject
{
public:
    StaffBaseUnit();
    StaffBaseUnit(const StaffBaseUnit &other);
    StaffBaseUnit &operator=(const StaffBaseUnit &other);
    virtual ~StaffBaseUnit();

    StaffBaseUnit clone(bool deep = true);
    StaffBaseUnit copy();

    void create();

public:
    // [ PROPERTIES ]
    // [ Должность: П | К | С | СС .... ]
    QString dutyName() const;
    void setDutyName(const QString& dutyName);

    // [ Порядковый номер по иерархии ]
    void setNumber(int unitNumber);

    // [ Имя в иерархии, т.е. 1-П (первый пулеметчик, ), СС.1-МСО (старший стрелок 1-го МСО) и т.п. ]
    void setName(const QString& name);
    void setName(const QString& dutyName, const QString& divisionName, int unitNumber = 0);

    // [ ФИО ]
    void setUnitFIO(const QString& fio);
    QString FIO() const;

    // [ ДАТА И МЕСТО РОЖДЕНИЯ ]
    void setBirthDate(const QDate& date);
    void setBirthDate(const QString& date);
    QDate birthDate() const;

    void setBirthPlace(const QString& birthPlace);
    QString birthPlace() const;

    // [ АДРЕС ПРОЖИВАНИЯ/МЕСТО ЖИТЕЛЬСТВА ]
    void setStreetAddress(const StaffUnitStreetAddress& streetAddress);
    StaffUnitStreetAddress streetAddress() const;

    // [ ВУС ]
    void setSpecialty(const QString& specialty);
    QString specialty() const;

    // [ Имя подразделения, к которому принадлежит ]
    QString divisionName() const;
    void setDivisionName(const QString& divisionName);

    void setFullPartName(const QString& fullPart);
    void setDescription(const QString& description);

    // [ IP-адрес ]
    QHostAddress netAddress() const;
    void setNetAddress(const QHostAddress& netAddress);

    // [ РЕГИСТРАЦИЯ ]
    time_t registrationTime() const;
    void setRegistrationTime(time_t registrationTime);

    void setRegistered(bool registered);
    bool isRegistered() const;

    // [ ДОЛЖНОСТЬ ]
    staff::UnitDuty duty() const;
    void setDuty(staff::UnitDuty duty);

    // [ ЗВАНИЕ ]
    staff::UnitRank rank() const;
    void setRank(staff::UnitRank rank);

    // [ ЗДОРОВЬЕ ]
    staff::Health health() const;
    void setHealth(staff::Health h);

    // [ СНАРЯЖЕНИЕ ]
    const StaffUnitEquipment &equipment() const;
    void setEquipment(const StaffUnitEquipment& equipment);

    staff::RankCategory category() const;

    inline StaffObject::StaffType staffType() const { return(BaseUnitType); }

    // [ PARENT DIVISION ]:
    StaffDivision division() const;

public:
    // [ CHECK PROPERTIES ]:
    bool hasDivision() const;    

protected:    
    StaffBaseUnit(StaffBaseUnitPrivate* dd);

private:
    Q_DECLARE_PRIVATE_D(StaffObject::d_ptr, StaffBaseUnit)
    Q_FRIEND(StaffObject)
    Q_FRIEND(StaffDivision)
    Q_FRIEND(ESUStaffEngine)    
    Q_FRIEND(ESUStaffSystemPrivate)
};

#endif // STAFF_BASE_UNIT_H
