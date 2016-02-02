#ifndef STAFF_BASE_UNIT_P_H
#define STAFF_BASE_UNIT_P_H

#include "staff_base_unit.h"
#include "staff_object_p.h"
//#include <vector>


using namespace std;

class StaffDivisionPrivate;


class StaffBaseUnitPrivate: public StaffObjectPrivate
{
public:
    StaffBaseUnitPrivate();
    StaffBaseUnitPrivate(StaffBaseUnitPrivate* other, bool deep, bool copyLinks = false);
    virtual ~StaffBaseUnitPrivate();

    virtual StaffBaseUnitPrivate *clone(bool deep = true);
    virtual StaffBaseUnitPrivate* copy();

public:
    virtual StaffObject::StaffType staffType() const { return StaffObject::BaseUnitType; }

public:
    void setName(const QString& unitName);
    void setName(const QString& dutyName, const QString& divisionName, int unitNumber = 0);
    void setDutyName(const QString& unitDutyName);

    void setFullPartName(const QString& fullPart);
    void setNumber(int unitNumber);

    void setDivisionName(const QString& divisionName);

    void setFIO(const QString& fio);
    void setSpecialty(const QString& spec);

    void setRank(staff::UnitRank rank);

    void setStreetAddress(const StaffUnitStreetAddress& m_streetAddress);
    void setEquipment(const StaffUnitEquipment& equipment);

    void setParentDivision(StaffDivisionPrivate* division);

    void setRegistrationState(bool registered);
    void setNetworkAddress(const QHostAddress& address);

public:
    void updateNameBase();
    void updateName();

public:    
    QString         m_nameDuty;             // П | К | С | СС ....
    QString         m_nameDivision;         // МСО | 1-МСО | 2-МСВ ...

    staff::UnitDuty     m_duty;             // Должность (перечисление)
    staff::UnitRank     m_rank;             // Звание (перечисление)
    staff::RankCategory m_category;

    QHostAddress        m_netAddress;       // IP-адрес согласно адресной книге
    time_t              m_registrationTime; // Время регистрации (UNIX TIMESTAMP)
    bool                m_registered;

    struct __FIO {
        QString name;
        QString surname;
        QString patronymic;
        QString full;

    } FIO;

    struct __SPECIALITY {
        int number;
        int dutyCode;
        QChar attr;
        QString full;

    } speciality;

    struct __BIRTH_INFO {
        QDate date;
        QString place;
    } birth;


    QSharedPointer<StaffUnitStreetAddress> m_streetAddress;
    QSharedPointer<StaffUnitEquipment> m_equipment;

    staff::Health health;
//    bool isPresent; // Присутствие военнослужащего(по факту - на построении)

    StaffDivisionPrivate*   m_parentDivision;
};

// <StaffObjectPrivate>:
//         nameBase;       // П | [1,2,3...]-П .... (Должностное имя с учетом порядкового номера)
//         name;           // П.1-МСО | 2-П.1-МСВ ...
//         nameFull;       // П.1-МСО.2-МСВ.1-МСР.1-МСБ ...
//         nameFullPart;   // 2-МСВ.1-МСР.1-МСБ ... | 1-МСР.1-МСБ.1-МСБ
//         description;    // Пулемтчик, Снайпер, Командир и т.д.
//         number;         // Порядковый номер по должности в подразделении, т.е.: 1-П, 2-П ит.п.

#endif // STAFF_BASE_UNIT_P_H
