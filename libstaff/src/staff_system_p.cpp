#include "esu_staff_system_p.h"
#include "staff_division_p.h"


// [ INIT/DESTROY ]: {{{

ESUStaffSystemPrivate::ESUStaffSystemPrivate(ESUStaffSystem *parent):
    q_ptr(parent)
  , ready(false)
  , structureFile("./../../conf/staff_structure.xml")
  //structureFile("mods/military_staff/conf/staff_structure.xml")
  , staffFile("./../../conf/staff.xml")
  , staffDocXML(QDomDocument("MilitaryStaffXML"))
  , structureDocXML(QDomDocument("MilitaryStaffStructureXML"))
{
    //
}


ESUStaffSystemPrivate::~ESUStaffSystemPrivate()
{
    structureData.release();
}


void ESUStaffSystemPrivate::init()
{

}


bool ESUStaffSystemPrivate::checkFiles()
{
    QDir dir;
    QString str = dir.currentPath();
    qDebug() << str;
    do {
        if( !QFile::exists(structureFile) ) break;
        if( !QFile::exists(staffFile) && !createStaffFile() ) break;

        return true;
    }
    while(0);

    return( false );
}


bool ESUStaffSystemPrivate::createStaffFile()
{
    do {
        if( staffFile.isEmpty() ) break;

        QFile file(staffFile);

        if( !file.exists(staffFile) )
        {
            QString temp = staffFile;
            int index = staffFile.lastIndexOf('/') + 1;
            if( index < 0 ) break;
            temp.remove(staffFile.mid(index));

            QDir filedir(temp);
            if( !filedir.exists() && !filedir.mkpath(temp) ) break;

            if( !file.open(QIODevice::Append) ) break;
        } else {
            if( !file.open(QIODevice::WriteOnly) ) break;
        }

        file.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        file.close();

        return true;
    }
    while(0);

    return( false );
}

// }}}
// [ LOAD AND CREATE DATA/STRUCTURE/CONFIGS ]: {{{

bool ESUStaffSystemPrivate::loadStructure()
{
    //QDomDocument domXml("staff_structure");
    QFile structureFileXml(structureFile);
    if( structureDocXML.hasChildNodes() ) structureDocXML.clear();

    if( !structureFileXml.open(QIODevice::ReadOnly) ) return(false);

    QString errorMsg;
    if( !structureDocXML.setContent(&structureFileXml, &errorMsg) ) return(false);
    if( !errorMsg.isEmpty() ) qDebug() << errorMsg;

    QDomElement rootElem = structureDocXML.documentElement();
    if( rootElem.isNull() ) return(false);
    QDomNode node = rootElem.firstChild();

    while( !node.isNull() )
    {
        QDomElement e = node.toElement();
        if( e.isNull() ) {
            node = node.nextSibling();
            continue;
        }
        // Считываем подразделение самого верхнего уровня (бригада, полк , дивизия и проч.)
        if( e.tagName() == "Division" )
        {
            // Предполагаем, что главное(корневое) подразделение у нас только одно
            // Считываем атрибуты (параметры) подразделения
            structureData.create();
            structureData.setNameBase( e.attribute("name") );
            structureData.setType( staff::getDivisionTypeByTypename(e.attribute("type")) );
            structureNodes.insert( structureData.nameFull(), structureData.d_func() );
            // Считываем подчиненных, командира и входящие подразделения
            readDivisionStructure(e, structureData);
            return(true);
        }
        node = node.nextSibling();
    }

    return(false);
}


bool ESUStaffSystemPrivate::loadStaff()
{
    return(true);
}

// [ INTERNAL STAFF CREATION METHODS ]:

