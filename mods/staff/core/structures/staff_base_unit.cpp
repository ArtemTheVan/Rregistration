#include "staff_base_unit.h"
#include "staff_base_unit_p.h"

#include "staff_division.h"


// [ INIT/DESTROY ]: {{{

StaffBaseUnit::StaffBaseUnit():
    StaffObject()
{
    //
}


StaffBaseUnit::StaffBaseUnit(const StaffBaseUnit &other):
    StaffObject(other)
{
    //
}


StaffBaseUnit &StaffBaseUnit::operator=(const StaffBaseUnit &other)
{
    return (StaffBaseUnit&) StaffObject::operator=(other);
}


StaffBaseUnit::StaffBaseUnit(StaffBaseUnitPrivate *dd):
    StaffObject(dd)
{
    //
}


StaffBaseUnit::~StaffBaseUnit()
{
    //
}


StaffBaseUnit StaffBaseUnit::clone(bool deep)
{
    if( d_ptr == nullptr )
        return StaffBaseUnit();
    Q_D(StaffBaseUnit);

    return StaffBaseUnit(d->clone(deep));
}


StaffBaseUnit StaffBaseUnit::copy()
{
    CHECK_D_RET(d_ptr, StaffBaseUnit());
    Q_D(StaffBaseUnit);

    return StaffBaseUnit(d->copy());
}


void StaffBaseUnit::create()
{
    if( d_ptr == nullptr ) {
        d_ptr = (StaffObjectPrivate*)(new StaffBaseUnitPrivate());
    }
}

// [ PROPERTIES ]: {{{

// [ NAME DUTY ]:

QString StaffBaseUnit::dutyName() const
{
    CHECK_D_RET(d_ptr, QString());
    Q_D(const StaffBaseUnit);

    return(d->m_nameDuty);
}


void StaffBaseUnit::setDutyName(const QString &nameDuty)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->setDutyName(nameDuty);
}

// [ ПОРЯДКОВЫЙ НОМЕР (по иерархии) ]:

void StaffBaseUnit::setNumber(int unitNumber)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->setNumber(unitNumber);
}

// [ NAME ]:

void StaffBaseUnit::setName(const QString &name)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->setName(name);
}


void StaffBaseUnit::setName(const QString &dutyName, const QString &divisionName, int unitNumber)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->setName( dutyName, divisionName, unitNumber );
}

// [ ФИО ]

void StaffBaseUnit::setUnitFIO(const QString &fio)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->setFIO(fio);
}

QString StaffBaseUnit::FIO() const
{
    CHECK_D_RET(d_ptr, QString());
    Q_D(const StaffBaseUnit);

    return( d->FIO.full );
}

// [ ДАТА И МЕСТО РОЖДЕНИЯ ]:

void StaffBaseUnit::setBirthDate(const QDate &date)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    if( d->birth.date != date )
        d->birth.date = date;
}


void StaffBaseUnit::setBirthDate(const QString &date)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->birth.date = QDate::fromString(date);
}


QDate StaffBaseUnit::birthDate() const
{
    CHECK_D_RET(d_ptr, QDate());
    Q_D(const StaffBaseUnit);

    return( d->birth.date );
}


void StaffBaseUnit::setBirthPlace(const QString &birthPlace)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->birth.place = birthPlace;
}

QString StaffBaseUnit::birthPlace() const
{
    CHECK_D_RET(d_ptr, QString());
    Q_D(const StaffBaseUnit);

    return( d->birth.place );
}

// [ Адрес проживания ]

void StaffBaseUnit::setStreetAddress(const StaffUnitStreetAddress &streetAddress)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->setStreetAddress(streetAddress);
}

StaffUnitStreetAddress StaffBaseUnit::streetAddress() const
{
    CHECK_D_RET(d_ptr, StaffUnitStreetAddress());
    Q_D(const StaffBaseUnit);

    return *( d->m_streetAddress.data() );
}

// [ ВУС ]

void StaffBaseUnit::setSpecialty(const QString &specialty)
{
    Q_D(StaffBaseUnit);

    d->setSpecialty(specialty);
}

