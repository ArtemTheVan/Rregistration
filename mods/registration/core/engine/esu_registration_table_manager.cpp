#include "esu_registration_table_manager.h"
#include "esu_registration_engine.h"
#include "esu_registration_net.h"

#include "localstoragemanager.h"

#include <QtXml>
#include <ctime>


// [ INIT/DESTROY ]: {{{

ESURegistrationTableManager::ESURegistrationTableManager(QObject *parent):
    QObject(parent)
  , m_ready(false)
  , m_filePath("/media/card/esutz/registration/registration_table.xml")
{
    QFileInfo fileInfo(m_filePath);
    QDir fileDir = fileInfo.absoluteDir();
    if( !fileDir.exists() ) {
        if( !fileDir.mkdir(fileDir.absolutePath()) )
            qDebug() << Q_FUNC_INFO << ": bad file path...";
    }

    m_tableSceme["role"]        = "VARCHAR(255) NOT NULL PRIMARY KEY"; // unique
    m_tableSceme["address"]     = "VARCHAR(255) NOT NULL UNIQUE";
    m_tableSceme["name"]        = "VARCHAR(255)";
    m_tableSceme["time"]        = "INTEGER";        // UNIX TIMESTAMP SINCE 1970
    m_tableSceme["speciality"]  = "VARCHAR(100)";
    m_tableSceme["rankIndex"]   = "INTEGER";
    m_tableSceme["homeAddress"] = "BLOB";
    m_tableSceme["birthDate"]   = "INTEGER";        // UNIX TIMESTAMP SINCE 1970

    m_lastSyncTime = time(0);

    m_dbTable = esuDb.getVariantTable("RegistrationTable", m_tableSceme);
    m_dbReady = (m_dbTable != nullptr);
}


ESURegistrationTableManager::~ESURegistrationTableManager()
{

}

// }}} [ INIT/DESTROY ]
// [ PRIVATE: HELPERS METHODS ]: {{{

QVariantMap ESURegistrationTableManager::__createDBRecord(const RegistrationUserInfo &r)
{
    QVariantMap record;

    record["role"]          = r.role;
    record["address"]       = r.addressString();
    record["name"]          = r.name;
    record["time"]          = r.time;
    record["speciality"]    = r.militarySpeciality;
    record["rankIndex"]     = r.rank;
    record["homeAddress"]   = r.streetAddress;
    record["birthDate"]     = r.birthDate;

    return( record );
}


void ESURegistrationTableManager::__fillDataFromDBRecord(RegistrationUserInfo &out, const QVariantMap &record)
{
    out.role                = record["role"].toString();
    out.address             = QHostAddress( record["address"].toString() );
    out.name                = record["name"].toString();
    out.time                = record["time"].toInt();
    out.militarySpeciality  = record["speciality"].toString();
    out.rank                = record["rankIndex"].toInt();
    out.streetAddress       = record["homeAddress"].toString();
    out.birthDate           = record["birthDate"].toInt();
}

// }}}
// [ REGISTRATION TABLE METHODS ]: {{{

void ESURegistrationTableManager::addRecord(const RegistrationUserInfo &userInfo)
{
    if( !m_tableData.contains(userInfo) ) {
        m_tableData.append(userInfo);

        if( m_dbReady ) {
            QVariantMap record = __createDBRecord(userInfo);
            m_dbTable->insert( record );
        }

        Q_EMIT contentUpdated();
    }
}


void ESURegistrationTableManager::updateRecord(const QHostAddress &address, const RegistrationUserInfo &data)
{
    for(int i=0; i<m_tableData.size(); i++ ) {
        if( m_tableData[i].address == address ) {
            if( m_dbReady ) {
                QVariantMap record = __createDBRecord(data);
                QString condition("address=\'%1\'");
                condition = condition.arg(address.toString());
                if( m_dbTable->updateRecord( record, condition ) )
                    Q_EMIT contentUpdated();
            }
            break;
        }
    }
}


void ESURegistrationTableManager::updateRecord(const QString &role, const RegistrationUserInfo &data)
{

    for( int i=0; i< m_tableData.size(); i++ ) {
        RegistrationUserInfo u = m_tableData[i];
        if( u.role == role ) {
            u = data;

            if( m_dbReady ) {
                QVariantMap record = __createDBRecord(u);
                QString condition("role=\'%1\'");
                condition = condition.arg(role);
                if( m_dbTable->updateRecord( record, condition ) )
                    Q_EMIT contentUpdated();
            }
            break;
        }
    }
}


