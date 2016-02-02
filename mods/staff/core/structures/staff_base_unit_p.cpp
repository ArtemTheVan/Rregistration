#include "staff_base_unit_p.h"

#include "staff_division_p.h"

#include "esu_staff_engine.h"
#include "staff_globals.h"


// [ INIT/DESTROY ]: {{{

StaffBaseUnitPrivate::StaffBaseUnitPrivate():
    StaffObjectPrivate()
  , m_nameDuty(""), m_nameDivision("")
  , m_duty(staff::UnknownDuty), m_rank(staff::SoldierRank)
  , m_category(getRankCategoryByRank(m_rank))
  , m_netAddress(QHostAddress::Null)
  , m_registered(false)
  , m_parentDivision(nullptr)
{
    setStreetAddress(StaffUnitStreetAddress());
    setEquipment(StaffUnitEquipment());
    birth.date = QDate(0, 0, 0);
    birth.place = "";
}


StaffBaseUnitPrivate::StaffBaseUnitPrivate(StaffBaseUnitPrivate *other, bool deep, bool copyLinks):
    StaffObjectPrivate(other, deep)
{
    m_nameDuty      = other->m_nameDuty;
    m_nameDivision  = other->m_nameDivision;
    m_duty          = other->m_duty;
    m_rank          = other->m_rank;
    m_category      = getRankCategoryByRank(other->m_rank);
    m_netAddress    = other->m_netAddress;

    FIO.full        = other->FIO.full;
    FIO.name        = other->FIO.name;
    FIO.surname     = other->FIO.surname;
    FIO.patronymic  = other->FIO.patronymic;

    if( !m_streetAddress.isNull() ) {
        *(m_streetAddress.data()) = *(other->m_streetAddress.data());
    }

    speciality.full = other->speciality.full;
    speciality.attr = other->speciality.attr;
    speciality.dutyCode = other->speciality.dutyCode;
    speciality.number = other->speciality.number;

    if( copyLinks ) {
        m_parentDivision = other->m_parentDivision;
    } else if( deep ) {
        // При глубоком клонировании сбрасываются все ссылки на радительские и соседние элементы.
        // Клонирование всегда идет по нисходящей (т.е. вниз), а соседи и родители сбрасываются
        m_parentDivision = nullptr;
    }
}


StaffBaseUnitPrivate::~StaffBaseUnitPrivate()
{
    //
}


StaffBaseUnitPrivate *StaffBaseUnitPrivate::clone(bool deep)
{
    StaffBaseUnitPrivate* p = new StaffBaseUnitPrivate(this, deep);
    p->ref.deref();
    return p;
}


StaffBaseUnitPrivate *StaffBaseUnitPrivate::copy()
{
    StaffBaseUnitPrivate* p = new StaffBaseUnitPrivate(this, true, false);
    p->ref.deref();
    return p;
}

// }}} [ INIT/DESTROY ]
// [ MAIN PROPERTIES ]: {{{

void StaffBaseUnitPrivate::setName(const QString &unitName)
{
    int partsCount = 0, dutyNameNum = 0;
    QString nameDuty;
    QString clearName = staff_private::formatName(unitName);
    QStringList nameParts, nameSplit;

    // [ Берем основное имя ]:
    partsCount = clearName.count('.');
    if( partsCount >= 1 ) {
        nameParts = clearName.split(QChar('.'));
        nameDuty = nameParts.first(); nameParts.removeFirst();
    } else {
        nameDuty = clearName;
    }

    // [ Разбираем первую часть имени (1-П/П) ]:
    nameSplit = nameDuty.split(QChar('-'));
    dutyNameNum = nameSplit.count();

    // Проверяем порядковый номер (по иерархии)
    if( dutyNameNum > 0 ) {
        number = nameSplit.first().toInt(); nameSplit.removeFirst();
        m_nameDuty = nameSplit.first();
    } else {
        m_nameDuty = nameDuty;
    }

    // [ Разбираем имя подразделения ]:
    if( partsCount >= 1 ) {
        m_nameDivision = nameParts.first(); nameParts.removeFirst();
    }

    if( partsCount > 1 ) {
        // [ Полное имя: К.1-МСО.2-МСВ.МСР ]
        nameFullPart = nameParts.join(".");
    }
}


void StaffBaseUnitPrivate::setName(const QString &dutyName, const QString &divisionName, int unitNumber)
{
    if( dutyName.isEmpty() && divisionName.isEmpty() ) return;
    if( m_nameDuty == dutyName && m_nameDivision == divisionName ) return;

    if( unitNumber > 0 ) number = unitNumber;
    m_nameDuty = staff_private::formatName(dutyName);
    m_nameDivision = staff_private::formatName(divisionName);

    updateNameBase();
}


void StaffBaseUnitPrivate::setDutyName(const QString &unitDutyName)
{
    m_nameDuty = staff_private::formatName(unitDutyName);
    updateNameBase();
}


void StaffBaseUnitPrivate::setFullPartName(const QString &fullPart)
{
    QString clearName = staff_private::formatNameRight(fullPart);

    if( nameFullPart != clearName ) {
        nameFullPart = clearName;
        if( !name.isEmpty() ) {
            nameFull = QString("%1.%2").arg(name).arg(nameFullPart);
        }
    }
}


void StaffBaseUnitPrivate::setNumber(int unitNumber)
{
    if( number != unitNumber ) {
        number = unitNumber;
        updateNameBase();
    }
}


