#include "staff_types.h"
#include <vector>

using namespace std;

#ifdef QT4_ETU
QString entityToString(const QHash<QString, StaffUnitEquipmentEntity* >& entity);
#else
QString entityToString(const QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >& entity);
#endif

StaffUnitStreetAddress::StaffUnitStreetAddress(QVariant data)
{
    full = data.toString();

    // Разбор строки
    /*          0        1       2       3
0      ул. Лесная, д. 5, кв. 176, т.34567890
1      Москва
2      Россия
3      125075
*/
    vector<QString> s;
    // Первая строка
    for(int i = 0; i <= 3; ++i)
        s.push_back(full.section(',', i, i));

    // Остальные 3 строки
    for(int i = 1; i <= 3; ++i)
        s.push_back(full.section('\n', i, i));

    street       = s[0];
    streetNumber = s[1];
    flatNumber   = s[2];
    homePhone    = s[3];
    city         = s[4];
    country      = s[5];
    zipCode      = s[6].toInt();
}

QVariant StaffUnitStreetAddress::toVariant() const
{
    return (QVariant(full));
}


void StaffUnitEquipment::ammunitionFromVariant(const QVariant v)
{
    ammunition.clear();
    QVariantMap m = v.toMap();
    for(int i = 0; i < m.keys().size(); ++i){
        QString type = m.keys()[i];
        QPoint p = m[type].toPoint();
        int state_cnt = p.x();
        int num_of_rounds = p.y();
#ifdef QT4_ETU
        ammunition[type] = (StaffUnitEquipmentEntity *)(new StaffUnitEquipmentEntityExt(state_cnt, num_of_rounds));
#else
        ammunition[type] = shared_ptr<StaffUnitEquipmentEntity>(new StaffUnitEquipmentEntityExt(state_cnt, num_of_rounds));
#endif
    }
}

void StaffUnitEquipment::logisticMeansFromVariant(const QVariant v)
{
    logistic_means.clear();
    QVariantMap m = v.toMap();
    for(int i = 0; i < m.keys().size(); ++i){
        QString type = m.keys()[i];
        QPoint p = m[type].toPoint();
        int state_cnt = p.x();
        int num_of_rounds = p.y();
#ifdef QT4_ETU
        logistic_means[type] = (StaffUnitEquipmentEntity *)(new StaffUnitEquipmentEntityExt(state_cnt, num_of_rounds));
#else
        logistic_means[type] = shared_ptr<StaffUnitEquipmentEntity>(new StaffUnitEquipmentEntityExt(state_cnt, num_of_rounds));
#endif
    }

}

void StaffUnitEquipment::weaponsFromVariant(const QVariant v)
{
    weapons.clear();
    QVariantMap mw = v.toMap();
    foreach (QString type, mw.keys()) {
        int state_cnt = mw[type].toInt();
#ifdef QT4_ETU
        weapons[type] = (StaffUnitEquipmentEntity *)(new StaffUnitEquipmentEntity(state_cnt));
#else
        weapons[type] = shared_ptr<StaffUnitEquipmentEntity>(new StaffUnitEquipmentEntity(state_cnt));
#endif
    }
}

#ifdef QT4_ETU
QString entityToString(const QHash<QString, StaffUnitEquipmentEntity* >& entity)
#else
QString entityToString(const QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >& entity)
#endif
{
    QString s;
    for(QHash<QString, StaffUnitEquipmentEntity*>::const_iterator it = entity.begin(); it != entity.end(); ++it) {
        s += QString("{%1: %2}, ").arg(it.key()).arg((*it)->toString());
    }
    return(s);
}

QString StaffUnitEquipment::toString() const
{
    QString ammo_str = entityToString(ammunition);
    QString weapon_str = entityToString(weapons);
    QString logmeans_str = entityToString(logistic_means);
    QString s = QString("Б/П: %1ВВТ: %2МТС: %3").arg(ammo_str).arg(weapon_str).arg(logmeans_str);

    return(s);
}

#ifdef QT4_ETU
void StaffUnitEquipment::add(QHash<QString, StaffUnitEquipmentEntity* >& res,
                             const QHash<QString, StaffUnitEquipmentEntity* >& summand)
#else
void StaffUnitEquipment::add(QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >& res,
                             const QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >& summand)
#endif
{
    for(QHash<QString, StaffUnitEquipmentEntity* >::const_iterator it = summand.begin(); it != summand.end(); ++it) {
        if(res.contains(it.key())) {         // если такой тип снаряжения существует, то
            *(res[it.key()]) += *(it.value());    // добавляем его в имеющийся
        } else
#ifdef QT4_ETU
            res.insert(it.key(), (StaffUnitEquipmentEntity *)(new StaffUnitEquipmentEntity(*(it.value()) ) ) ); // иначе добавляем новый элемент
#else
            res.insert(it.key(), shared_ptr<StaffUnitEquipmentEntity>(new StaffUnitEquipmentEntity(*(it.value()) ) ) ); // иначе добавляем новый элемент
#endif
    }
}

StaffUnitEquipment &StaffUnitEquipment::operator +=(const StaffUnitEquipment &rhs)
{
    add(ammunition, rhs.ammunition);
    add(weapons, rhs.weapons);
    add(logistic_means, rhs.logistic_means);

    return(*this);
}

#ifdef QT4_ETU
const QHash<QString, StaffUnitEquipmentEntity* > &StaffUnitEquipment::getConst(StaffUnitEquipment::EquipmentType type) const
{
    static QHash<QString, StaffUnitEquipmentEntity* > fake_hash;
#else
const QHash<QString, shared_ptr<StaffUnitEquipmentEntity> > &StaffUnitEquipment::getConst(StaffUnitEquipment::EquipmentType type) const
{
    static QHash<QString, shared_ptr<StaffUnitEquipmentEntity> > fake_hash;
#endif
    switch (type) {
    case Ammunition:    return ammunition;
    case Weapon:        return weapons;
    case LogisticMeans: return logistic_means;
    default:            return(fake_hash);
    }
}

#ifdef QT4_ETU
QHash<QString, StaffUnitEquipmentEntity* > &StaffUnitEquipment::get(StaffUnitEquipment::EquipmentType type)
{
    static QHash<QString, StaffUnitEquipmentEntity* > fake_hash;
#else
QHash<QString, shared_ptr<StaffUnitEquipmentEntity> > &StaffUnitEquipment::get(StaffUnitEquipment::EquipmentType type)
{
    static QHash<QString, shared_ptr<StaffUnitEquipmentEntity> > fake_hash;
#endif
    switch (type) {
    case Ammunition:    return ammunition;
    case Weapon:        return weapons;
    case LogisticMeans: return logistic_means;
    default:            return(fake_hash);
    }
}

StaffUnitEquipmentEntity &StaffUnitEquipmentEntity::operator +=(const StaffUnitEquipmentEntity &rhs)
{
    state_count += rhs.state_count;
    current_count += rhs.current_count;
    return(*this);
}

int &StaffUnitEquipmentEntity::get(int column)
{
    static int i = -1;
    switch (column) {
    /*models::StateCount*/
    case 1: return(state_count);
        /*models::CurCount*/
    case 2: return(current_count);
    default: return(i);
    }
}

int StaffUnitEquipmentEntity::getConst(int column) const
{
    static int i = -1;
    switch (column) {
    /*models::StateCount*/
    case 1: return(state_count);
        /*models::CurCount*/
    case 2: return(current_count);
    default: return(i);
    }
}


bool isExtendedEntity(StaffUnitEquipment::EquipmentType type)
{
    if(type == StaffUnitEquipment::Weapon) return(false);
    else                                   return(true);
}