bool ESUStaffSystemPrivate::createStaff(const StaffBaseUnit &unit, const QHostAddress &address)
{
    // Заголовок XML-документа
    QDomProcessingInstruction xmlNode = staffDocXML.createProcessingInstruction("xml",
                                                "version=\'1.0\' encoding=\'UTF-8\'");

    // Создаем корневой элемент XML-документа
    QDomElement rootElement = staffDocXML.createElement("MilitaryStaff");
    staffDocXML.appendChild(rootElement);
    staffDocXML.insertBefore(rootElement, xmlNode);
    rootElement.setAttribute("version", "1.0");
    rootElement.setAttribute("type", "staff");    

    if( unit.hasDivision() ) {
        StaffDivision div = unit.division();
        staffData.setName(div.nameBase(), div.number());
        staffData.setNameFullPart(div.nameFullPart());
    }

    if( unit.isCommander() )
    {
        StaffCommander commander = staffData.createCommander();
        commander.setNetAddress(address);
    }
    else if( unit.isSoldier() )
    {
        StaffSoldier soldier = staffData.createSoldier(unit.dutyName(), unit.number());
        soldier.setNetAddress(address);
    }

    convertToDom(staffData, rootElement, staffDocXML);

    QFile fileXML(staffFile);    
    if( !fileXML.open(QIODevice::WriteOnly) ) return(false);    
    QTextStream fileStreamXml(&fileXML);

    staffDocXML.save(fileStreamXml, 4);
    //qDebug() << QString(staffXML.toByteArray(4));
    fileXML.close();

    return(true);
}


bool ESUStaffSystemPrivate::createStaff(const QByteArray &data)
{
    staffDocXML.setContent(data);

    QDomElement rootElem = staffDocXML.documentElement();
    if( rootElem.isNull() ) return(false);
    QDomNode node = rootElem.firstChild();

    if( staffData.hasObjects() ) staffData.clear();

    while( !node.isNull() )
    {
        QDomElement e = node.toElement();
        if( e.isNull() ) {
            node = node.nextSibling();
            continue;
        }

        if( e.tagName() == "Division" )
        {
            if( staffData.isNull() ) staffData.create();
            staffData.setNameBase( e.attribute("name") );
            staffData.setType( staff::getDivisionTypeByTypename(e.attribute("type")) );
            staffNodes.insert( staffData.nameFull(), staffData.d_func() );
            if( e.hasAttribute("num") ) {
                staffData.setNumber(e.attribute("num").toInt());
            }
            readDivisionStaff(e, staffData);

            QFile fileXML(staffFile);
            if( !fileXML.open(QIODevice::WriteOnly) ) return(false);
            QTextStream fileStreamXml(&fileXML);
            staffDocXML.save(fileStreamXml, 4);
            fileXML.close();

            return(true);
        }
        node = node.nextSibling();
    }

    return(false);
}


