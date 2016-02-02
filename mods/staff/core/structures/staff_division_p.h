#ifndef STAFF_DIVISION_P_H
#define STAFF_DIVISION_P_H

#include "staff_division.h"
#include "staff_object_p.h"

#include "staff_soldier.h"
#include "staff_commander.h"
#include "staff_soldier_p.h"
#include "staff_commander_p.h"

using namespace std;


class StaffDivisionPrivate: public StaffObjectPrivate
{
public:
    StaffDivisionPrivate(StaffDivisionPrivate* parentDivision = nullptr);
    StaffDivisionPrivate(StaffDivisionPrivate* other, bool deep, bool copyLinks = false);
    virtual ~StaffDivisionPrivate();

    virtual StaffDivisionPrivate *clone(bool deep = true);
    virtual StaffDivisionPrivate *copy();

    void clear();

public:
    virtual StaffObject::StaffType staffType() const { return StaffObject::DivisionType; }

public:
    void setName(const QString& baseName, int num = 0);
    void setNameFullPart( const QString& fullPart );

    void setNumber(int num);
    void updateName();
    void updateSoldiersCommander();
    void setParentDivision(StaffDivisionPrivate* p);

    int count(staff::RankCategory category);

public:
    void addSoldier(StaffSoldierPrivate* soldier);
    void addDivision(StaffDivisionPrivate* division);

public:
    staff::DivisionType type;       // Тип подразделения (отделение, возвод, рота, батальон, бригада и т.д.)
    StaffDivisionPrivate* parentDivision;
    /*
    StaffCommanderPrivate*          commander;  // Командир подразделения
    QList<StaffSoldierPrivate*>     soldiers;   // Подчиненные (солдаты и т.п.)
    QList<StaffDivisionPrivate*>    divisions;  // Подчиненные подразделения
    */
    StaffCommander      commander;  // Командир подразделения
    StaffSoldierList    soldiers;   // Подчиненные (солдаты и т.п.)
    StaffDivisionList   divisions;  // Подчиненные подразделения


    // Количественный состав подразделения
    int countData[staff::CountCategoryCount][staff::RankCategoryCount];
    StaffUnitEquipment equip;

    friend class StaffSoldier;
    friend class StaffCommander;
};


// <StaffObjectPrivate>:
//     nameBase;       // МСО | ПУЛО - Сокращенное наименование подразделения
//     name;           // 1-МСО | [1,2,3...]-МСВ - Наименование подразделения с учетом порядкового номера
//     nameFull;       // Полное наименование. Пример: 1-МСО.1-МСВ.2-МСР.1-МСБ
//     nameFullPart;   // Дополнение для полного наименования. Пример: 1-МСВ.2-МСР.1-МСБ
//     description;    // Описание. Пример: если у нас МСО, то --> "Мотострелковое отделение"
//     number;         // Порядковый номер подразделения

#endif // STAFF_DIVISION_P_H
