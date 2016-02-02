#include "staff_types.h"
#include <vector>

using namespace std;

QString entityToString(const QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >& entity);

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
        ammunition[type] = shared_ptr<StaffUnitEquipmentEntity>(new StaffUnitEquipmentEntityExt(state_cnt, num_of_rounds));
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
        logistic_means[type] = shared_ptr<StaffUnitEquipmentEntity>(new StaffUnitEquipmentEntityExt(state_cnt, num_of_rounds));
    }

}

void StaffUnitEquipment::weaponsFromVariant(const QVariant v)
{
    weapons.clear();
    QVariantMap mw = v.toMap();
    for(QString type: mw.keys()){
        int state_cnt = mw[type].toInt();
        weapons[type] = shared_ptr<StaffUnitEquipmentEntity>(new StaffUnitEquipmentEntity(state_cnt));
    }
}

QString entityToString(const QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >& entity)
{
    QString s;
    for(auto it = entity.begin(); it != entity.end(); ++it){
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

void StaffUnitEquipment::add(QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >& res,
                             const QHash<QString, shared_ptr<StaffUnitEquipmentEntity> >& summand)
{
    for(auto it = summand.cbegin(); it != summand.cend(); ++it){
        if(res.contains(it.key())){         // если такой тип снаряжения существует, то
            *(res[it.key()]) += *(it.value());    // добавляем его в имеющийся
        }else
            res.insert(it.key(), shared_ptr<StaffUnitEquipmentEntity>(new StaffUnitEquipmentEntity(*(it.value()) ) ) ); // иначе добавляем новый элемент
    }
}

StaffUnitEquipment &StaffUnitEquipment::operator +=(const StaffUnitEquipment &rhs)
{
    add(ammunition, rhs.ammunition);
    add(weapons, rhs.weapons);
    add(logistic_means, rhs.logistic_means);

    return(*this);
}

const QHash<QString, shared_ptr<StaffUnitEquipmentEntity> > &StaffUnitEquipment::getConst(StaffUnitEquipment::EquipmentType type) const
{
    static QHash<QString, shared_ptr<StaffUnitEquipmentEntity> > fake_hash;
    switch (type) {
    case Ammunition:    return ammunition;
    case Weapon:        return weapons;
    case LogisticMeans: return logistic_means;
    default:            return(fake_hash);
    }
}

QHash<QString, shared_ptr<StaffUnitEquipmentEntity> > &StaffUnitEquipment::get(StaffUnitEquipment::EquipmentType type)
{
    static QHash<QString, shared_ptr<StaffUnitEquipmentEntity> > fake_hash;
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
