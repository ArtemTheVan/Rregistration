#include "staff_division.h"
#include "staff_division_p.h"

#include "staff_soldier_p.h"
#include "staff_commander_p.h"


// [ INIT/DESTROY ]: {{{

StaffDivision::StaffDivision():
    StaffObject()
{

}


StaffDivision::StaffDivision(const StaffDivision &other):
    StaffObject(other)
{
    //
}


StaffDivision::StaffDivision(StaffDivisionPrivate* dd):
    StaffObject(dd)
{
    //
}


StaffDivision &StaffDivision::operator=(const StaffDivision &other)
{    
    return (StaffDivision&) StaffObject::operator=(other);
}


StaffDivision::~StaffDivision()
{
    //
}

// [ CLONE DIVISION ]

StaffDivision StaffDivision::clone(bool deep) const
{
    if( d_ptr == nullptr )
        return StaffDivision();    

    return StaffDivision(((StaffDivisionPrivate*)d_ptr)->clone(deep));
}


StaffDivision StaffDivision::copy() const
{
    if( d_ptr == nullptr )
        return StaffDivision();

    return StaffDivision(((StaffDivisionPrivate*)d_ptr)->copy());
}


void StaffDivision::create(StaffDivision *parent)
{
    if( d_ptr == nullptr ) {
        d_ptr = (StaffObjectPrivate*)(new StaffDivisionPrivate());
        Q_D(StaffDivision);

        if( parent != nullptr ) d->setParentDivision(parent->d_func());
    }
}


void StaffDivision::clear()
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    d->clear();
}

// }}} [ INIT/DESTROY ]
// [ PROPERTIES ] {{{

// [ BASE_NAME ]:

void StaffDivision::setNameBase(const QString &nameBase)
{    
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    QString clearName = staff_private::formatName(nameBase);

    if( d->nameBase != clearName ) {
        d->nameBase = clearName;       
        d->nameDescription = staff::getDivisionDescrByName(d->nameBase);
        d->updateName();
    }
}

// [ NAME ]:

void StaffDivision::setName(const QString &nameBase, int num)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    d->setName(nameBase, num);
}

// [ FULL_NAME ]:

void StaffDivision::setNameFullPart(const QString &fullPart)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    d->setNameFullPart( fullPart );
}

// [ NUMBER ]:

void StaffDivision::setNumber(int num)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    if( num > 0 && d->number != num ) {
        d->number = num;
        d->updateName();
    }
}

// [ DIVISION TYPE ]:

staff::DivisionType StaffDivision::type() const
{
    CHECK_D_RET(d_ptr, staff::UnknownDivision);
    Q_D(const StaffDivision);

    return( d->type );
}


void StaffDivision::setType(staff::DivisionType type)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    if( d->type != type ) {
        d->type = type;
    }
}

// [DIVISION/COMMANDER]:

StaffCommander StaffDivision::commander() const
{
    CHECK_D_RET(d_ptr, StaffCommander());
    Q_D(const StaffDivision);

    return( d->commander );
}


bool StaffDivision::setCommander(const StaffCommander &commander)
{
    CHECK_D_RET(d_ptr, false);
    Q_D(StaffDivision);

    if( d->commander != commander ) {
        d->commander = commander;
        d->commander.setDivisionName(d->name);
        if( !d->nameFullPart.isEmpty() )
            d->commander.setFullPartName(d->nameFullPart);
        d->commander.setDuty(staff::getCommanderDutyByDivType(d->type));

        d->commander.d_func()->m_parentDivision = d;
        if( d->engine ) commander.d_ptr->engine = d->engine;

        d->updateSoldiersCommander();

        return( true );
    }

    return( false );
}

// [ PARENT DIVISION ]:

StaffDivision StaffDivision::parentDivision() const
{
    Q_D(const StaffDivision);

    if( !d || d->parentDivision == nullptr )
        return( StaffDivision() );

    return( StaffDivision(d->parentDivision) );
}


StaffSoldierList StaffDivision::soldiers() const
{
    CHECK_D_RET(d_ptr, StaffSoldierList());
    Q_D(const StaffDivision);

    return(d->soldiers);
}

// [ SOLDIERS/DIVISIONS ]:

void StaffDivision::setSoldiers(const StaffSoldierList &soldier)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    if( d->soldiers.isEmpty() ) {
        d->soldiers = soldier;
        d->soldiers.detach();
    }
}


StaffDivisionList StaffDivision::divisions() const
{
    CHECK_D_RET(d_ptr, StaffDivisionList());
    Q_D(const StaffDivision);

    return( d->divisions );
}


