#ifndef STAFF_TYPES_H
#define STAFF_TYPES_H

#include <global1.h>
#include <QtGlobal>
#include <QtCore/qstring.h>
#include <QtCore/qlist.h>
#include <QtCore/qvector.h>
#include <QVariant>
#include <QPoint>
#include <memory>

using namespace std;


// [ Адрес проживания (место жительства) ]
/*
    street, streetNumber, flatNumber, homePhone
    city
    country
    zipCode
*/
struct StaffUnitStreetAddress
{
public:
    StaffUnitStreetAddress()
    { zipCode = 0; }

    StaffUnitStreetAddress(const StaffUnitStreetAddress& other):
        country(other.country), city(other.city), street(other.street), streetNumber(other.streetNumber)
      , homePhone(other.homePhone), flatNumber(other.flatNumber), zipCode(other.zipCode)
      , full( other.full )
    { }

    StaffUnitStreetAddress(QVariant data);
    QVariant toVariant() const;

public:
    QString country;
    QString city;
    QString street;
    QString streetNumber;
    QString homePhone;
    QString flatNumber;
    int zipCode;
    QString full;
};


// [ Боезапас/вооружение ]:

struct StaffUnitEquipmentEntity
{
public:
    StaffUnitEquipmentEntity(int stateCount = 0, int currentCount = 0):
        state_count(stateCount), current_count(currentCount)
    { }

    inline qreal levelOfEquipment() const
    { return(current_count / static_cast<qreal>(state_count)); }

    virtual QString toString() const{
        return(QString("%1/%2").arg(current_count).arg(state_count));
    }

    virtual StaffUnitEquipmentEntity& operator +=(const StaffUnitEquipmentEntity& rhs);
    virtual int& get(int column);
    virtual int  getConst(int column) const;

public:
    int state_count; // кол-во по штату
    int current_count; // текущее кол-во
};

class StaffUnitEquipmentEntityExt: public StaffUnitEquipmentEntity
{
public:
    StaffUnitEquipmentEntityExt(int stateCount = 0, int num_of_rounds = 1, int currentCount = 0):
        StaffUnitEquipmentEntity(stateCount, currentCount),
        volume_of_entity(num_of_rounds){}

    int  getVolumeOfEntity() const{ return volume_of_entity; }
    void chgVolumeOfEntity(int new_volume){ volume_of_entity = new_volume; }
    QString toString() const{
        return(QString("%1 x %2").arg(StaffUnitEquipmentEntity::toString()).arg(volume_of_entity));
    }

private:
    int volume_of_entity; // размер единицы снаряжения(например, для боекомплекта - количество патронов в одном боекомплекте,
                          // для топлива - объем канистры с топливом)
};


struct StaffUnitEquipment
{
public:
    enum EquipmentType{ Ammunition, Weapon, LogisticMeans, EquipmentTypesCount };

    StaffUnitEquipment()
    { }

    StaffUnitEquipment(const StaffUnitEquipment& other):
        ammunition(other.ammunition), logistic_means(other.logistic_means)
      , weapons( other.weapons )
    { }

    StaffUnitEquipment(QVariant weapons, QVariant ammunition = QVariant(), QVariant logistic_means = QVariant())
    { weaponsFromVariant(weapons); ammunitionFromVariant(ammunition); logisticMeansFromVariant(logistic_means); }

    void ammunitionFromVariant(const QVariant v);
    void logisticMeansFromVariant(const QVariant v);
    void weaponsFromVariant(const QVariant v);

    QString toString() const;

    StaffUnitEquipment& operator +=(const StaffUnitEquipment& rhs);
//    const QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >& operator[](EquipmentType type) const;
#ifdef QT4_ETU
    const QHash<QString, StaffUnitEquipmentEntity* >& getConst(EquipmentType type) const;
    QHash<QString, StaffUnitEquipmentEntity* >& get(EquipmentType type);
#else
    const QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >& getConst(EquipmentType type) const;
    QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >& get(EquipmentType type);
#endif


private:
#ifdef QT4_ETU
    void add(QHash<QString, StaffUnitEquipmentEntity* > &res,
             const QHash<QString, StaffUnitEquipmentEntity* > &summand);
#else
    void add(QHash<QString, shared_ptr<StaffUnitEquipmentEntity> > &res,
             const QHash<QString, shared_ptr<StaffUnitEquipmentEntity> > &summand);
#endif

public:
#ifdef QT4_ETU
    QHash<QString, StaffUnitEquipmentEntity* >  ammunition; // боеприпасы
    QHash<QString, StaffUnitEquipmentEntity* >  logistic_means; // материально-технические средства
    QHash<QString, StaffUnitEquipmentEntity* >  weapons; // вооружение
#else
    QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >  ammunition; // боеприпасы
    QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >  logistic_means; // материально-технические средства
    QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >  weapons; // вооружение
#endif
};

bool isExtendedEntity(StaffUnitEquipment::EquipmentType type);

#endif // STAFF_TYPES_H
