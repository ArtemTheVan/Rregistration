#include "esu_registration_engine_p.h"



ESURegistrationEnginePrivate::ESURegistrationEnginePrivate(ESURegistrationEngine *parent):
    q_ptr(parent)
  , base(nullptr)
  , regTablePath("/media/card/esutz/registration/registration_conf.xml")
  , registeredName("Не зарегестрирован")
  , registered(false), registrar(false), regTableReady(false), registrarAddress("")
  , state(ESURegistration::UnknownState)
  , registrarState(ESURegistrationEngine::RegistrarSteady)
  , mode(ESURegistration::InitMode)
  , error(ESURegistration::NoErrors)
  , locked(false)
{
    //
}


ESURegistrationEnginePrivate::~ESURegistrationEnginePrivate()
{

}


void ESURegistrationEnginePrivate::init()
{
    // Init registrar
    tasksQueue.enqueue(RegistrarTaskSpec(ESURegistrationEngine::InitTask));
    semTasks.release();
}

// [ HELPERS METHODS ]: {{{

void ESURegistrationEnginePrivate::sendMsg(quint32 id, ESURegistrationEngine::Message message, const QString &addr)
{
    Q_Q(ESURegistrationEngine);
    char msg_buf[1] = { 0 };

    msg_buf[0] = (qint8)message;
    esuNet->sendData(msg_buf, 1, id, q->ID(), addr);
}


void ESURegistrationEnginePrivate::sendMsg(quint32 id, ESURegistrationEngine::Message message, const QByteArray &data, const QString &addr)
{
    Q_Q(ESURegistrationEngine);
    QByteArray msgbuf;

    msgbuf.append((char)message);
    msgbuf.append(data);

    esuNet->sendData(msgbuf, id, q->ID(), addr);
}


void ESURegistrationEnginePrivate::sendMsgWide(quint32 id, ESURegistrationEngine::Message message, const QStringList &addrs)
{
    Q_Q(ESURegistrationEngine);
    char msg_buf[1] = { 0 };

    msg_buf[0] = (qint8)message;
    esuNet->sendWide(msg_buf, 1, id, addrs, q->ID());
}


void ESURegistrationEnginePrivate::sendError(quint32 id, ESURegistration::Error err, const QString &addr)
{
    Q_Q(ESURegistrationEngine);
    char msg_buf[2];

    memset( msg_buf, 0x00, sizeof(msg_buf) );
    msg_buf[0] = (qint8)ESURegistrationEngine::RegistrationErrorMsg;
    msg_buf[1] = (qint8)err;
    esuNet->sendData(msg_buf, sizeof(msg_buf), id, q->ID(), addr);
}


void ESURegistrationEnginePrivate::registrarNotify()
{
    Q_Q(ESURegistrationEngine);    
    char msg_buf[1] = { 0 };

    msg_buf[0] = (qint8)ESURegistrationEngine::RegistrarAddrNotifyMsg;
    esuNet->sendData(msg_buf, 1, q->ID(), q->ID());
}


bool ESURegistrationEnginePrivate::checkRegistrarAvailability()
{
    Q_Q(ESURegistrationEngine);

    char msg_buf[1] = { 0 };
    QString addr;

    mutParams.lock();
    addr = registrarAddress;
    mutParams.unlock();
    if( addr.isEmpty() ) return(false);

    q->setState(ESURegistration::CheckRegistrar);
    q->sleep(2);
    msg_buf[0] = (qint8)ESURegistrationEngine::RegistrarEchoRequestMsg;
    esuNet->sendData(msg_buf, 1, q->ID(), q->ID(), addr);
    q->waitOperation(WAIT_OPERATION_REPLY);

    if( state == ESURegistration::CheckRegistrarSuccess )
        return(true);
    else
        q->setState(ESURegistration::CheckRegistrarFailure);

    return(false);
}


QString ESURegistrationEnginePrivate::requestRegistrarAddress()
{
    Q_Q(ESURegistrationEngine);

    char msg_buf[1] = { 0 };
    QString addr;

    msg_buf[0] = (qint8)ESURegistrationEngine::RegistrarAddrRequestMsg;
    esuNet->sendData(msg_buf, 1, q->ID(), q->ID());

#ifdef ESU_REGISTRATION_TEST
    //esuNet->sendData(msg_buf, 1, q->ID(), q->ID(), "fe80::12bf:48ff:fe1d:a61d");
#else
    //
#endif
    q->setState(ESURegistration::RequestRegistrar);
    q->sleep(1);

//    mutParams.lock();
//    addr = registrarAddress;
//    if( !addr.isEmpty() ) {
//        mutParams.unlock();
//        return( addr );
//    }
//    mutParams.unlock();

    q->waitOperation(WAIT_OPERATION_REPLY);
    mutParams.lock();
    addr = registrarAddress;
    mutParams.unlock();

    return( addr );
}

// }}} [ HELPERS METHODS ]
// [ PROPERTIES ]: {{{

