#ifndef ESUTZ_MILITARY_STAFF_H
#define ESUTZ_MILITARY_STAFF_H

#include <QtCore>

#include <interfaces/i_singleton.h>
#include "esu_staff_structure.h"


#define esuStaff ESUStaffSystem::instance()
#define staffESU ESUStaffSystem::instance()


class ESUStaffSystemPrivate;

class ESUStaffSystem : public QObject,
        public ISingleton<ESUStaffSystem>
{
    Q_OBJECT
    I_SINGLETON(ESUStaffSystem)

public:
    enum Type { UnitType, DivisionType };
    enum RoleType { SoldierRole, CommanderRole };

    Q_ENUMS(Type RoleType)

protected:
    explicit ESUStaffSystem();
    ~ESUStaffSystem();

public:
    static void declareQmlTypes();    

public:
    void loadData();
    ESUStaffStructure &data() const;

    void printDivisions();
    void printDivision(const StaffDivision& division, int level = 0, bool recursive = true);

public Q_SLOTS:
    // [ QML METHODS ]:

public:
    // [ IEventListener ]:
    quint32 ID() const      { return( ESU_STAFF_ID ); }
    QObject* toObject()     { return( this ); }

protected:
    // [ Q_EVENTS ]
    void customEvent(QEvent *event);

protected:
    ESUStaffSystemPrivate* d;

private:
    Q_DECLARE_PRIVATE_D(ESUStaffSystem::d, ESUStaffSystem)
    Q_DISABLE_COPY(ESUStaffSystem)
};

Q_DECLARE_METATYPE(ESUStaffSystem::Type)
Q_DECLARE_METATYPE(ESUStaffSystem::RoleType)


#endif // ESUTZ_MILITARY_STAFF_H
