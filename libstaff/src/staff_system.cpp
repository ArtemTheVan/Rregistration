#include "staff_system.h"
#include "staff_system_p.h"

#ifdef ESU_UI_QUICK
#include <QtQml>
#endif


// [ INIT/DESTROY ]: {{{

ESUStaffSystem::ESUStaffSystem() :
    QObject(nullptr)
  , d(new ESUStaffSystemPrivate(this))
{
    d->init();
}

ESUStaffSystem::~ESUStaffSystem()
{
    delete d;
}

// }}} [ INIT/DESTROY ]
// [ QML Methods ]: {{{

void ESUStaffSystem::declareQmlTypes()
{
    static bool declared = false;
    if( declared ) return;

#ifdef ESU_UI_QUICK
    qmlRegisterUncreatableType<ESUStaffSystem>(
                "MKBKompas.CSR.ESUTZ.Staff", 1, 0, "EsuStaff", "Can\'t create ESUStaff instance!");
#endif

    declared = true;
}

// }}} [ QML Methods ]

void ESUStaffSystem::loadData()
{
    d->data.load("./../../conf/staff_structure_msb.xml");
}


ESUStaffStructure &ESUStaffSystem::data() const
{
    return( d->data );
}


void ESUStaffSystem::printDivisions()
{
    StaffDivision division = d->data.rootDivision();
    qDebug() << " [0] => name = " << division.nameFull() << "; type = " << division.type();
    if( division.hasDivisions() ) {
        StaffDivisionList list = division.divisions();
        for( int i = 0; i < list.size(); ++i ) {
            printDivision( list[i], 1, true );
        }
    }
}

void ESUStaffSystem::printDivision(const StaffDivision &division, int level, bool recursive)
{
    qDebug() << "[" << level << "] " << "name = " << division.nameFull() << "; type = " << division.type();
    if( division.hasDivisions() ) {
        StaffDivisionList list = division.divisions();
        for( int i = 0; i < list.size(); ++i ) {
            printDivision( list[i], (level+1), true );
        }
    }
}

// [ IEventListener ]: {{{

// }}} [ IEventListener ]
// [ Q_EVENTS ]: {{{

void ESUStaffSystem::customEvent(QEvent *event)
{
    QObject::customEvent(event);
}

// }}} [ Q_EVENTS ]