void ESURegistrationEnginePrivate::setRegistrarAddress(const QString &addr)
{
    if( addr.isEmpty() ) return;

    mutParams.lock();
    if( !addr.isEmpty() ) registrarAddress = addr;
    mutParams.unlock();
}

// }}} [ PROPERTIES ]
// [ REGISTRATION TABLE METHODS ]: {{{

bool ESURegistrationEnginePrivate::checkRegTableFile(bool *hasContent)
{
    QFile file(regTablePath);

    do {
        if( file.exists() ) {
            if( !file.open(QIODevice::ReadOnly) ) break;

            if( hasContent != nullptr ) {
                if( file.size() > 5 ) *hasContent = true;
                else *hasContent = false;
            }

            file.close();
            return(true);
        }

        QFileInfo fileInfo(regTablePath);
        QDir fileDir = fileInfo.absoluteDir();
        if( !fileDir.exists() )
            if( !fileDir.mkdir(fileDir.absolutePath()) ) break;
        if( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) ) break;
        file.close();

    } while(0);

    if( file.isOpen() ) file.close();
    return(false);
}


bool ESURegistrationEnginePrivate::updateRegTable(const QString &addr, const QString &name)
{
    QMutexLocker locker(&mutRegTable);

    if( regTable.contains(addr) ) {
        if(  regTable[addr] == name ) return(false);
        regTable[addr] = name;
    } else {
        regTable.insert(addr, RegisterRecordSpec(addr, name));
    }

    return(true);
}


void ESURegistrationEnginePrivate::updateRegTable(const RegistrationTableData &srcTable, RegistrationTableData &dstTable)
{
    if( srcTable.isEmpty() ) return;

    for( auto it = srcTable.begin(); it != srcTable.end(); ++it ) {
        if( !dstTable.contains(it.key()) ) {
            dstTable.insert(it.key(), it.value());
        }
    }
}


bool ESURegistrationEnginePrivate::loadRegTable()
{
    QMutexLocker locker(&mutRegTable);

    QFile file(regTablePath);
    RegistrationTableData table;

    // Open file
    if( !file.exists() || !file.open(QIODevice::ReadOnly) ) { regTableReady = false; return(false); }
    if( file.size() <= 20 ) { file.close(); regTableReady = false; return(false); }

    table = convertRegTableFromXML(file.readAll());
    if( table.isEmpty() ) { regTableReady = false; return( false ); }

    mutRegTable.lock();
    if( !regTable.isEmpty() ) regTable.clear();
    regTable = table;
    regTableReady = true;
    mutRegTable.unlock();

    return(true);
}


bool ESURegistrationEnginePrivate::loadRegTableFromJson(const QByteArray &dataJSON)
{
    RegistrationTableData table;

    do {
        if( dataJSON.isEmpty() || dataJSON.isNull() ) break;

        table = convertRegTableFromJSON(dataJSON);
        if( table.isEmpty() ) { regTableReady = false; break; }

        mutRegTable.lock();
        if( !regTable.isEmpty() ) regTable.clear();
        regTable = table;
        regTableReady = true;
        mutRegTable.unlock();

        return( true );

    } while(0);

    return( false );
}


bool ESURegistrationEnginePrivate::loadRegTableFromXML(const QByteArray &dataXML)
{
    RegistrationTableData table;

    do {
        if( dataXML.isEmpty() || dataXML.isNull() ) break;

        table = convertRegTableFromXML(dataXML);
        if( table.isEmpty() ) { regTableReady = false; break; }

        mutRegTable.lock();
        if( !regTable.isEmpty() ) regTable.clear();
        regTable = table;
        regTableReady = true;
        mutRegTable.unlock();

        return( true );

    } while(0);

    return( false );
}


bool ESURegistrationEnginePrivate::syncRegTable(const RegistrationTableData &updateTable, const QString& addr)
{
    do {
        if( updateTable.isEmpty() ) break;

        QByteArray syncData = qCompress(convertRegTableToJSON(updateTable), 9);
        if( syncData.isEmpty() ) break;

        sendMsg(ESU_REGISTRATION_ID, ESURegistrationEngine::RegTableSyncMsg, syncData, addr);

        return( true );

    } while(0);

    return( false );
}


bool ESURegistrationEnginePrivate::syncFullRegTable(const RegistrationTableData &srcTable, const QString &addr)
{
    do {
        if( srcTable.isEmpty() ) break;

        QByteArray syncData = qCompress(convertRegTableToJSON(srcTable), 9);
        if( syncData.isEmpty() ) break;

        sendMsg(ESU_REGISTRATION_ID, ESURegistrationEngine::RegTableSyncFullMsg, syncData, addr);

        return( true );

    } while(0);

    return( false );
}


bool ESURegistrationEnginePrivate::saveRegTable(const RegistrationTableData &srcTable)
{
    QFile file(regTablePath);
    QByteArray dataXML;

    if( regTable.isEmpty() ) return(false);

    dataXML = convertRegTableToXML(regTable);
    if( dataXML.isEmpty() ) return(false);

    if( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) ) return(false);
    file.write(dataXML);
    file.close();

    return(true);
}