void StaffDivision::setDivisions(const StaffDivisionList &divisions)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    if( d->divisions.isEmpty() ) {
        d->divisions = divisions;
        d->divisions.detach();
    }
}


StaffObjectList StaffDivision::objects() const
{
    CHECK_D_RET(d_ptr, StaffObjectList());
    Q_D(const StaffDivision);

    StaffObjectList result;
    int i, count;

    if( !d->commander.isNull() ) {
        result.append(d->commander);
    }

    count = d->soldiers.count();
    for( i = 0; i < count; ++i ) {
        result.append(d->soldiers[i]);
    }

    count = d->divisions.count();
    for( i = 0; i < count; ++i ) {
        result.append(d->divisions[i]);
    }    

    return( result );
}

// }}} [ PROPERTIES ]
// [ CREATION METHODS ][ CHILD NODES ]: {{{

StaffDivision StaffDivision::createSubDivision(const QString &baseName, int num)
{
    CHECK_D_RET(d_ptr, StaffDivision());
    Q_D(StaffDivision);

    StaffDivision division(new StaffDivisionPrivate(d));
    if( d->engine ) division.d_ptr->engine = d->engine;
    division.setName(baseName, num);
    addDivision(division);

    return(division);
}


StaffSoldier StaffDivision::createSoldier(const QString &dutyName, int num)
{
    CHECK_D_RET(d_ptr, StaffSoldier());
    Q_D(StaffDivision);

    StaffSoldier soldier;
    soldier.create();
    if( d->engine ) soldier.d_ptr->engine = d->engine;
    soldier.setName(dutyName, d->name, num);    
    soldier.d_func()->m_parentDivision = d;

    addSoldier(soldier);

    return(soldier);
}


StaffCommander StaffDivision::createCommander()
{
    CHECK_D_RET(d_ptr, StaffCommander());
    Q_D(StaffDivision);

    StaffCommander commander;
    if( !d->commander.isNull() ) return commander;

    commander.create();
    if( d->engine ) commander.d_ptr->engine = d->engine;
    commander.d_func()->m_parentDivision = d;
    setCommander(commander);

    return(commander);
}


bool StaffDivision::addObject(StaffObject &object)
{
    CHECK_D_RET(d_ptr, false);
    Q_D(StaffDivision);

    if( object.isDivision() ) {
        StaffDivision obj = object.toDivision();
        if( d->divisions.contains(obj) ) return(false);
        return( addDivision(obj) );

    } else if( object.isSoldier() ) {
        StaffSoldier obj = object.toSoldier();
        if( d->soldiers.contains(obj) ) return(false);
        return( addSoldier(obj) );

    } else if( object.isCommander() ) {
        StaffCommander obj = object.toCommander();
        if( d->commander == (object.toCommander()) ) return(false);
        return( setCommander(obj) );
    }

    return( false );
}

// }}} [ CREATION METHODS ][ CHILD NODES ]
// [SOLDIERS METHODS]: {{{

// [ ADD ]:

bool StaffDivision::addSoldier(StaffSoldier &soldier)
{
    CHECK_D_RET(d_ptr, false);
    Q_D(StaffDivision);

    if( !d->soldiers.contains(soldier) ) {
        soldier.setDivisionName(d->name);
        soldier.setFullPartName( d->nameFullPart );

        if( d->engine ) soldier.d_ptr->engine = d->engine;
        soldier.d_func()->m_parentDivision = d;
        soldier.d_func()->commander = d->commander.d_func();

        d->soldiers.append(soldier);
        return(true);
    }
    return(false);
}

// [ REMOVE ]:

bool StaffDivision::removeSoldier(StaffSoldier &soldier)
{
    CHECK_D_RET(d_ptr, false);
    Q_D(StaffDivision);

    if( d->soldiers.contains(soldier) ) {
        soldier.d_func()->commander = nullptr;
        d->soldiers.removeOne(soldier);
        return(true);
    }
    return(false);
}


bool StaffDivision::removeSoldier(const QString &name)
{
    CHECK_D_RET(d_ptr, false);
    Q_D(StaffDivision);

    StaffSoldierList::iterator it = d->soldiers.begin();
    while (it != d->soldiers.end()) {
        if( it->name() == name )
        {
            (*it).d_func()->commander = nullptr;
            d->soldiers.erase(it);
            return(true);
        }
        ++it;
    }

    return(false);
}