bool ESUStaffSystemPrivate::updateStaff(const QString &name, const QString &address, QByteArray *outData)
{
    QDomElement rootElement;
    QDomElement e;
    QDomNode    node;
    StaffDivision rootDivision;

    //if( staffData.isNull() ) staffData.create();

    if( !structureNodes.contains(name) ) return(false);
    StaffObject staffObj((StaffObjectPrivate*) structureNodes[name] );
    if( staffData.contains(staffObj) ) return(false);
    StaffObject rootObj = staffObj.copy();

    StaffBaseUnit unit = *((StaffBaseUnit *)&staffObj);
    rootDivision = unit.division().copy();
    if( rootDivision.isNull() ) return( false );

    if( !staffDocXML.hasChildNodes() || staffDocXML.isNull() )
    {
        QDomProcessingInstruction xmlNode = staffDocXML.createProcessingInstruction("xml",
                                                    "version=\'1.0\' encoding=\'UTF-8\'");
        e = staffDocXML.createElement("MilitaryStaff");
        staffDocXML.appendChild(e);
        staffDocXML.insertBefore(e, xmlNode);
        e.setAttribute("version", "1.0");
        staffData = rootDivision;

        rootElement = staffDocXML.createElement("Division");
        rootElement.setAttribute( "name", rootDivision.nameBase());
        rootElement.setAttribute( "type", staff::getDivisionTypename(rootDivision.type()) );
        if( rootDivision.number() > 1 )
            rootElement.setAttribute("num", rootDivision.number());
        e.appendChild(rootElement);

        if( staffObj.isSoldier() )
        {
            StaffSoldier soldier = ((StaffSoldier *)&staffObj)->copy();
            soldier.setNetAddress(QHostAddress(address));
            e = staffDocXML.createElement("Soldier");
            e.setAttribute( "name", soldier.nameBase() );
            e.setAttribute( "duty", soldier.dutyName() );
            if( soldier.number() > 0 )
                e.setAttribute( "num", soldier.number() );
            rootElement.appendChild(e);

            QDomElement addressNode = staffDocXML.createElement("address");
            QDomText textNode = staffDocXML.createTextNode(address);
            addressNode.appendChild(textNode);
            e.appendChild(addressNode);

            rootDivision.addSoldier(soldier);
        }
        else if( staffObj.isCommander() )
        {
            StaffCommander commander = ((StaffCommander *)&staffObj)->copy();
            commander.setNetAddress(QHostAddress(address));
            e = staffDocXML.createElement("Commander");
            e.setAttribute( "rank", staff::getRankDescription(commander.rank()) );
            rootElement.appendChild(e);

            QDomElement addressNode = staffDocXML.createElement("address");
            QDomText textNode = staffDocXML.createTextNode(address);
            addressNode.appendChild(textNode);
            e.appendChild(addressNode);

            rootDivision.setCommander(commander);
        }
    }
    else
    {
        e = staffDocXML.firstChildElement("MilitaryStaff");
        rootElement = e;
        if( !e.hasChildNodes() || e.isNull() ) return( false );

        bool finded = false;

        node = e.firstChild();
        while( !node.isNull() ) {
            e = node.toElement();
            if( e.tagName() == "Division" )
            {
                do {
                    if( e.attribute("name") != rootDivision.nameBase() ) break;
                    if( e.hasAttribute("num") ) {
                        if( e.attribute("num").toInt() != rootDivision.number() ) break;
                    }
                    qDebug() << rootDivision.name() << " \t " << staffData.name();

                    if( rootDivision.name() == staffData.name() )
                        rootDivision = staffData;
                    else rootDivision = staffData.getDivision(rootDivision.name());
                    if( rootDivision.isNull() ) break;

                    rootDivision.addObject(rootObj);
                    rootElement = e;
                    convertToDom( rootObj, e, staffDocXML);
                    finded = true;

                } while(0);

                if( finded ) break;
            }
            node = e.nextSibling();
        }

        if( !finded ) {
            staffData.addDivision(rootDivision);
            rootDivision.addObject(rootObj);
            convertToDom( *((StaffObject *)&rootDivision), rootElement, staffDocXML);
        }
    }

    if( outData != nullptr ) {
        *outData = staffDocXML.toByteArray();
    }

    QFile fileXML(staffFile);
    if( !fileXML.open(QIODevice::WriteOnly) ) return(false);
    QTextStream fileStreamXml(&fileXML);
    staffDocXML.save(fileStreamXml, 4);
    fileXML.close();

    return(true);
}


bool ESUStaffSystemPrivate::updateStaff(const QByteArray &data)
{

    // TODO: ШТАТНЫЙ СОСТАВ - реализовать
     /*
    QDomDocument docUpdate;
    QDomElement rootElement;
    QDomElement e;
    QDomNode    node;

    if( !docUpdate.setContent(data) ) return( false );


    if( !staffDocXML.hasChildNodes() || staffDocXML.isNull() ) {
        QDomProcessingInstruction xmlNode = staffDocXML.createProcessingInstruction("xml",
                                                    "version=\'1.0\' encoding=\'UTF-8\'");
        rootElement = staffDocXML.createElement("MilitaryStaff");
        staffDocXML.appendChild(rootElement);
        staffDocXML.insertBefore(rootElement, xmlNode);
        rootElement.setAttribute("version", "1.0");
    }

    node = docUpdate.firstChild(); // "StaffUpdate"
    if( node.isNull() || !node.hasChildNodes() ) return( false );
    e = node.firstChild().toElement(); // Root Division
    if( e.isNull() ) return( false );

    if( !staffData.hasObjects() ) {
        readDivisionStaff(e, staffData);
        return( true );
    }
    */

    return( createStaff(data) );
}

// [ INTERNAL READ METHODS ]:

