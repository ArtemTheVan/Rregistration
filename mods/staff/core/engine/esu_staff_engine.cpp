#include "esu_staff_engine.h"



ESUStaffEngine::ESUStaffEngine(QObject *parent):
    QObject(parent)
{

}


ESUStaffEngine::~ESUStaffEngine()
{
    //delete d;
}

// [ XML Document Parsing ]: {{{

bool ESUStaffEngine::readDocument()
{
    DocumentType type = UnknownDocument;

    if( currentDocument.isNull() ) return( false );

    // Определяем тип документа (орг.штатная структура/штатный (фактический) состав)
    QDomDocumentType docTypeNode = currentDocument.doctype();
    if( docTypeNode.isNull() ) return(false);
    if( docTypeNode.name() == "StaffStructure" ) type = StructureDocument;
    else if( docTypeNode.name() == "Staff" ) type = StaffDocument;
    else return( false );

    QDomElement rootElement = currentDocument.documentElement(); // read <Staff/StaffStructure>
    if( rootElement.isNull() ) return(false);
    QDomNode node = rootElement.firstChild(); // read root division element
    if( node.isNull() ) return( false );

    // Считываем корневое подразделение
    QDomElement element = node.toElement();    
    while(element.isNull() || !element.tagName().contains("Division", Qt::CaseInsensitive)) {
        node = node.nextSibling();
        if( node.isNull() ) return( false );
        element = node.toElement();
    }
//    qDebug() << "tagName: " << element.tagName();
//    qDebug() << element.attribute("name") << " " << element.attribute("type");
    if( rootDivision.isNull() ) {
        rootDivision.create();
        rootDivision.d_ptr->engine = this;
    }

    rootDivision.setNameBase( element.attribute("name") );
    rootDivision.setType( staff::getDivisionTypeByTypename(element.attribute("type")) );
    nodes.insert( rootDivision.name(), rootDivision.d_ptr );

    if( type == StructureDocument ) {        
        readStructure(element, rootDivision);
    } else {
        readStaff(element, rootDivision);
    }

    return( !rootDivision.isEmpty() );
}


void ESUStaffEngine::readStructure(QDomElement &element, StaffDivision &division, int contNum)
{
    int index = 0, count = 1;
    QString tagName;

    // Добавляем командира
    StaffCommander commander;
    commander.create();
    division.setCommander(commander);
    nodes.insert( commander.nameFull(), commander.d_ptr );

    // Считываем подразделения, командира и подчиненных
    QDomNode node = element.firstChild();
    while(!node.isNull())
    {
        QDomElement e = node.toElement();
        if( e.isNull() ) {
            node = node.nextSibling();
            continue;
        }
        tagName = e.tagName();

        if( tagName == "Division" )
        {
            count = index = 1;
            if( e.hasAttribute("count") ) {
                count = e.attribute("count").toInt();
                if( count < 1 ) count = 1;
            }

            while( index <= count )
            {
                StaffDivision subDivision;                
                subDivision = division.createSubDivision( e.attribute("name") );
//                qDebug() << element.attribute("name") << " " << element.attribute("type");
                subDivision.setType( staff::getDivisionTypeByTypename( e.attribute("type")) );
                if( count > 1 ) subDivision.setNumber(index);

                readStructure(e, subDivision);
                nodes.insert( subDivision.nameFull(), subDivision.d_ptr );
                ++index;
            }
        }
        else if( tagName == "DivisionGroup" )
        {
            bool continuous = false;
            int num = 0;

            count = index = 1;
            if( e.hasAttribute("count") ) {
                count = e.attribute("count").toInt();
                if( count < 1 ) count = 1;
            }

            if( e.hasAttribute("numeration") && e.attribute("numeration") == "continuous" )
            {
                continuous = true;
            }

            while( index <= count )
            {
                StaffDivision subDivision = division.createSubDivision(e.attribute("name"));
                subDivision.setType( staff::getDivisionTypeByTypename(e.attribute("type")));
//                qDebug() << element.attribute("name") << " " << element.attribute("type");
                num = index;
                if( continuous ) num = index + (contNum - 1)*count;
                subDivision.setNumber(num);

                readStructure(e, subDivision, index);
                nodes.insert( subDivision.nameFull(), subDivision.d_ptr );
                ++index;
            }
        }
        /*
        else if( e.tagName() == "Commander" ) {
            StaffCommander commander; //= div.createCommander();
            commander.create();
            div.setCommander(commander);
            structureNodes.insert( commander.nameFull(), commander.d_func() );
        } */
        else if( e.tagName() == "Soldier" ) {
            count = index = 1;
            if( e.hasAttribute("count") ) {
                count = e.attribute("count").toInt();
                if( count <= 0 ) count = 1;
            }
            while( index <= count )
            {
                StaffSoldier soldier = division.createSoldier(e.attribute("name"),(count > 1 ? (index):(0)));
                bool flag = staff::isSecondInCommand(soldier.dutyName());
                if(flag){
                    staff::UnitDuty duty = staff::getSecondInCommandDuty(division.type());
                    soldier.setDuty(duty);
                }
                nodes.insert( soldier.nameFull(), soldier.d_ptr );
                ++index;
            }
        }

        node = node.nextSibling();
    }
}


void ESUStaffEngine::readStaff(QDomElement &element, StaffDivision &division)
{

}

// }}} [ XML Document Parsing ]

bool ESUStaffEngine::nodesAddressContains(const QHostAddress &address)
{
    return( nodesNetAddress.contains(address) );
}


bool ESUStaffEngine::nodesAddressContains(StaffObjectPrivate *node)
{
    return( nodesNetAddress.values().contains(node) );
}
