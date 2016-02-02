#include "esu_staff_structure.h"
#include "esu_staff_structure_p.h"


// [ INIT/DESTROY ]: {{{

ESUStaffStructure::ESUStaffStructure(QObject *parent): QObject(parent)
  , d(new ESUStaffStructurePrivate(this))
{

}


ESUStaffStructure::~ESUStaffStructure()
{
    delete d;
}

// }}} [ INIT/DESTROY ]
// [ CONTENT STAFF ]: {{{

void ESUStaffStructure::create()
{
    if( !d->engine.rootDivision.isNull() ) return;

    d->engine.rootDivision.create();
}


bool ESUStaffStructure::load(const QString &fileName)
{
    do {
        QFile fileXml(fileName);
        if( !fileXml.exists() || !fileXml.open(QIODevice::ReadOnly) ) break;
        if( fileXml.size() <= 0L ) { fileXml.close(); break; }

        if( d->engine.currentDocument.hasChildNodes() ) d->engine.currentDocument.clear();

        QString errorMsg;
        if( !d->engine.currentDocument.setContent(&fileXml, &errorMsg) ){
            if( !errorMsg.isEmpty() ) { qDebug() << errorMsg; break; }
            break;
        }

        if( !d->engine.readDocument() ) break;

        return(true);

    } while(0);

    return(false);
}


void ESUStaffStructure::save(const QString &fileName)
{

}


void ESUStaffStructure::clear()
{

}

// }}} [ CONTENT STAFF ]
// [ SEARCH/FIND METHODS ]: {{{

StaffDivision ESUStaffStructure::rootDivision()
{
    return( d->engine.rootDivision );
}


StaffDivision ESUStaffStructure::findDivisionByName(const QString &name)
{
    return( StaffDivision() );
}


StaffBaseUnit ESUStaffStructure::findUnitByName(const QString &name)
{
    if( d->engine.nodes.contains(name) ) {
        StaffBaseUnit unit = StaffObject(d->engine.nodes[name]).toBaseUnit();
        return(unit);
    }

    return( StaffBaseUnit() );
}

// }}}
// [ CHECKS METHODS ]: {{{

bool ESUStaffStructure::isEmpty() const
{
        return( d->engine.rootDivision.isEmpty() );
}


bool ESUStaffStructure::contains(const QString &name)
{
    return( false );
}


bool ESUStaffStructure::containsDivision(const QString &name)
{
    return( false );
}


bool ESUStaffStructure::containsUnit(const QString &name)
{
    return( false );
}

// }}} [ CHECKS METHODS ]