QByteArray ESURegistrationEnginePrivate::convertRegTableToXML(const RegistrationTableData &table)
{
    do {
        if( table.isEmpty() ) break;

        QDomDocument domDoc("RegistrationTable");
        QDomElement rootElement;
        QDateTime currentDateTime = QDateTime::currentDateTime();

        // Заголовок XML-документа
        QDomProcessingInstruction xmlNode =
                domDoc.createProcessingInstruction("xml", "version=\'1.0\' encoding=\'UTF-8\'");

        // Создаем корневой элемент XML-документа
        rootElement = domDoc.createElement("RegistrationTable");
        domDoc.appendChild(rootElement);
        domDoc.insertBefore(rootElement, xmlNode);
        rootElement.setAttribute("time", currentDateTime.toString());

        RegistrationTableData::const_iterator i = table.cbegin();

        while( i != table.cend() ) {
            QDomElement recordElement = domDoc.createElement("record");
            rootElement.appendChild(recordElement);

            QDomElement e = domDoc.createElement("name");
            recordElement.appendChild(e);
            QDomText t = domDoc.createTextNode(i.value().name);
            e.appendChild(t);

            e = domDoc.createElement("address");
            recordElement.appendChild(e);
            t = domDoc.createTextNode(i.key());
            e.appendChild(t);

            ++i;
        }

        return( domDoc.toByteArray() );

    } while(0);

    return QByteArray();
}


QByteArray ESURegistrationEnginePrivate::convertRegTableToJSON(const RegistrationTableData &table)
{
    do {
        if( table.isEmpty() ) break;

        QJsonObject jsonRootObj;
        QJsonArray jsonRecordArray;

        RegistrationTableData::const_iterator i = table.cbegin();

        while( i != table.cend() ) {
            QJsonObject jsonRecord;
            jsonRecord["name"] = i.value().name;
            jsonRecord["address"] = i.key();
            jsonRecordArray.append(jsonRecord);
            ++i;
        }

        jsonRootObj["data"] = jsonRecordArray;

        return (QJsonDocument(jsonRootObj).toJson(QJsonDocument::Compact));

    } while(0);

    return QByteArray();
}


RegistrationTableData ESURegistrationEnginePrivate::convertRegTableFromXML(const QByteArray &dataXML)
{
    QDomDocument domDoc("RegistrationTable");
    QDomElement rootElement;
    RegistrationTableData table;
    QString name;
    QString addr;

    do {
        if( !domDoc.setContent(dataXML) ) break;

        rootElement = domDoc.firstChildElement("RegistrationTable");
        if( rootElement.isNull() || !rootElement.hasChildNodes() ) break;

        QDomNode recordNode = rootElement.firstChild();
        while( !recordNode.isNull() )
        {
            QDomElement recordElement = recordNode.toElement();
            if( recordElement.isNull() ) {
                recordNode = recordNode.nextSibling();
                continue;
            }
            QDomNode attrNode = recordElement.firstChild();
            while( !attrNode.isNull() )
            {
                QDomElement e = attrNode.toElement();
                if( !e.isNull() ) {
                    if( e.tagName() == "name" ) {
                        name = e.text();
                    } else if( e.tagName() == "address" ) {
                        addr = e.text();
                    }
                }
                attrNode = attrNode.nextSibling();
            }
            if( !name.isEmpty() && !addr.isEmpty() ) table.insert(addr, name);
            recordNode = recordNode.nextSibling();
        }

        return( table );

    } while(0);

    return( table );
}


RegistrationTableData ESURegistrationEnginePrivate::convertRegTableFromJSON(const QByteArray &dataJSON)
{
    RegistrationTableData table;

    do {
        if( dataJSON.isEmpty() || dataJSON.isNull() ) break;

        QJsonDocument jsonDoc;
        QJsonObject jsonRootObj;
        QJsonArray jsonRecordArray;
        QString addr, name;
        int index, size;

        QJsonParseError jsonErr;
        jsonDoc = QJsonDocument::fromJson(dataJSON, &jsonErr);
        if( jsonErr.error != QJsonParseError::NoError ) break;

        jsonRootObj = jsonDoc.object();
        if( jsonRootObj.isEmpty() ) break;

        jsonRecordArray = jsonRootObj["data"].toArray();
        if( jsonRecordArray.isEmpty() ) break;

        size = jsonRecordArray.size();

        for( index = 0; index < size; ++index ) {
            QJsonObject jsonRecord = jsonRecordArray[index].toObject();
            if( jsonRecord.isEmpty() ) continue;

            addr = jsonRecord["address"].toString();
            name = jsonRecord["name"].toString();
            if( addr.isEmpty() || name.isEmpty() ) continue;
            table.insert(addr, name);
        }

        return( table );

    } while(0);

    return( table );
}

// }}} [ REGISTRATION TABLE METHODS ]