bool StaffDivision::removeSoldier(const QHostAddress &address)
{
    CHECK_D_RET(d_ptr, false);
    Q_D(StaffDivision);

    StaffSoldierList::iterator it = d->soldiers.begin();
    while (it != d->soldiers.end()) {
        if( it->netAddress() == address ) {
            (*it).d_func()->commander = nullptr;
            d->soldiers.erase(it);
            return(true);
        }
        ++it;
    }

    return(false);
}


int StaffDivision::removeSoldiers(staff::UnitDuty duty)
{
    CHECK_D_RET(d_ptr, -1);
    Q_D(StaffDivision);

    StaffSoldierList::iterator it = d->soldiers.begin();
    StaffSoldierList::iterator it_next = it;
    int count = 0;

    while( it != d->soldiers.end() )
    {
        if( it->duty() == duty ) {
            (*it).d_func()->commander = nullptr;
            it_next = d->soldiers.erase(it);
            it = it_next;
            ++count;
            continue;
        }
        ++it;
    }

    return(count);
}

// [ GET ]:

StaffSoldier StaffDivision::getSoldier(const QString &name)
{
    CHECK_D_RET(d_ptr, StaffSoldier());
    Q_D(StaffDivision);

    StaffSoldierList::const_iterator it = d->soldiers.cbegin();
    while (it != d->soldiers.cend()) {
        if( it->name() == name || it->nameFull() == name ) {
            return(*it);
        }
        ++it;
    }

    return(StaffSoldier());
}


StaffSoldier StaffDivision::getSoldier(const QHostAddress &address)
{
    CHECK_D_RET(d_ptr, StaffSoldier());
    Q_D(StaffDivision);

    StaffSoldierList::const_iterator it = d->soldiers.cbegin();
    while (it != d->soldiers.cend()) {
        if( it->netAddress() == address ) {
            return(*it);
        }
        ++it;
    }

    return(StaffSoldier());
}


StaffSoldierList StaffDivision::getSoldiers(staff::UnitDuty duty)
{
    StaffSoldierList result;

    CHECK_D_RET(d_ptr, result);
    Q_D(StaffDivision);

    StaffSoldierList::const_iterator it = d->soldiers.cbegin();
    while (it != d->soldiers.cend()) {
        if( it->duty() == duty ) {
            result.append(*it);
        }
        ++it;
    }

    return( result );
}

// }}} [SOLDIERS METHODS]
// [DIVISIONS METHODS]: {{{

// [ ADD ]:
bool StaffDivision::addDivision(StaffDivision &division)
{
    CHECK_D_RET(d_ptr, false);
    Q_D(StaffDivision);

    if( !d->divisions.contains(division) )
    {
        division.setNameFullPart(d->nameFull);
        if( d->engine ) division.d_ptr->engine = d->engine;
        if( division.d_func()->parentDivision != d ) {
            division.d_func()->parentDivision = d;
        }
        d->divisions.append(division);
        return(true);
    }

    return(false);
}

// [ REMOVE ]:
bool StaffDivision::removeDivision(const StaffDivision &division)
{
    CHECK_D_RET(d_ptr, false);
    Q_D(StaffDivision);

    if( d->divisions.contains(division) ) {
        d->divisions.removeOne(division);
        return(true);
    }

    return(false);
}


bool StaffDivision::removeDivision(const QString &name)
{
    CHECK_D_RET(d_ptr, false);
    Q_D(StaffDivision);

    StaffDivisionList::iterator it = d->divisions.begin();
    while (it != d->divisions.end()) {
        if( it->name() == name ) {
            d->divisions.erase(it);
            return(true);
        }
        ++it;
    }

    return(false);
}


bool StaffDivision::removeDivision(int index)
{
    CHECK_D_RET(d_ptr, false);
    Q_D(StaffDivision);

    if( index <= (d->divisions.count()-1) ) {
        d->divisions.removeAt(index);
        return(true);
    }

    return(false);
}


int StaffDivision::removeDivisions(staff::DivisionType type)
{
    CHECK_D_RET(d_ptr, -1);
    Q_D(StaffDivision);

    StaffDivisionList::iterator it = d->divisions.begin();
    StaffDivisionList::iterator it_next = it;
    int count = 0;

    while( it != d->divisions.end() )
    {
        if( it->type() == type ) {
            it_next = d->divisions.erase(it);
            it = it_next;
            ++count;
            continue;
        }
        ++it;
    }

    return(count);
}

// [ GET ]:

StaffDivision StaffDivision::getDivision(const QString &name)
{
    CHECK_D_RET(d_ptr, StaffDivision());
    Q_D(StaffDivision);

    if( d->divisions.isEmpty() ) return(StaffDivision());

    int i, size = d->divisions.size();

    for( i = 0; i < size; ++i ) {
        if( d->divisions[i].name() == name || d->divisions[i].nameFull() == name ) {
            return(d->divisions[i]);
        }
    }

    return(StaffDivision());
}