bool ESURegistrationTableManager::containsRole(const QString &role)
{
    foreach( const RegistrationUserInfo& r, m_tableData ) {
        if( r.role == role ) return( true );
    }

    return( false );
}

bool ESURegistrationTableManager::containsAddress(const QHostAddress &address)
{
    foreach( const RegistrationUserInfo& r, m_tableData ) {
        if( r.address == address ) return( true );
    }

    return( false );
}

bool ESURegistrationTableManager::containsAddress(const QString &address)
{
    QHostAddress a(address);
    if( a.isNull() ) return( false );

    foreach( const RegistrationUserInfo& r, m_tableData ) {
        if( r.address == a ) return( true );
    }

    return( false );
}

bool ESURegistrationTableManager::containsName(const QString &name)
{
    foreach( const RegistrationUserInfo& r, m_tableData ) {
        if( r.name == name ) return( true );
    }

    return( false );
}

// [ Data/Content properties ]:

RegistrationUserInfo ESURegistrationTableManager::getRecordByRole(const QString &role) const
{
    foreach( const RegistrationUserInfo& r, m_tableData ) {
        if( r.role == role ) return( r );
    }

    return( RegistrationUserInfo() );
}


RegistrationUserInfo ESURegistrationTableManager::getRecordByAddress(const QHostAddress &address) const
{
    foreach( const RegistrationUserInfo& r, m_tableData ) {
        if( r.address == address ) return( r );
    }

    return( RegistrationUserInfo() );
}


RegistrationUserInfo ESURegistrationTableManager::getRecordByAddress(const QString &address) const
{
    QHostAddress addr(address);

    foreach( const RegistrationUserInfo& r, m_tableData ) {
        if( r.address == addr ) return( r );
    }

    return( RegistrationUserInfo() );
}


RegistrationUserInfo ESURegistrationTableManager::getRecordByName(const QString &name) const
{
    foreach( const RegistrationUserInfo& r, m_tableData ) {
        if( r.name == name ) return( r );
    }

    return( RegistrationUserInfo() );
}

// [ REMOVE RECORDS/CONTENT METHODS ]:

bool ESURegistrationTableManager::removeRecordByAddress(const QHostAddress &address)
{
    for( RegistrationTableData::iterator it = m_tableData.begin(); it != m_tableData.end(); ++it ) {
        if( it->address == address ) {
            QString condition("role=%1");
            condition = condition.arg(it->role);
            m_dbTable->remove(condition);

            it = m_tableData.erase(it);

            Q_EMIT contentUpdated();
            return( true );
        }
    }

    return( false );
}


bool ESURegistrationTableManager::removeRecordByAddress(const QString &address)
{
    return removeRecordByAddress(QHostAddress(address));
}


bool ESURegistrationTableManager::removeRecordByRole(const QString &role)
{
    for( RegistrationTableData::iterator it = m_tableData.begin(); it != m_tableData.end(); ++it ) {
        if( it->role == role ) {
            QString condition("role=%1");
            condition = condition.arg(it->role);
            m_dbTable->remove(condition);

            it = m_tableData.erase(it);
            Q_EMIT contentUpdated();

            return( true );
        }
    }

    return( false );
}

// [ SYNC FUNCTIONS ]:

void ESURegistrationTableManager::syncData()
{
    if( m_tableData.isEmpty() ) return;

    RegistrationTableData::iterator it;
    foreach( const RegistrationUserInfo& r, m_tableData ) {
        if( it->time >= m_lastSyncTime ) {
            RegistrationPackageNET p(ESURegistrationEngine::SyncRegistrationTableMsg);
            RegistrationPackageDataNET record;
            record = r;
            p.addRecord(record);
#ifdef ESU_NET_PROTO_SERVER
            esuNet.sendRegistrationMsg(p);
#endif
        }
    }

    m_lastSyncTime = std::time(0);
}


void ESURegistrationTableManager::syncRecord(const QHostAddress &address)
{
    if( m_tableData.isEmpty() ) return;

    RegistrationPackageNET p(ESURegistrationEngine::SyncRegistrationTableMsg);
    p.records.enable = true;

    foreach( const RegistrationUserInfo& r, m_tableData ) {
        if( r.address == address ) {
            RegistrationPackageDataNET record(r);
            p.addRecord(record);
#ifdef ESU_NET_PROTO_SERVER
            esuNet.sendRegistrationMsg(p);
#endif
            break;
        }
    }
}


