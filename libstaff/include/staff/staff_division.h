#ifndef STAFF_DIVISION_H
#define STAFF_DIVISION_H

#include <QtCore>
#include <QtNetwork/qhostaddress.h>

#include "staff_object.h"
#include "staff_soldier.h"
#include "staff_commander.h"


//class StaffDivision; //todo wtf
class StaffDivisionPrivate;


class StaffDivision: public StaffObject
{
public:
    StaffDivision();
    StaffDivision(const StaffDivision &);
    StaffDivision &operator=(const StaffDivision &);
    ~StaffDivision();

    StaffDivision clone(bool deep = true) const;
    StaffDivision copy() const;

    void create(StaffDivision* parent = nullptr);    
    void clear();

public:
    // [ PROPERTIES ]: {{{
    void setNameBase(const QString& nameBase);
    void setName(const QString& nameBase, int num = 0);
    void setNameFullPart( const QString& fullPart );
    void setNumber(int num);

    staff::DivisionType type() const;
    void setType(staff::DivisionType type);

    StaffCommander commander() const;
    bool setCommander(const StaffCommander& commander);

    StaffDivision parentDivision() const;

    StaffSoldierList soldiers() const;
    void setSoldiers(const StaffSoldierList& soldier);

    QList<StaffDivision> divisions() const;
    void setDivisions(const QList<StaffDivision>& divisions);

    StaffObjectList objects() const;
    // }}}

    // [ CREATION METHODS ][ CHILD NODES ]:
    StaffDivision   createSubDivision(const QString& baseName, int num = 0);
    StaffSoldier    createSoldier(const QString& dutyName, int num = 0);
    StaffCommander  createCommander();

    bool addObject(StaffObject& object);

    // [SOLDIERS METHODS]:
    bool addSoldier(StaffSoldier& soldier);
    bool removeSoldier(StaffSoldier& soldier);
    bool removeSoldier(const QString& name);
    bool removeSoldier(const QHostAddress& address);
    int  removeSoldiers(staff::UnitDuty duty);

    StaffSoldier getSoldier(const QString& name);
    StaffSoldier getSoldier(const QHostAddress& address);
    StaffSoldierList getSoldiers(staff::UnitDuty duty);

    // [DIVISIONS METHODS]:
    bool addDivision(StaffDivision& division);
    bool removeDivision(const StaffDivision& division);
    bool removeDivision(const QString& name);
    bool removeDivision(int index);
    int  removeDivisions(staff::DivisionType type);

    StaffDivision getDivision(const QString& name);
    StaffDivision getDivision(const StaffCommander& commander);
    StaffDivision getDivision(const StaffDivision& division);
    QList<StaffDivision> getDivisions(staff::DivisionType type);

    // [Для модуля учета]:
    // Количество военнослужащих, состоящих непосредственно в текущем подразделении
    // и не входящих в подчиненные подразделения(по категориям)
    int staffCountSimple(staff::RankCategory category);
    int calcStaffCount(staff::CountCategory cntCategory, staff::RankCategory category) const;

    /* Подсчитывает кол-во военнослужащих разных категорий(Солдаты, Сержанты, Офицеры) по оргштатной структуре.
     * Запоминает результаты в узлах дерева являющихся подразделениями.
     * Имеет смысл вызывать единожды для заполнения статистики подразделений. */
    int staffCountRecursive(staff::RankCategory category);
    int countData(staff::CountCategory cntCategory, staff::RankCategory rankCategory) const;
    void chgCountData(staff::CountCategory cntCategory, staff::RankCategory rankCategory, int new_data);
    void chgCountDataRecursive(staff::CountCategory cntCategory, staff::RankCategory rankCategory, int new_data);
    void incCountData(staff::CountCategory cntCategory, staff::RankCategory rankCategory, int inc_value = 1, int max_value = 1e6);
    void incCountDataRecursive(staff::CountCategory cntCategory, staff::RankCategory rankCategory, int inc_value = 1);

    /* Рассчет суммарного кол-ва снаряжения, содержащегося в подразделении. Сохраняет данные в подразделении. */
    void initEquipment();
    /* Пересчет кол-ва элементов снаряжения определенного типа(eqtype: БП,ВВТ,МТС),
     * наименования элемента(АК-74) и категории(category: current_count, state_count, multiplier) */
    int  calcEquipment(StaffUnitEquipment::EquipmentType eqtype, QString type, int category = 2/*current_count*/) const;
    /* Доступ к снаряжению(только для чтения) */
    const StaffUnitEquipment& equipmentConst() const;
    StaffUnitEquipment& equipment();
    /* Изменение кол-ва элементов снаряжения определенного типа(eqtype: БП,ВВТ,МТС),
     * наименования элемента(АК-74) и категории(category: current_count, state_count, multiplier) */
    void chgEquipment(StaffUnitEquipment::EquipmentType eqtype, QString type, int new_value, int category = 2/*current_count*/);
    void incEquipment(StaffUnitEquipment::EquipmentType eqtype, QString type, int inc_value, int category = 2/*current_count*/);

private:
    void initEquipment(StaffObject obj);


public:
    // [CHECK PROPERTIES]:
    bool hasSoldiers() const;
    bool hasCommander() const;
    bool hasDivisions() const;
    bool hasObjects() const;
    bool hasParentDivision() const;    

    // [CHECK METHODS]:
    bool isEmpty() const;
    bool isParentDivision(const StaffDivision& division) const;

    bool contains(const StaffObject& object);
    bool containsDivision(const StaffDivision& division) const;
    bool containsSoldier(const StaffSoldier& soldier) const;

public:
    inline StaffObject::StaffType staffType() const { return(DivisionType); }

protected:
    StaffDivision(StaffDivisionPrivate* dd);

private:
    Q_DECLARE_PRIVATE_D(StaffObject::d_ptr, StaffDivision)    

    friend class StaffObject;
    friend class StaffBaseUnit;
    friend class StaffSoldier;
    friend class StaffCommander;

    Q_FRIEND(ESUStaffEngine)
    Q_FRIEND(ESUStaffSystemPrivate)
};

typedef QList<StaffDivision> StaffDivisionList; //!!! todo wtf!


#endif // STAFF_DIVISION_H