void StaffBaseUnitPrivate::setDivisionName(const QString &divisionName)
{
    if( m_nameDivision != divisionName && !divisionName.isEmpty() )
    {
        m_nameDivision = staff_private::formatNameRight(divisionName);
        updateName();
    }
}


void StaffBaseUnitPrivate::setFIO(const QString &fio)
{
    if( FIO.full != fio ) {
        FIO.full = fio;
    }
}


void StaffBaseUnitPrivate::setSpecialty(const QString &spec)
{
    if( speciality.full != spec ) {
        speciality.full = spec;
    }
}


void StaffBaseUnitPrivate::setRank(staff::UnitRank rank)
{
    if( m_rank != rank ) {
        m_rank = rank;
        m_category = getRankCategoryByRank(m_rank);
    }
}


void StaffBaseUnitPrivate::setStreetAddress(const StaffUnitStreetAddress &streetAddress)
{
    if( m_streetAddress.isNull() ) {
        StaffUnitStreetAddress* pData = new StaffUnitStreetAddress(streetAddress);
        m_streetAddress = QSharedPointer<StaffUnitStreetAddress>(pData);
    } else {
        *(m_streetAddress.data()) = streetAddress;
    }
}


void StaffBaseUnitPrivate::setEquipment(const StaffUnitEquipment &equipment)
{
    if( m_equipment.isNull() ) {
        StaffUnitEquipment* pData = new StaffUnitEquipment(equipment);
        m_equipment = QSharedPointer<StaffUnitEquipment>(pData);
    } else {
        *(m_equipment.data()) = equipment;
    }
}


void StaffBaseUnitPrivate::setParentDivision(StaffDivisionPrivate *division)
{
    if( !division || division == m_parentDivision ) return;
    m_parentDivision = division;

    m_nameDivision = m_parentDivision->name;
    nameFullPart = m_parentDivision->nameFullPart;

    if( !m_nameDivision.isEmpty() ) {
        name = QString("%1.%2").arg(nameBase).arg(m_nameDivision);
        if( !nameFullPart.isEmpty() ) {
            nameFull = QString("%1.%2").arg(name).arg(nameFullPart);
        }
    } else {
        name = nameBase;
        nameFull = name;
    }
}


void StaffBaseUnitPrivate::setRegistrationState(bool registered)
{
    m_registered = registered;

    if( engine != nullptr )
    {
        if( m_registered ) {
            if( !engine->nodesRegistered.contains(nameFull) )
                engine->nodesRegistered.insert(nameFull, this);
        } else {
            if( engine->nodesRegistered.contains(nameFull) )
                engine->nodesRegistered.remove(nameFull);
        }
    }
}

void StaffBaseUnitPrivate::setNetworkAddress(const QHostAddress &address)
{
    m_netAddress = address;
    if( !engine ) return;

    QList<StaffObjectPrivate*> nodes = engine->nodesNetAddress.values();
    bool containsNode = nodes.contains((StaffObjectPrivate*)this);

    if( nodes.isEmpty() && !m_netAddress.isNull() ) {
         engine->nodesNetAddress.insert(m_netAddress, (StaffObjectPrivate*)this);
         return;
    } else if(  nodes.isEmpty() && m_netAddress.isNull() ) {
        return;
    }

    if( !m_netAddress.isNull() && !engine->nodesNetAddress.contains(m_netAddress) )
    {
        if( containsNode ) {
            // Изменение NET адреса
            for( QHash<QHostAddress, StaffObjectPrivate*>::iterator it = engine->nodesNetAddress.begin(); it != engine->nodesNetAddress.end(); ++it )
            {
                if( this == it.value() ) {
                    it = engine->nodesNetAddress.erase(it);
                    engine->nodesNetAddress.insert(m_netAddress, (StaffObjectPrivate*)this);
                    break;
                }
            }
        } else {
            // Добавление нового узла
            engine->nodesNetAddress.insert(m_netAddress, (StaffObjectPrivate*)this);
        }
    } else if( m_netAddress.isNull() && containsNode ) {
        // Удаление существующего узла
        for( QHash<QHostAddress, StaffObjectPrivate*>::iterator it = engine->nodesNetAddress.begin(); it != engine->nodesNetAddress.end(); ++it )
        {
            if( this == it.value() ) {
                it = engine->nodesNetAddress.erase(it);
                break;
            }
        }
    }
}

// }}} [ MAIN PROPERTIES ]
// [ HELPERS METHODS ]: {{{

void StaffBaseUnitPrivate::updateNameBase()
{
    if( m_nameDuty.isEmpty() ) return;

    nameDescription = staff::getDutyDescrByName(m_nameDuty);
    m_duty = staff::getUnitDutyByDescr(nameDescription);

    if( number > 0 ) {
        nameBase = QString("%1-%2").arg(number).arg(m_nameDuty);
    } else nameBase = m_nameDuty;

    if( !m_nameDivision.isEmpty() ) {
        name = QString("%1.%2").arg(nameBase).arg(m_nameDivision);

        if( !nameFullPart.isEmpty() ) {
            nameFull = QString("%1.%2").arg(name).arg(nameFullPart);
        }
    } else {
        name = nameBase;
        nameFull = name;
    }
}


void StaffBaseUnitPrivate::updateName()
{
    if( nameBase.isEmpty() || m_nameDivision.isEmpty() ) return;

    name = QString("%1.%2").arg(nameBase).arg(m_nameDivision);
    if( !nameFullPart.isEmpty() ) {
        nameFull = QString("%1.%2").arg(name).arg(nameFullPart);
    } else {
        nameFull = name;
    }
}

// }}} [ HELPERS METHODS ]