void ESURegistrationTableManager::syncAllData()
{
    if( m_tableData.isEmpty() ) return;

    foreach( const RegistrationUserInfo& r, m_tableData )
    {
        RegistrationPackageNET p(ESURegistrationEngine::SyncRegistrationTableMsg);
        RegistrationPackageDataNET record(r);
        p.addRecord(record);
#ifdef ESU_NET_PROTO_SERVER
        esuNet.sendRegistrationMsg(p);
#endif
    }

    m_lastSyncTime = std::time(0);
}


void ESURegistrationTableManager::updateData(const RegistrationTableData &updateData)
{
    if( updateData.isEmpty() ) return;

    int index = 0;


    foreach( const RegistrationUserInfo& r, updateData )
    {
        QVariantMap record = __createDBRecord(r);

        if( m_tableData.contains(r) ) {
            index = m_tableData.indexOf(r);
            if( m_tableData[index].time < r.time ) {
                m_tableData[index] = r;
                if( m_dbReady ) {
                    QString condition("role=%1");
                    condition = condition.arg(r.role);
                    m_dbTable->updateRecord(record, condition);
                    Q_EMIT contentUpdated();
                }
            }
        } else {
            m_tableData.append(r);
            if( m_dbReady ) {
                m_dbTable->insert(record);
                Q_EMIT contentUpdated();
            }
        }
    }

    m_lastSyncTime = time(0);
    Q_EMIT contentUpdated();
}


void ESURegistrationTableManager::updateData(const RegistrationPackageNET &p)
{
    if( !p.records.enable || p.records.recordsList.isEmpty() )
        return;

    int index = 0, size = m_tableData.size();
    bool contains = false;


    foreach( const RegistrationPackageDataNET& r, p.records.recordsList )
    {
        RegistrationUserInfo rec = r;

        for( index = 0; index < size; ++index ) {
            if( m_tableData[index].address == r.netAddress &&
                    m_tableData[index].time < r.time) {
                contains = true;
                m_tableData[index] = rec;
            }
        }
        if( contains ) continue;

        QVariantMap record = __createDBRecord(rec);
        m_tableData.append(rec);
        if( m_dbReady ) {
            m_dbTable->insert(record);
        }
        contains = false;
    }

    Q_EMIT contentUpdated();
}

// [ Load functions ]:

bool ESURegistrationTableManager::loadFromFile(const QString &filePath)
{
    if( !filePath.isEmpty() && QFile::exists(filePath) )
        m_filePath = filePath;

    QFile file(m_filePath);

    do {
        if( !file.exists() || !file.open(QIODevice::ReadOnly) ) { m_ready = false; break; }
        if( file.size() <= 20 ) { file.close(); m_ready = false; break; }

        m_tableData = convertFromXML(file.readAll());
        if( m_tableData.isEmpty() ) { m_ready = false; break; }
        m_ready = true;
        saveToLocalDB();

        return(true);

    } while(0);

    return(false);
}


bool ESURegistrationTableManager::loadFromJSON(const QByteArray &dataJSON)
{
    RegistrationTableData table;

    do {
        if( dataJSON.isEmpty() || dataJSON.isNull() ) break;

        table = convertFromJSON(dataJSON);
        if( table.isEmpty() ) { m_ready = false; break; }

        if( !m_tableData.isEmpty() ) m_tableData.clear();
        m_tableData = table;
        m_ready = true;
        saveToLocalDB();

    } while(0);

    return( m_ready );
}


bool ESURegistrationTableManager::loadFromXML(const QByteArray &dataXML)
{
    RegistrationTableData table;

    do {
        if( dataXML.isEmpty() || dataXML.isNull() ) break;

        table = convertFromXML(dataXML);
        if( table.isEmpty() ) { m_ready = false; break; }

        if( !m_tableData.isEmpty() ) m_tableData.clear();
        m_tableData = table;
        m_ready = true;
        saveToLocalDB();

    } while(0);

    return( m_ready );
}


bool ESURegistrationTableManager::loadFromLocalDB()
{
    VariantTable* tableDb = esuDb.getVariantTable("RegistrationTable", m_tableSceme);
    if( tableDb == nullptr ) return(false);

    QList<QVariantMap> records = tableDb->select("");
    if( records.isEmpty() ) return(false);

    m_tableData.clear();
    foreach( const QVariantMap& record, records ) {
        RegistrationUserInfo data;
        __fillDataFromDBRecord(data, record);
        m_tableData.append(data);
    }
    Q_EMIT contentUpdated();

    return(true);
}

// [ Save functions ]:

bool ESURegistrationTableManager::saveToFile(const QString &filePath)
{
    if( !filePath.isEmpty() && QFile::exists(filePath) )
        m_filePath = filePath;

    QFile file(m_filePath);
    QByteArray dataXML;

    if( m_tableData.isEmpty() ) return(false);

    dataXML = convertToXML(Indented);
    if( dataXML.isEmpty() ) return(false);

    if( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) ) return(false);
    file.write(dataXML);
    file.close();

    return(true);
}


bool ESURegistrationTableManager::saveToLocalDB()
{
    VariantTable* tableDb = esuDb.getVariantTable("RegistrationTable", m_tableSceme);
    if( tableDb == nullptr ) return(false);

    foreach( const RegistrationUserInfo& r, m_tableData ) {
        QVariantMap record = __createDBRecord(r);
        QString cond = QString("role=%1").arg(r.role);
        if( !tableDb->containsRecord(cond) )
            tableDb->insert(record);
    }

    return(true);
}

// [ Clear functions ]:

void ESURegistrationTableManager::clearData()
{
    if( m_tableData.isEmpty() )
        return;

    m_tableData.clear();
    esuDb.removeTable("RegistrationTable");
    Q_EMIT contentUpdated();
}

// [ FORMATS FUNCTIONS ]:

QByteArray ESURegistrationTableManager::convertToXML(FormatType format)
{
    do {
        if( m_tableData.isEmpty() ) break;

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
        rootElement.setAttribute("created", currentDateTime.toString());

        foreach( const RegistrationUserInfo& r, m_tableData )
        {
            QDomElement recordElement = domDoc.createElement("record");
            rootElement.appendChild(recordElement);

            QDomElement e = domDoc.createElement("role");
            recordElement.appendChild(e);
            QDomText t = domDoc.createTextNode(r.role);
            e.appendChild(t);

            e = domDoc.createElement("address");
            recordElement.appendChild(e);
            t = domDoc.createTextNode(r.address.toString());
            e.appendChild(t);

            e = domDoc.createElement("name");
            recordElement.appendChild(e);
            t = domDoc.createTextNode(r.name);
            e.appendChild(t);
        }

        return( domDoc.toByteArray( (format == Compact)?(1):(4)) );

    } while(0);

    return QByteArray();
}


QByteArray ESURegistrationTableManager::convertToJSON(FormatType format)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    do {
        if( m_tableData.isEmpty() ) break;

        QJsonObject jsonRootObj;
        QJsonArray jsonRecordArray;

        for( RegistrationUserInfo& r: m_tableData ) {
            QJsonObject jsonRecord;
            jsonRecord["role"] = r.role;
            jsonRecord["address"] = r.address.toString();
            jsonRecord["name"] = r.name;
            jsonRecordArray.append(jsonRecord);
        }

        jsonRootObj["data"] = jsonRecordArray;

        return (QJsonDocument(jsonRootObj).toJson( (format == Compact)?(QJsonDocument::Compact):(QJsonDocument::Indented) ));

    } while(0);
#endif

    return QByteArray();
}


RegistrationTableData ESURegistrationTableManager::convertFromXML(const QByteArray &dataXML)
{
    QDomDocument domDoc("RegistrationTable");
    QDomElement rootElement;
    RegistrationTableData table;
    QString role, addr, name;

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
                    if( e.tagName() == "role" ) {
                        role = e.text();
                    } else if( e.tagName() == "address" ) {
                        addr = e.text();
                    } else if( e.tagName() == "name" ) {
                        name = e.text();
                    }
                }
                attrNode = attrNode.nextSibling();
            }
            if( !name.isEmpty() && !addr.isEmpty() )
                table.append(RegistrationUserInfo(role, addr, name));

            recordNode = recordNode.nextSibling();
        }

        return( table );

    } while(0);

    return( table );
}


RegistrationTableData ESURegistrationTableManager::convertFromJSON(const QByteArray &dataJSON)
{
    RegistrationTableData table;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    do {
        if( dataJSON.isEmpty() || dataJSON.isNull() ) break;

        QJsonDocument jsonDoc;
        QJsonObject jsonRootObj;
        QJsonArray jsonRecordArray;
        QString role, addr, name;
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

            role = jsonRecord["role"].toString();
            addr = jsonRecord["address"].toString();
            name = jsonRecord["name"].toString();
            if( addr.isEmpty() || role.isEmpty() ) continue;
            table.append(RegistrationUserInfo(role, addr, name));
        }

        return( table );

    } while(0);
#endif

    return( table );
}

// }}} [ REGISTRATION TABLE METHODS ]
// [ Q_SLOTS ]:

void ESURegistrationTableManager::onUpdateDB()
{

}