void ESUStaffSystemPrivate::readDivisionStructure(QDomElement &e, StaffDivision &div, int contNum)
{
    if( div.isNull() ) div.create();
    int index = 0, count = 1;
    QString tag;

    // Считываем подразделения, командира и подчиненных
    QDomNode node = e.firstChild();
    while(!node.isNull())
    {
        QDomElement elem = node.toElement();
        if( elem.isNull() ) {
            node = node.nextSibling();
            continue;
        }
        tag = elem.tagName();

        if( tag == "Division" )
        {
            count = index = 1;
            if( elem.hasAttribute("count") ) {                
                count = elem.attribute("count").toInt();
                if( count < 1 ) count = 1;
            }

            while( index <= count )
            {
                StaffDivision subdiv = div.createSubDivision(elem.attribute("name"));
                subdiv.setType( staff::getDivisionTypeByTypename( elem.attribute("type")) );
                if( count > 1 ) subdiv.setNumber(index);
//                qDebug() << subdiv.nameFull();
                readDivisionStructure(elem, subdiv);                
                structureNodes.insert( subdiv.nameFull(), subdiv.d_func() );
                ++index;
            }
        }
        else if( tag == "DivisionGroup" )
        {
            bool continuous = false;
            int num = 0;

            count = index = 1;
            if( elem.hasAttribute("count") ) {
                count = elem.attribute("count").toInt();
                if( count < 1 ) count = 1;
            }

            if( elem.hasAttribute("numeration") && elem.attribute("numeration") == "continuous" )
            {
                continuous = true;
            }

            while( index <= count )
            {
                StaffDivision subdiv = div.createSubDivision(elem.attribute("name"));
                subdiv.setType( staff::getDivisionTypeByTypename(elem.attribute("type")));
                num = index;
                if( continuous ) num = index + (contNum - 1)*count;
                subdiv.setNumber(num);
//                qDebug() << subdiv.nameFull();
                readDivisionStructure(elem, subdiv, index);                
                structureNodes.insert( subdiv.nameFull(), subdiv.d_func() );
                ++index;
            }
        }
        else if( elem.tagName() == "Commander" ) {
            StaffCommander commander; //= div.createCommander();
            commander.create();
            div.setCommander(commander);
            structureNodes.insert( commander.nameFull(), commander.d_func() );
        }
        else if( elem.tagName() == "Soldier" ) {
            count = index = 1;
            if( elem.hasAttribute("count") ) {
                count = elem.attribute("count").toInt();
                if( count <= 0 ) count = 1;                
            }
            while( index <= count )
            {
                StaffSoldier soldier = div.createSoldier(elem.attribute("name"),(count > 1 ? (index):(0)));
                structureNodes.insert( soldier.nameFull(), soldier.d_func() );
                ++index;
            }
        }
        node = node.nextSibling();
    }
}


void ESUStaffSystemPrivate::readDivisionStaff(QDomElement &e, StaffDivision &division)
{
    if( division.isNull() ) division.create();

    int num = 0;
    QString tagName;

    // Считываем подразделения, командира и подчиненных
    QDomNode node = e.firstChild();
    while(!node.isNull())
    {
        QDomElement elem = node.toElement();
        if( elem.isNull() ) {
            node = node.nextSibling();
            continue;
        }
        tagName = elem.tagName();

        if( tagName == "Division" )
        {
            StaffDivision subDivision = division.createSubDivision(elem.attribute("name"));
            subDivision.setType( staff::getDivisionTypeByTypename( elem.attribute("type")) );
            if( elem.hasAttribute("num") ) {
                num = elem.attribute("num").toInt();
                subDivision.setNumber(num);
            }
            readDivisionStaff(elem, subDivision);
            staffNodes.insert( subDivision.nameFull(), subDivision.d_func() );
        }
        else if( elem.tagName() == "Commander" )
        {
            StaffCommander commander = division.createCommander();
            if( elem.hasAttribute("rank") ) {
                commander.setRank( staff::getUnitRankByDescr(elem.attribute("rank")) );
            }
            if( elem.hasChildNodes() )
                readStaffUnitAttributes(elem, commander);

            staffNodes.insert( commander.nameFull(), commander.d_func() );
        }
        else if( elem.tagName() == "Soldier" )
        {
            StaffSoldier soldier = division.createSoldier(elem.attribute("name"));
            staffNodes.insert( soldier.nameFull(), soldier.d_func() );
            if( elem.hasChildNodes() )
                readStaffUnitAttributes(elem, soldier);
        }

        node = node.nextSibling();
    }
}


