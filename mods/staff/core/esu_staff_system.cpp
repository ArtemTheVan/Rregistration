#include "esu_staff_system.h"
#include "esu_staff_system_p.h"

#include <QtQml>


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

    qmlRegisterUncreatableType<ESUStaffSystem>(
                "MKBKompas.CSR.ESUTZ.Staff", 1, 0, "EsuStaff", "Can\'t create ESUStaff instance!");

    declared = true;
}

// }}} [ QML Methods ]

void ESUStaffSystem::loadData()
{
    d->data.load(":/mods/staff/conf/staff_structure_msb.xml");
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