StaffDivision StaffDivision::getDivision(const StaffCommander &commander)
{
    CHECK_D_RET(d_ptr, StaffDivision());
    Q_D(StaffDivision);

    StaffDivisionList::const_iterator it = d->divisions.cbegin();
    while (it != d->divisions.cend()) {
        StaffCommander c = it->commander();
        if( c == commander ) {
            return(*it);
        }
        ++it;
    }

    return(StaffDivision());
}


StaffDivision StaffDivision::getDivision(const StaffDivision &division)
{
    CHECK_D_RET(d_ptr, StaffDivision());
    Q_D(StaffDivision);

    StaffDivisionList::const_iterator it = d->divisions.cbegin();
    while (it != d->divisions.cend()) {
        if( it->name() == division.name() && it->nameFull() == division.nameFull() ) {
            return(*it);
        }
        ++it;
    }

    return(StaffDivision());
}


StaffDivisionList StaffDivision::getDivisions(staff::DivisionType type)
{
    StaffDivisionList result;

    CHECK_D_RET(d_ptr, result);
    Q_D(StaffDivision);

    StaffDivisionList::const_iterator it = d->divisions.cbegin();
    while (it != d->divisions.cend()) {
        if( it->type() == type ) {
            result.append(*it);
        }
        ++it;
    }

    return( result );
}

int StaffDivision::staffCountSimple(staff::RankCategory category)
{
    CHECK_D_RET(d_ptr, 0);
    Q_D(StaffDivision);

    int cnt = d->count(category);

    return(cnt);
}

int StaffDivision::calcStaffCount(staff::CountCategory cntCategory, staff::RankCategory category) const
{
    CHECK_D_RET(d_ptr, 0);
    Q_D(const StaffDivision);

    int cnt = 0;

    for(StaffDivision subdiv: divisions()){
        cnt += subdiv.countData(cntCategory, category);
    }

    return(cnt);
}

int StaffDivision::staffCountRecursive(staff::RankCategory category)
{
    CHECK_D_RET(d_ptr, -1);
    Q_D(StaffDivision);

    // Количество военнослужащих, состоящих непосредственно в текущем подразделении и не входящих в подчиненные подразделения
    int cnt = d->count(category);

    // Подсчитываем военнослужащих в подчиненных подразделениях
    for(int i = 0; i < divisions().count(); ++i)
    {
        cnt += divisions()[i].staffCountRecursive(category);
    }

    // Запоминаем данные в дереве
    d->countData[staff::InState][category] = cnt;

    return(cnt);
}

int StaffDivision::countData(staff::CountCategory cntCategory, staff::RankCategory rankCategory) const
{
    CHECK_D_RET(d_ptr, -1);
    Q_D(const StaffDivision);

    return d->countData[cntCategory][rankCategory];
}

void StaffDivision::chgCountData(staff::CountCategory cntCategory, staff::RankCategory rankCategory, int new_data)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    d->countData[cntCategory][rankCategory] = new_data;
}

void StaffDivision::chgCountDataRecursive(staff::CountCategory cntCategory, staff::RankCategory rankCategory, int new_data)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    int delta = new_data - countData(cntCategory, rankCategory);
    d->countData[cntCategory][rankCategory] = new_data;
    if(hasParentDivision()){
        int cnt = parentDivision().countData(cntCategory, rankCategory);
        parentDivision().chgCountDataRecursive(cntCategory, rankCategory, cnt + delta);
    }
}

void StaffDivision::incCountData(staff::CountCategory cntCategory, staff::RankCategory rankCategory, int inc_value, int max_value)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    d->countData[cntCategory][rankCategory] = min(max_value, inc_value + d->countData[cntCategory][rankCategory]);
}

void StaffDivision::incCountDataRecursive(staff::CountCategory cntCategory, staff::RankCategory rankCategory, int inc_value)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    d->countData[cntCategory][rankCategory] += inc_value;
    if(hasParentDivision()){
        parentDivision().incCountDataRecursive(cntCategory, rankCategory, inc_value);
    }
}

void StaffDivision::initEquipment()
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    initEquipment(toObject());
}

int StaffDivision::calcEquipment(StaffUnitEquipment::EquipmentType eqtype, QString type, int category) const
{
    CHECK_D_RET(d_ptr, -1);
    Q_D(const StaffDivision);

    int res = 0;
    StaffDivisionList div_lst = divisions();
    for(int i = 0; i < div_lst.size(); ++i){
        StaffDivision subdiv = div_lst[i];
        res += subdiv.equipmentConst().getConst(eqtype)[type]->get(category);
    }

    return(res);
}