void ESUStaffSystemPrivate::readStaffUpdate(QDomElement &newElement, QDomElement &rootElement, StaffDivision &division)
{
    if( division.isNull() ) division.create();
    int num = 0;
    QString tagName;

    // Считываем подразделения, командира и подчиненных
    QDomNode node = newElement.firstChild();
    QDomElement findElement;
    bool exists = false;

    while(!node.isNull())
    {
        QDomElement elem = node.toElement();
        if( elem.isNull() ) {
            node = node.nextSibling();
            continue;
        }
        tagName = elem.tagName();

        if( tagName == "Division" )
        {

            //findElement = rootElement.fi

            StaffDivision subDivision = division.createSubDivision(elem.attribute("name"));
            subDivision.setType( staff::getDivisionTypeByTypename( elem.attribute("type")) );
            if( elem.hasAttribute("num") ) {
                num = elem.attribute("num").toInt();
                subDivision.setNumber(num);
            }
            readDivisionStaff(elem, subDivision);
            staffNodes.insert( subDivision.nameFull(), subDivision.d_func() );
        }
        else if( elem.tagName() == "Commander" )
        {
            StaffCommander commander = division.createCommander();
            if( elem.hasAttribute("rank") ) {
                commander.setRank( staff::getUnitRankByDescr(elem.attribute("rank")) );
            }
            if( elem.hasChildNodes() )
                readStaffUnitAttributes(elem, commander);

            staffNodes.insert( commander.nameFull(), commander.d_func() );
        }
        else if( elem.tagName() == "Soldier" )
        {
            StaffSoldier soldier = division.createSoldier(elem.attribute("name"));
            staffNodes.insert( soldier.nameFull(), soldier.d_func() );
            if( elem.hasChildNodes() )
                readStaffUnitAttributes(elem, soldier);
        }

        node = node.nextSibling();
    }
}


void ESUStaffSystemPrivate::readStaffUnitAttributes(QDomElement &e, StaffBaseUnit &unit)
{
    if( unit.isNull() ) unit.create();
    QDomNode node = e.firstChild();

    while(!node.isNull())
    {
        QDomElement elem = node.toElement();
        if( elem.isNull() ) continue;

        if( elem.tagName() == "address" ) {
            QHostAddress address(elem.attribute("address"));
            if( address.isNull() ) return;
            unit.setNetAddress(address);
        }
    }
}

// }}} [ LOAD AND CREATE DATA/STRUCTURE/CONFIGS ]
// [ DOCUMENT FORMAT CONVERTIONS ]: {{{

void ESUStaffSystemPrivate::convertToDom(const StaffObject &object, QDomElement &parent, QDomDocument &docXML, QDomElement *updateData)
{
    QDomElement elem;
    QString address;
    if( object.isNull() ) return;

    if( object.isDivision() )
    {
        StaffDivision division = *((StaffDivision *)&object);
        elem = docXML.createElement("Division");
        elem.setAttribute( "name", division.nameBase());
        elem.setAttribute( "type", staff::getDivisionTypename(division.type()) );
        if( division.number() > 1 )
            elem.setAttribute("num", division.number());

        parent.appendChild(elem);
        if( updateData ) updateData->appendChild(elem);

        if( division.hasCommander() )
            convertToDom(division.commander(), elem, docXML);

        if( division.hasSoldiers() ) {
            StaffSoldierList soldiers = division.soldiers();
            for( int i = 0; i < soldiers.count(); ++i ) {
                convertToDom(soldiers[i], elem, staffDocXML);
            }
        }

        if( division.hasDivisions() ) {
            StaffDivisionList divisions = division.divisions();
            for( int i = 0; i < divisions.count(); ++i ) {
                convertToDom(divisions[i], elem, staffDocXML);
            }
        }
    }
    else if( object.isSoldier() )
    {
        StaffSoldier soldier = *((StaffSoldier *)&object);
        elem = docXML.createElement("Soldier");
        elem.setAttribute( "name", soldier.nameBase() );
        elem.setAttribute( "duty", soldier.dutyName() );
        if( soldier.number() > 0 )
            elem.setAttribute( "num", soldier.number() );
        address = soldier.netAddress().toString();
        parent.appendChild(elem);
        if( updateData ) updateData->appendChild(elem);
    }
    else if( object.isCommander() )
    {
        StaffCommander commander = *((StaffCommander *)&object);
        elem = docXML.createElement("Commander");
        elem.setAttribute( "rank", staff::getRankDescription(commander.rank()) );
        address = commander.netAddress().toString();
        parent.appendChild(elem);
        if( updateData ) updateData->appendChild(elem);
    }

    if( object.isCommander() || object.isSoldier() ) {
        QDomElement addressNode = docXML.createElement("address");
        QDomText textNode = docXML.createTextNode(address);
        addressNode.appendChild(textNode);
        elem.appendChild(addressNode);
    }
}

// }}} [ DOCUMENT FORMAT CONVERTIONS ]
