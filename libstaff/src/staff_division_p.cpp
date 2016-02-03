#include "staff_division_p.h"



// [ INIT/DESTROY ]: {{{

StaffDivisionPrivate::StaffDivisionPrivate(StaffDivisionPrivate *parent):
    StaffObjectPrivate()  
  , type(staff::UnknownDivision)
  , parentDivision(parent)
{
    //
    memset(countData, 0, sizeof(countData));
}


StaffDivisionPrivate::StaffDivisionPrivate(StaffDivisionPrivate *other, bool deep, bool copyLinks):
    StaffObjectPrivate(other, deep)
{
    type = other->type;

    if( copyLinks ) {
        parentDivision = other->parentDivision;

        if( !deep ) {
            commander = other->commander;
            soldiers = other->soldiers;
            divisions = other->divisions;
            return;
        }
    }
    // TODO: Сделать реализацию глобокого копирования
}


StaffDivisionPrivate::~StaffDivisionPrivate()
{
    // TODO: Сделать правильную реализацию
    /*
    StaffSoldierList::const_iterator s_it = soldiers.constBegin();
    while (s_it != soldiers.cend()) {
        s_it->d

        ++s_it;
    }
    */

    soldiers.clear();
    divisions.clear();
}


StaffDivisionPrivate *StaffDivisionPrivate::clone(bool deep)
{
    StaffDivisionPrivate* p = new StaffDivisionPrivate(this, deep);
    p->ref.deref();
    return p;
}


StaffDivisionPrivate *StaffDivisionPrivate::copy()
{
    StaffDivisionPrivate* p = new StaffDivisionPrivate(this, false, false);
    p->ref.deref();
    return p;
}


void StaffDivisionPrivate::clear()
{
    StaffObjectPrivate::clear();

    parentDivision = nullptr;
    type = staff::UnknownDivision;

    if( !commander.isNull() ) commander.setNull();
    if( soldiers.isEmpty() ) soldiers.clear();
    if( divisions.isEmpty() ) divisions.clear();
}



// }}} [ INIT/DESTROY ]
// [ PROPERTIES ]: {{{

void StaffDivisionPrivate::setName(const QString &baseName, int num)
{
    QString clearName = staff_private::formatName(baseName);

    if( number != num ) number = num;

    if( nameBase != clearName ) {
        nameBase = clearName;
        nameDescription = staff::getDivisionDescrByName(nameBase);
        updateName();
    }
}


void StaffDivisionPrivate::setNameFullPart(const QString &fullPart)
{
    QString clearName = staff_private::formatNameRight(fullPart);

    if( nameFullPart != clearName ) {
        nameFullPart = clearName;
        if( !name.isEmpty() ) {
            nameFull = QString("%1.%2").arg(name).arg(nameFullPart);
        }
    }
}


void StaffDivisionPrivate::setNumber(int num)
{
    if( number != num && num > 0 ) {
        number = num;
        if( !nameBase.isEmpty() ) {
            name = QString("%1-%2").arg(number).arg(nameBase);
            if( !nameFullPart.isEmpty() ) {
                nameFull = QString("%1.%2").arg(name).arg(nameFullPart);
            } else {
                nameFull = name;
            }
        }
    }
}


void StaffDivisionPrivate::updateName()
{
    if( nameBase.isEmpty() ) return;

    if( number > 0 ) name = QString("%1-%2").arg(number).arg(nameBase);
    else name = nameBase;

    if( !nameFullPart.isEmpty() ) {
        nameFull = QString("%1.%2").arg(name).arg(nameFullPart);
    } else nameFull = name;
}


void StaffDivisionPrivate::updateSoldiersCommander()
{
    if( soldiers.isEmpty() ) return;
    int i, cnt = soldiers.count();

    for( i = 0; i < cnt; ++i ) {
        soldiers[i].d_func()->setCommander(commander.d_func());
    }

}


void StaffDivisionPrivate::setParentDivision(StaffDivisionPrivate *p)
{
    if( p == nullptr ) return;

    parentDivision = p;
    if( !p->nameFull.isEmpty() ) {
        nameFullPart = p->nameFull;
        if( !name.isEmpty() ) {
            nameFull = QString("%1.%2").arg(name).arg(nameFullPart);
        }
    }
}


int StaffDivisionPrivate::count(staff::RankCategory category)
{
    if( soldiers.isEmpty() ) return(0);
    int i, result = 0, count = soldiers.count();

    for( i = 0; i < count; ++i ) {
        //qDebug() << "Category: " << soldiers[i].category();
        if( soldiers[i].category() == category )
            ++result;
    }

    return(result);
}


void StaffDivisionPrivate::addSoldier(StaffSoldierPrivate *soldier)
{
    /*
    if( !soldiers.contains(soldier) )
    {
        soldier->(d->name);
        soldier.setFullPartName( d->nameFullPart );
        soldier.d_func()->m_parentDivision = d;
        soldier.d_func()->setCommander(d->commander.d_func());

        d->soldiers.append(soldier);
        return(true);
    }*/
}