QString StaffBaseUnit::specialty() const
{
    CHECK_D_RET(d_ptr, QString());
    Q_D(const StaffBaseUnit);

    return( d->speciality.full );
}

// [ DIVISION NAME ]:

QString StaffBaseUnit::divisionName() const
{
    CHECK_D_RET(d_ptr, QString());
    Q_D(const StaffBaseUnit);

    return(d->m_nameDivision);
}


void StaffBaseUnit::setDivisionName(const QString &divisionName)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->setDivisionName(divisionName);
}

// [ FULL NAME ]:

void StaffBaseUnit::setFullPartName(const QString &fullPart)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->setFullPartName(fullPart);
}

// [ DESCRIPTION ]:

void StaffBaseUnit::setDescription(const QString &description)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    if( d->nameDescription != description ) {
        d->nameDescription = description;
    }
}

// [ ADDRESS ]:

QHostAddress StaffBaseUnit::netAddress() const
{
    CHECK_D_RET(d_ptr, QHostAddress(QHostAddress::Null));
    Q_D(const StaffBaseUnit);

    return(d->m_netAddress);
}


void StaffBaseUnit::setNetAddress(const QHostAddress &address)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    if( d->m_netAddress != address ) {
        d->setNetworkAddress(address);
    }
}

// [ РЕГИСТРАЦИЯ ]:

time_t StaffBaseUnit::registrationTime() const
{
    CHECK_D_RET(d_ptr, 0);
    Q_D(const StaffBaseUnit);

    return d->m_registrationTime;
}

void StaffBaseUnit::setRegistrationTime(time_t registrationTime)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->m_registrationTime = registrationTime;
}


void StaffBaseUnit::setRegistered(bool registered)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->setRegistrationState(registered);
}

bool StaffBaseUnit::isRegistered() const
{
    CHECK_D_RET(d_ptr, false);
    Q_D(const StaffBaseUnit);

    return( d->m_registered );
}

// [ UNIT DUTY ]:

staff::UnitDuty StaffBaseUnit::duty() const
{
    CHECK_D_RET(d_ptr, staff::UnknownDuty);
    Q_D(const StaffBaseUnit);

    return(d->m_duty);
}


void StaffBaseUnit::setDuty(staff::UnitDuty duty)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    if( d->m_duty != duty && duty != staff::UnknownDuty )
        d->m_duty = duty;
}

// [ UNIT RANK ]:

staff::UnitRank StaffBaseUnit::rank() const
{
    CHECK_D_RET(d_ptr, staff::UnknownRank);
    Q_D(const StaffBaseUnit);

    return(d->m_rank);
}


void StaffBaseUnit::setRank(staff::UnitRank rank)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->setRank(rank);
}

staff::Health StaffBaseUnit::health() const
{
    CHECK_D_RET(d_ptr, staff::UnknownHealth);
    Q_D(const StaffBaseUnit);

    return (d->health);
}

void StaffBaseUnit::setHealth(staff::Health h)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->health = h;
}


const StaffUnitEquipment& StaffBaseUnit::equipment() const
{
    CHECK_D_RET(d_ptr, StaffUnitEquipment());
    Q_D(const StaffBaseUnit);

    return *(d->m_equipment.data());
}

void StaffBaseUnit::setEquipment(const StaffUnitEquipment &equipment)
{
    CHECK_D(d_ptr);
    Q_D(StaffBaseUnit);

    d->setEquipment(equipment);
}


staff::RankCategory StaffBaseUnit::category() const
{
    CHECK_D_RET(d_ptr, staff::UnknownCategory);
    Q_D(const StaffBaseUnit);

    return( d->m_category );
}


StaffDivision StaffBaseUnit::division() const
{
    CHECK_D_RET(d_ptr, StaffDivision());
    Q_D(const StaffBaseUnit);

    return(StaffDivision(d->m_parentDivision));
}

// }}} [ PROPERTIES ]
// [ CHECK PROPERTIES ]: {{{

bool StaffBaseUnit::hasDivision() const
{
    CHECK_D_RET(d_ptr, false);
    Q_D(const StaffBaseUnit);

    return( d->m_parentDivision != nullptr );
}

// }}} [ CHECK PROPERTIES ]