const StaffUnitEquipment &StaffDivision::equipmentConst() const
{
    CHECK_D_RET(d_ptr, StaffUnitEquipment());
    Q_D(const StaffDivision);

    return(d->equip);
}

StaffUnitEquipment &StaffDivision::equipment()
{
    StaffUnitEquipment equip_tmp;
    CHECK_D_RET(d_ptr, equip_tmp);
    Q_D(StaffDivision);

    return(d->equip);
}

void StaffDivision::chgEquipment(StaffUnitEquipment::EquipmentType eqtype, QString type, int new_value, int category)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    d->equip.get(eqtype)[type]->get(category) = new_value;
}

void StaffDivision::incEquipment(StaffUnitEquipment::EquipmentType eqtype, QString type, int inc_value, int category)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    d->equip.get(eqtype)[type]->get(category) += inc_value;
}


void StaffDivision::initEquipment(StaffObject obj)
{
    CHECK_D(d_ptr);
    Q_D(StaffDivision);

    d->equip = StaffUnitEquipment();

    if(obj.key() == "МСБ"){
        qDebug();
    }

    if(obj.isDivision()){
        StaffObjectList obj_lst = obj.toDivision().objects();
        for(int i = 0; i < obj_lst.size(); ++i){
            StaffObject subobj = obj_lst[i];
            if(subobj.isBaseUnit()){
                d->equip += subobj.toBaseUnit().equipment();
            }else if(subobj.isDivision()){
                subobj.toDivision().initEquipment(subobj.toDivision());
                d->equip += subobj.toDivision().equipmentConst();
            }
        }
    }
}

// }}} [DIVISIONS METHODS]
// [CHECK PROPERTIES]: {{{

bool StaffDivision::hasSoldiers() const
{
    CHECK_D_RET(d_ptr, false);
    Q_D(const StaffDivision);

    return( !d->soldiers.isEmpty() );
}


bool StaffDivision::hasCommander() const
{
    CHECK_D_RET(d_ptr, false);
    Q_D(const StaffDivision);

    return( !d->commander.isNull() );
}


bool StaffDivision::hasDivisions() const
{
    CHECK_D_RET(d_ptr, false);
    Q_D(const StaffDivision);

    return( !d->divisions.isEmpty() );
}


bool StaffDivision::hasObjects() const
{
    CHECK_D_RET(d_ptr, false);
    Q_D(const StaffDivision);

    return( !d->divisions.isEmpty() || !d->soldiers.isEmpty() || !d->commander.isNull() );
}


bool StaffDivision::hasParentDivision() const
{
    CHECK_D_RET(d_ptr, false);
    Q_D(const StaffDivision);

    return( d->parentDivision != nullptr );
}


bool StaffDivision::isEmpty() const
{
    CHECK_D_RET(d_ptr, false);
    Q_D(const StaffDivision);

    return( d->divisions.isEmpty() && d->soldiers.isEmpty() && d->commander.isNull() );
}

// }}} [CHECK PROPERTIES]
// [CHECK METHODS]: {{{

bool StaffDivision::isParentDivision(const StaffDivision &division) const
{
    CHECK_D_RET(d_ptr, false);
    Q_D(const StaffDivision);

    if( division.isNull() ) return(false);
    return( d == division.d_func()->parentDivision );
}


bool StaffDivision::contains(const StaffObject &object)
{
    CHECK_D_RET(d_ptr, false);
    Q_D(StaffDivision);

    if( object.isDivision() ) return( d->divisions.contains(object.toDivision()) );
    if( object.isSoldier() ) return( d->soldiers.contains(object.toSoldier()) );
    if( object.isCommander() ) return( d->commander == (object.toCommander()) );

    return( false );
}


bool StaffDivision::containsDivision(const StaffDivision &division) const
{
    CHECK_D_RET(d_ptr, false);
    Q_D(const StaffDivision);

    if( division.isNull() || d->divisions.isEmpty() ) return(false);
    return( d->divisions.contains(division) );
}


bool StaffDivision::containsSoldier(const StaffSoldier &soldier) const
{
    CHECK_D_RET(d_ptr, false);
    Q_D(const StaffDivision);

    if( soldier.isNull() || d->soldiers.isEmpty() ) return(false);
    return( d->soldiers.contains(soldier) );
}

// }}} [CHECK METHODS]
