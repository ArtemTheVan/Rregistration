#include "registration_engine.h"
#include "registration_engine_p.h"
#include "registration_net.h"

#include <QDomDocument>

//#define ESU_REG_TEST


ESURegistrationEngine::ESURegistrationEngine(QObject *parent):
    QObject(parent)
  , d(new ESURegistrationEnginePrivate(this))
{
    d->init();
}


ESURegistrationEngine::ESURegistrationEngine(ESURegistration *base, QObject *parent):
    QObject(parent)
  , d(new ESURegistrationEnginePrivate(this))
{    
    d->base = base;
    d->init();
}


ESURegistrationEngine::~ESURegistrationEngine()
{
    delete d;
}

void ESURegistrationEngine::setBase(ESURegistration *base)
{
    d->base = base;
}

//void ESURegistrationEngine::setUI(ESURegistrationUI *ui)
//{
//    d->ui = ui;
//}

// [ PROPERTIES ]: {{{

ESURegistrationTableManager *ESURegistrationEngine::tableManager() const
{
    return( &d->tableManager );
}


RegistrationUserInfo &ESURegistrationEngine::registrationData()
{
    return( d->_u.registrationData );
}

void ESURegistrationEngine::setConfigurationFilePath(const QString &path)
{
    if( d->configurationFile != path ) {
        d->configurationFile = path;
        //закомментировал, из-за этого не находил конфиг
        //        QFileInfo info(d->configurationFile);
        //        d->configurationFile = info.fileName();
    }
}
// }}} [ PROPERTIES ]
// [ Opertation status ]: {{{

void ESURegistrationEngine::acceptOperation()
{
    d->base->setOperationState( ESURegistration::OperationSuccess );
}

void ESURegistrationEngine::rejectOperation()
{
    d->base->setOperationState( ESURegistration::OperationFailure );
}

// }}}
// [ CONFIGURATION ][ INVOKABLE ]: {{{

void ESURegistrationEngine::loadConfiguration(const QString &selfIp) //передать в аргументе свой ip
{
    if( !loadRegistrarList() )
        return;
    d->base->setRegistrarCount(d->registrarList.size());
    if( d->registrarList.count() == 1 )
        d->base->setCurrentRegistrarIndex(0);

    // [ Registration Table init ]:
    d->tableManager.loadFromLocalDB();
    Q_EMIT d->base->registrationTableUpdated();

    // [ Profile init ]:
#ifdef ESU_NET_SYSTEM
    d->profileAddress = esuNet.localAddress();
    if( d->profileAddress.isNull() )
        return;
#else
    //    d->profileAddress = "::1"; //это мой адрес?
    d->profileAddress = QHostAddress(selfIp);
#endif
    //todo убрать esuNet

    // Проверяем, зарегестрированы ли мы. Для этого смотрим запись в БД. Если есть - знначит - да.
    if( d->tableManager.containsAddress( d->profileAddress ) )
    {
        d->_u.registrationData = d->tableManager.getRecordByAddress(d->profileAddress);
        d->base->setRegistrationData(d->_u.registrationData);
        d->base->setRegistrationState(ESURegistration::RegisteredState);
        d->isRegistered = true;

#ifdef ESU_APPLICATION
        ESUSettings::set(ESUSettings::ProfileRegistered) = d->isRegistered;
        ESUSettings::set(ESUSettings::ProfileRole) = d->_u.registrationData.role;
        ESUSettings::set(ESUSettings::ProfileRegistrationTime) = d->_u.registrationData.timeString;

        if( d->_u.registrationData.hasUserName() )
            ESUSettings::set(ESUSettings::ProfileName) = d->_u.registrationData.name;

        if( d->_u.registrationData.hasUserSpeciality() )
            ESUSettings::set(ESUSettings::ProfileSpeciality) = d->_u.registrationData.militarySpeciality;

        if( d->_u.registrationData.hasUserBirthday() )
            ESUSettings::set(ESUSettings::ProfileBirthDate) = d->_u.registrationData.birthDateString();

        if( d->_u.registrationData.hasUserRank() )
            ESUSettings::set(ESUSettings::ProfileRank) = d->_u.registrationData.rank;

        if( d->_u.registrationData.hasStreetAddress() )
            ESUSettings::set(ESUSettings::ProfileStreetAddress) = d->_u.registrationData.streetAddress;
#endif

    } else {
        // Не зарегистрированы
        d->isRegistered = false;
        d->base->setRegistrationState(ESURegistration::UnregisteredState);
    }

    // Проверяем, являемся ли мы регистратором
    if( d->registrarMain.address == d->profileAddress ) {
        d->isRegistrar = true;
        d->base->setProfile( ESURegistration::RegistrarMainProfile );
        d->base->setCurrentRegistrarIndex( d->registrarList.indexOf(d->registrarMain) );

    } else {
        foreach( const RegistrarInfoSpec& r, d->registrarList )
        {
            if( r.address == d->profileAddress ) {
                d->isRegistrar = true;
                d->base->setProfile( ESURegistration::RegistrarProfile );
                d->registrarMain = r;
                d->base->setCurrentRegistrarIndex( d->registrarList.indexOf(d->registrarMain) );
                break;
            }
        }

        if( !d->isRegistrar )
            d->base->setProfile( ESURegistration::UserProfile );
    }
}


void ESURegistrationEngine::saveConfiguration()
{
    if( d->_u.registrationData.isEmpty() )
        return;

    // Сохранение настроек профиля
#ifdef ESU_APPLICATION
    ESUSettings::set(ESUSettings::ProfileRegistered) = d->isRegistered;
    ESUSettings::set(ESUSettings::ProfileRole) = d->_u.registrationData.role;
    ESUSettings::set(ESUSettings::ProfileRegistrationTime) = d->_u.registrationData.timeString;

    if( d->_u.registrationData.hasUserName() )
        ESUSettings::set(ESUSettings::ProfileName) = d->_u.registrationData.name;

    if( d->_u.registrationData.hasUserSpeciality() )
        ESUSettings::set(ESUSettings::ProfileSpeciality) = d->_u.registrationData.militarySpeciality;

    if( d->_u.registrationData.hasUserBirthday() )
        ESUSettings::set(ESUSettings::ProfileBirthDate) = d->_u.registrationData.birthDateString();

    if( d->_u.registrationData.hasUserRank() )
        ESUSettings::set(ESUSettings::ProfileRank) = d->_u.registrationData.rank;

    if( d->_u.registrationData.hasStreetAddress() )
        ESUSettings::set(ESUSettings::ProfileStreetAddress) = d->_u.registrationData.streetAddress;
#endif
    // Сохраняем в базу данных
    VariantTable* tableDb = esuDb.getVariantTable("RegistrationTable", d->m_tableSceme);
    if( tableDb == nullptr || d->_u.registrationData.isEmpty() )
        return;

    if( d->isRegistered )
    {
#ifndef ESU_REG_TEST
        QVariantMap record = ESURegistrationTableManager::__createDBRecord(d->_u.registrationData);

        QString condition = QString("role=%1").arg( d->_u.registrationData.role );
        if( tableDb->containsRecord(condition) ) {
            tableDb->updateRecord(record, condition);
        } else {
            tableDb->insert(record);
        }
#endif
    }
}


void ESURegistrationEngine::clearConfiguration()
{
    d->base->setRegistrationState(ESURegistration::UnregisteredState);
    d->base->clearRegistrationData();
    d->_u.registrationData.clear();

#ifdef ESU_APPLICATION
    ESUSettings::set(ESUSettings::ProfileRegistered) = QVariant(d->isRegistered);
    ESUSettings::set(ESUSettings::ProfileRole) = d->_u.registrationData.role;
    ESUSettings::set(ESUSettings::ProfileName) = d->_u.registrationData.name;
    ESUSettings::set(ESUSettings::ProfileRegistrationTime) = QVariant(0);
#endif

    if( d->isRegistered ) {
        d->isRegistered = false;
        if( d->tableManager.containsAddress(d->profileAddress) )
            d->tableManager.removeRecordByAddress(d->profileAddress);
    }
}


bool ESURegistrationEngine::loadRegistrarList()
{
    QFile fileDevice;
    fileDevice.setFileName( d->configurationFile );
    QString addr;
    QDomDocument domDoc("RegistrarList");
    QDomElement rootElement;


    do {
        if( !QFile::exists(d->configurationFile) ) {
#ifdef QT4_ETU
            fileDevice.setFileName("./../../conf/registration_config.xml");
#else
            fileDevice.setFileName(":/mods/registration/conf/registration_config.xml");
#endif
            if( !fileDevice.open(QIODevice::ReadOnly) ) break;
            QByteArray data = fileDevice.readAll();
            fileDevice.close();

            if( data.isEmpty() ) break;
            fileDevice.setFileName( d->configurationFile );

            //возможно здесь должен быть return, а не break?
            if( !fileDevice.open(QIODevice::ReadWrite | QIODevice::Truncate) ) break;
            fileDevice.write(data);
            fileDevice.flush();
            fileDevice.seek(0LL);

        } else {
            //возможно здесь должен быть return, а не break?
            if( !fileDevice.open(QIODevice::ReadWrite) ) break;
        }

        if( !domDoc.setContent(fileDevice.readAll()) ) break;

        rootElement = domDoc.firstChildElement("RegistrarList");
        if( rootElement.isNull() || !rootElement.hasChildNodes() ) break;

        QDomNode recordNode = rootElement.firstChild();
        while( !recordNode.isNull() )
        {
            QDomElement recordElement = recordNode.toElement();
            if( recordElement.isNull() || recordElement.tagName() != "Registrar" ) {
                recordNode = recordNode.nextSibling();
                continue;
            }
            addr = recordElement.text();

            if( !addr.isEmpty() )
            {
                RegistrarInfoSpec spec(addr);
                if( recordElement.hasAttribute("current") ) {
                    if( recordElement.attribute("current") == "true" ) {
                        spec.main = true;
                        d->registrarMain = spec;
                    }
                }

                d->registrarList.append( spec );
            }

            recordNode = recordNode.nextSibling();
        }

        if( !d->registrarList.isEmpty() ) {
            if( d->registrarMain.address.isNull() ) {
                d->registrarMain = d->registrarList.first();
                d->registrarMain.main = true;
            }
            return( true );
        }

    } while(0);

    return( false );
}



// }}} [ CONFIGURATION ]
// [ MESSAGES ]: {{{

void ESURegistrationEngine::sendSystemMsg(Message message, const QString &addr)
{
#ifdef ESU_NET_PROTO_SERVER
    esuNet.sendRegistrationMsg(RegistrationPackageNET( int(message) ), addr);
#else

    emit emitsendRegistrationMsg( RegistrationPackageNET(int(message)), QStringList(addr));

    //    Q_UNUSED(message);
    //    Q_UNUSED(addr);
#endif
}


void ESURegistrationEngine::sendSystemMsgWide(Message message, const QStringList &addrs)
{
#ifdef ESU_NET_PROTO_SERVER
    esuNet.sendRegistrationMsg(RegistrationPackageNET( int(message) ), addrs);
#else

    emit emitsendRegistrationMsg( RegistrationPackageNET(int(message)), addrs);

    //    Q_UNUSED(message);
    //    Q_UNUSED(addrs);
#endif
}


void ESURegistrationEngine::sendUserMsg(const RegistrationPackageNET &p, const QString &addr)
{  
#ifdef ESU_NET_PROTO_SERVER
    if( addr.isEmpty() )
    {
        esuNet.sendRegistrationMsg(p, d->registrarMain.address.toString());
    } else
    {
        esuNet.sendRegistrationMsg(p, addr);
    }
#else

    if( addr.isEmpty() )
    {
        emit emitsendRegistrationMsg( p , QStringList(d->registrarMain.address.toString()));
    } else
    {
        emit emitsendRegistrationMsg( p , QStringList(addr));
    }

    //    Q_UNUSED(p);
    //    Q_UNUSED(addr);
#endif
}


void ESURegistrationEngine::sendRegistrationMsg(const RegistrationPackageNET &p, const QString &addr)
{
#ifdef ESU_NET_PROTO_SERVER
    esuNet.sendRegistrationMsg(p, addr);
#else
    emit emitsendRegistrationMsg(p, QStringList(addr));

    //    Q_UNUSED(p);
    //    Q_UNUSED(addr);
#endif
}


void ESURegistrationEngine::sendErrorMsg(ESURegistration::Error error, const QString &addr)
{
#ifdef ESU_NET_PROTO_SERVER
    esuNet.sendRegistrationMsg(RegistrationPackageNET( (int)ErrorMsg, (int)error ), addr);
#else
    emit emitsendRegistrationMsg(RegistrationPackageNET( (int)ErrorMsg, (int)error ), QStringList(addr));

    //    Q_UNUSED(error);
    //    Q_UNUSED(addr);
#endif
}

void ESURegistrationEngine::sendErrorMsg(ESURegistration::Error error, const QStringList &addrs)
{
#ifdef ESU_NET_PROTO_SERVER
    esuNet.sendRegistrationMsg(RegistrationPackageNET( (int)ErrorMsg, (int)error ), addrs);
#else
    emit emitsendRegistrationMsg(RegistrationPackageNET( (int)ErrorMsg, (int)error ), addrs);

    //    Q_UNUSED(error);
    //    Q_UNUSED(addrs);
#endif
}

// }}} [ MESSAGES ]
// [ USER METHODS ]: {{{

bool ESURegistrationEngine::checkRegistrator()
{
    QString addr = d->registrarMain.address.toString();
    ESU_MESSAGE_LOG(QString("Main registrar at %1").arg(addr), "ESU_REG");
    bool contains = false;

#ifdef ESU_NET_PROTO_SERVER
    contains = esuNetProto.discoverInterface().containsDevice(addr);
#endif

    if( contains )
    {
        d->base->setCurrentRegistrarIndex( d->registrarList.indexOf(d->registrarMain) );
        d->base->setCurrentRegistrarAddress( addr );
        d->base->setRegistrarCheckCount( 0 );
        d->registrarReady = true;

        return( true );

    } else {
        d->base->pushTask( d->base->currentTask() );
        qDebug() << Q_FUNC_INFO << " Current operation = " << d->base->currentTask();
        d->base->setCurrentTask(ESURegistration::CheckRegistrarTask);
        d->base->setOperationState(ESURegistration::WaitState);

        if( d->registrarCheckStack.isEmpty() ) {
            // Инициализируем стек проверки регистраторов
            foreach( const RegistrarInfoSpec& r, d->registrarList ) {
                if( r.address == d->registrarMain.address ) continue;
                d->registrarCheckStack.push(r);
            }
        }
    }

    return( false );
}

// }}} [ USER METHODS ]
// [ REGISTRAR METHODS ]: {{{

void ESURegistrationEngine::processRegistrationRequest(bool accept, const QString &address)
{
    if( d->_r.registrationQueries.isEmpty() || !d->_r.registrationQueries.contains(address) )
        return;

    RegistrationUserInfo info = d->_r.registrationQueries.take(address);
    RegistrationPackageNET p;

    switch (info.request)
    {
    case registration::RegistrationRequest:
    {
        p.msgType = (int)UserRegistrationReplyMsg;

        if( accept ) {
            info.updateTime(); // Дата/время регистрации клиента
            d->tableManager.addRecord(info);

            p.setSuccess();
            p.userData.enable = true;
            p.userData.role = info.role;
            p.userData.time = info.time;

            Q_EMIT d->base->registrationTableUpdated();

        } else {
            ESU_MESSAGE_LOG(QString("Reject registration to %1").arg(address), "RegistrationEngine");
            p.setSuccess(false);
        }

        sendRegistrationMsg(p, address);
        if( accept ) {
            d->tableManager.syncRecord(info.address);
        }
        break;
    }
    case registration::ChangeProfileRequest:
    {
        p.msgType = (int)UserChangeProfileReplyMsg;

        if( accept ) {
            info.updateTime(); // Дата/время обновления данных
            d->tableManager.updateRecord(info.address, info);
            //d->tableManager.syncData();

            p.setSuccess();
            p.userData.enable = true;
            p.userData.role = info.role;
            p.userData.time = info.time;

            Q_EMIT d->base->registrationTableUpdated();

        } else {
            ESU_MESSAGE_LOG(QString("Reject update profile to %1").arg(address), "RegistrationEngine");
            p.setSuccess(false);
        }

#ifdef ESU_REG_TEST
        sendRegistrarMsg(p, "::1");
#else
        sendRegistrationMsg(p, address);
#endif
        if( accept ) {
            d->tableManager.syncRecord(info.address);
        }
        break;
    }
    case registration::UpdateProfileRequest:

        break;
    default:
        break;
    }

    //esuNet->sendRegistrationMsg(p, address);
    //esuNetProto.registrationInterface().sendRegistrationMsg(p, address);
}


void ESURegistrationEngine::processRegistrationRequests(bool accept)
{
    if( d->_r.registrationQueries.isEmpty() )
        return;

    QHash<QString, RegistrationUserInfo>::iterator it = d->_r.registrationQueries.begin();
    while (it != d->_r.registrationQueries.end())
    {
        RegistrationPackageNET p;

        switch (d->base->state())
        {
        case registration::RegistrationRequest:
        {
            p.msgType = (int)UserRegistrationReplyMsg;

            if( accept ) {
                it->updateTime();
                d->tableManager.addRecord(it.value());
                d->tableManager.syncData();
                p.setSuccess();
            } else {
                p.setSuccess(false);
            }

            sendRegistrationMsg(p, it->address.toString());
            break;
        }
        case registration::ChangeProfileRequest:
        {
            p.msgType = (int)UserChangeProfileReplyMsg;

            if( accept ) {
                it.value().updateTime(); // Дата/время обновления данных
                d->tableManager.updateRecord(it->address, it.value());
                //d->tableManager.syncData();

                p.setSuccess();
                p.userData.enable = true;
                //p.userData.role = d->_u.registrationData.role;

                Q_EMIT d->base->registrationTableUpdated();

            } else {
                ESU_MESSAGE_LOG(QString("Reject update profile to %1").arg(it.key()), "RegistrationEngine");
                p.setSuccess(false);
            }
            sendRegistrationMsg(p, it->address.toString());
            break;
        }
        case registration::UpdateProfileRequest:

            break;
        default:
            break;
        }
    }

    d->_r.registrationQueries.clear();

    if( accept ) {
        d->tableManager.syncData();
        Q_EMIT d->base->registrationTableUpdated();
    }
}


void ESURegistrationEngine::registerCurrentData()
{
    QString msg = "Планшет зарегистрирован как:\n";
    d->_u.registrationData = d->_u.tempData;

    d->isRegistered = true;

    d->base->setRegistrationData(d->_u.registrationData);

    d->base->setCurrentMessage(msg);
    d->base->setRegistrationState(ESURegistration::RegisteredState);
    d->base->setState(ESURegistration::SteadyState);
    d->base->setOperationState(ESURegistration::NoOperationState);
    d->base->setError(ESURegistration::NoErrors);

    acceptOperation();

    QMetaObject::invokeMethod(this, "saveConfiguration", Qt::QueuedConnection);
}


void ESURegistrationEngine::registerProfile()
{
    QString msg = "Планшет зарегистрирован как:\n" + d->base->choosedRole();
    d->_u.registrationData = d->base->choosedData();
    d->_u.registrationData.time = std::time(nullptr);
    d->_u.registrationData.address = d->profileAddress;
    d->isRegistered = true;
    d->tableManager.addRecord(d->_u.registrationData);

    d->base->setRegistrationData(d->_u.registrationData);

    d->base->setCurrentMessage(msg);

    d->base->setRegistrationState(ESURegistration::RegisteredState);
    d->base->setState(ESURegistration::SteadyState);
    d->base->setOperationState(ESURegistration::NoOperationState);
    d->base->setError(ESURegistration::NoErrors);

    qApp->processEvents();
    acceptOperation();
    QMetaObject::invokeMethod(this, "saveConfiguration", Qt::QueuedConnection);
}

// }}} [ REGISTRAR METHODS ]
// [ REGISTRATION METHODS ]: {{{

void ESURegistrationEngine::runUserRegistration()
{
    d->_u.registrationData = d->base->choosedData();
    //d->_u.registrationData.role = d->base->choosedRole();
    //d->_u.registrationData.name = d->base->choosedName();

    d->base->setState(ESURegistration::RegisterUserState);
    d->base->setCurrentTask(ESURegistration::RegistrationBeginTask);
    d->_u.userTasks.enqueue(ESURegistration::RegistrationBeginTask);
    Q_EMIT execUserRegistration();
}


void ESURegistrationEngine::changeUserProfile()
{
    d->_u.tempData = d->base->choosedData();

    d->base->setState(ESURegistration::ChangeRegistrationState);
    d->base->setCurrentTask(ESURegistration::ChangeProfileTask);
    d->_u.userTasks.enqueue(ESURegistration::ChangeProfileTask);
    Q_EMIT execUserRegistration();
}


void ESURegistrationEngine::onTaskTimeout()
{
    QString msg;
    QString address;

    switch( d->base->currentTask() )
    {
    case ESURegistration::CheckRegistrarTask:
    {
        if( d->registrarCheckStack.isEmpty() ) {

            d->base->setCurrentMessage("Нет связи с регистраторами");
            d->base->clearCurrentOperation();
            //            d->ui->stopTaskTimer();
            break;

        } else {
            d->registrarMain = d->registrarCheckStack.pop();
            address = d->registrarMain.address.toString();
            d->base->setCurrentRegistrarIndex( d->registrarList.indexOf(d->registrarMain) );
            d->base->setCurrentRegistrarAddress( address );
            d->base->setRegistrarCheckCount( d->registrarCheckStack.count() );
        }

        bool contains = false;

#ifdef ESU_NET_PROTO_SERVER
        contains = esuNetProto.discoverInterface().containsDevice(address);
#endif

        if( contains )
        {
            d->registrarReady = true;
            d->registrarCheckTime = std::time(nullptr);
            d->registrarMain.enabled = true;
            d->registrarCheckStack.clear();
            acceptOperation();

            msg = QString("Связь с регистратором [%1] установлена").
                    arg(d->registrarMain.address.toString());
            d->base->setCurrentMessage(msg);
            d->base->setRegistrarCheckCount( 0 );

            // TODO: здесь запрос даннных
            // Продолжаем выполнять поставленную задачу
            if( !d->base->isTaskStackEmpty() ) {
                d->_u.userTasks.enqueue(d->base->popTask());
                qDebug() << Q_FUNC_INFO << " Current operation = " << d->base->currentTask();
                Q_EMIT execUserRegistration();
            }

        } else {
            d->base->setOperationState(ESURegistration::WaitState);

            RegistrationPackageNET p;
            p.msgType = int(RegistrarEchoRequestMsg);
            if( !d->tableManager.containsAddress( address ) ) {
                p.setRawData("RQST_PROF");
            }
            msg = QString("Проверка связи с регистратором {%1}...").arg(address);
            d->base->setCurrentMessage(msg);

            sendRegistrationMsg((const RegistrationPackageNET&)p, address);
            //            d->ui->startTaskTimer(20000); // 20 секунд
        }

        break;
    }
    default: break;
    }
}

// }}}

/*!
 * \brief ESURegistrationEngine::processUserRegistration - Метод обработки задач регистрации (пользовательская сторона)
 */
void ESURegistrationEngine::processUserRegistration()
{
    static ESURegistration::Task task = ESURegistration::NoTasks;

    // Проверка свзяи с регистраторами
    bool checkTimeout = ( (std::time(nullptr) - d->registrarCheckTime) >= 3600 );

    if( !d->registrarReady || checkTimeout )
    {
        d->registrarReady = false;
        QString msg;
        QString address;

        if( checkRegistrator() ) {
            msg = QString("Связь с регистратором {%1} установлена").arg(d->base->currentRegistrarAddress());
            d->base->setCurrentMessage(msg);

        } else if( !d->registrarCheckStack.isEmpty() ) {

            address = d->registrarMain.address.toString();
            d->base->setCurrentRegistrarAddress( address );
            d->base->setRegistrarCheckCount( d->registrarCheckStack.count() );

            msg = QString("Проверка связи с регистратором {%1}...").arg(address);
            d->base->setCurrentMessage(msg);

            RegistrationPackageNET p; //( int(RegistrarEchoRequestMsg) );
            p.msgType = int(RegistrarEchoRequestMsg);
            if( !d->tableManager.containsAddress( address ) ) {
                p.setRawData("RQST_PROF");
            }
            sendRegistrationMsg((const RegistrationPackageNET&)p, address);

            //            d->ui->startTaskTimer(20000); // 20 секунд
            return;

        } else {
            d->base->setCurrentMessage("Нет связи с регистраторами");
            d->base->clearCurrentOperation();
            return;
        }
    }

    // Цикл задач регистрации
    while( !d->_u.userTasks.isEmpty() )
    {
        task = d->_u.userTasks.dequeue();

        switch( task )
        {
        /* User Tasks */
        case ESURegistration::RegistrationBeginTask:
        {
            RegistrationPackageNET p;
            p.msgType = (int)UserRegistrationRequestMsg;
            p.userData.enable = true;

            if( d->_u.registrationData.birthDate > 0 )
                p.userData.birthDate = d->_u.registrationData.birthDate;
            if( d->_u.registrationData.time > 0 )
                p.userData.time = d->_u.registrationData.time;
            if( d->_u.registrationData.rank >= 0 )
                p.userData.rank = d->_u.registrationData.rank;

            if( !d->_u.registrationData.role.isEmpty() )
                p.userData.role = d->_u.registrationData.role;
            if( !d->_u.registrationData.name.isEmpty() )
                p.userData.name = d->_u.registrationData.name;
            if( !d->_u.registrationData.militarySpeciality.isEmpty() )
                p.userData.militarySpeciality = d->_u.registrationData.militarySpeciality;
            if( !d->_u.registrationData.streetAddress.isEmpty() )
                p.userData.streetAddress = d->_u.registrationData.streetAddress;

            d->base->setCurrentMessage("Ожидание ответа регистраторов");
            sendUserMsg(p);
            //            d->ui->startTaskTimer(d->timeout.registrationReply);
            //d->waitTimer.start(d->timeout.registrationReply);

            break;
        }
        case ESURegistration::ChangeProfileTask:
        {
            RegistrationPackageNET p;
            p.msgType = (int)UserChangeProfileRequestMsg;
            p.userData.enable = true;

            p.userData.role = d->_u.tempData.role;

            if( d->_u.tempData.birthDate > 0 &&
                    d->_u.tempData.birthDate != d->_u.registrationData.birthDate )
                p.userData.birthDate = d->_u.tempData.birthDate;

            if( d->_u.tempData.rank >= 0 && d->_u.tempData.rank != d->_u.registrationData.rank )
                p.userData.rank = d->_u.tempData.rank;

            if( !d->_u.tempData.name.isEmpty() &&
                    d->_u.tempData.name != d->_u.registrationData.name )
                p.userData.name = d->_u.tempData.name;

            if( !d->_u.tempData.militarySpeciality.isEmpty() &&
                    d->_u.tempData.militarySpeciality != d->_u.registrationData.militarySpeciality )
                p.userData.militarySpeciality = d->_u.tempData.militarySpeciality;

            if( !d->_u.tempData.streetAddress.isEmpty() &&
                    d->_u.tempData.streetAddress != d->_u.registrationData.streetAddress )
                p.userData.streetAddress = d->_u.tempData.streetAddress;

            d->base->setCurrentMessage("Ожидание ответа регистраторов");

            sendUserMsg(p);
            //            d->ui->startTaskTimer(d->timeout.registrationReply);
            break;
        }
        default:
            break;
        }
    }
}

// }}} [ REGISTRATION METHODS ]
// [ IListenerESU ]: {{{

/*!
 * \brief ESURegistrationEngine::onReceivedData - Процедура(метод) обработки сетевых пакетов регистрации
 * \param p
 */
void ESURegistrationEngine::onReceivedData(RegistrationPackageNET p)
{
    Message msg;
    msg = static_cast<Message>( p.msgType );

    ESU_STATUS_LOG(QString("Received from [%1] :: MESSAGE = %2").arg(p.address).arg(p.msgType), "ESU_REG");

    switch(msg)
    {
    case ErrorMsg:
    {
        d->base->setError(static_cast<ESURegistration::Error>(p.error));
        processErrorMessage(p);
        break;
    }
    case UserChangeProfileRequestMsg:
    case UserChangeProfileReplyMsg:
    {
        processChangeProfile(msg, p);
        break;
    }
    case RegistrarEchoRequestMsg:
    {
        // [РЕГИСТРАТОР]: Запрос на валидность(доступность) регистратора -
        // посылаем ответ, что регистратор доступен
        if( !d->base->isRegistrar() ) {
            ESU_STATUS_LOG(QString("Tablet is not a registrar..."), "ESU_REG");
            break;
        }
#ifdef ESU_REG_TEST
        p.address = "::1";
#endif
        ESU_STATUS_LOG(QString("Registrar Echo request from [%1]").arg(p.address), "ESU_REG");
        RegistrationPackageNET sendPckg((int)RegistrarEchoReplyMsg);

        if( !p.data.isEmpty() && QString(p.data) == "RQST_PROF" ) {
            if( d->isRegistered ) {
                sendPckg.setUserData(true);
                sendPckg.userData = d->_u.registrationData;
            }
        }
        sendRegistrationMsg(sendPckg, p.address);
        break;
    }
    case RegistrarEchoReplyMsg:
    {
        // [КЛИЕНТ]: Получили подтверждение, что регистратор доступен
        ESU_STATUS_LOG(QString("Registrar Echo reply from [%1] confirmed").arg(p.address), "ESU_REG");

        if( d->base->currentTask() == ESURegistration::CheckRegistrarTask ) {
            d->registrarReady = true;
            d->registrarCheckTime = std::time(nullptr);
            d->registrarMain.enabled = true;
            acceptOperation();
            d->registrarCheckStack.clear();

            QString msg = QString("Связь с регистратором [%1] установлена").
                    arg(d->registrarMain.address.toString());
            d->base->setCurrentMessage(msg);

            if( p.hasUserData() ) {
                RegistrationUserInfo info( p.userData );
                d->tableManager.addRecord( info );
            }

            // Продолжаем выполнять поставленную задачу
            if( !d->base->isTaskStackEmpty() ) {
                d->_u.userTasks.enqueue(d->base->popTask());
                qDebug() << Q_FUNC_INFO << " Current operation = " << d->base->currentTask();
                Q_EMIT execUserRegistration();
            }

        } else break;

        //        d->ui->stopTaskTimer();

        break;
    }
    case UserRegistrationRequestMsg:
    {
        // [РЕГИСТРАТОР]: Поступил запрос на регистрацию пользователя
#ifdef ESU_REG_TEST
        p.address = "::1";
#endif
        if( !p.userData.enable ) {
            sendErrorMsg(ESURegistration::DataNotAvailableError, p.address);
            break;
        }
        else if( d->tableManager.containsAddress(QHostAddress(p.address)) )
        {
#ifndef ESU_REGISTRATION_TEST
            RegistrationUserInfo record = d->tableManager.getRecordByAddress(QHostAddress(p.address));
            RegistrationPackageNET msgError((int)ErrorMsg, (int)ESURegistration::UserAlreadyRegisteredError);
            msgError.userData = record;

            sendRegistrationMsg(msgError, p.address);
            break;
#endif
        } else if( d->_r.registrationQueries.contains(p.address) ) {
            sendErrorMsg(ESURegistration::RegistrationInProcessError, p.address);
            break;
        }

        RegistrationUserInfo user;
        user = p.userData;
        user.setTime(); // Дата и время запроса
        user.request = registration::RegistrationRequest;
        user.message = p.message;
        user.address = QHostAddress(p.address);

        d->_r.registrationQueries.insert( p.address, user );

        // Помещаем в очередь на регистрацию (обработку данных регистратором)
        //        d->ui->modelQueries.appendData(user);

        break;
    }
    case UserRegistrationReplyMsg:
    {
        // [КЛИЕНТ]: Получили уведомление о состоянии регистрации
        //d->waitTimer.stop();
        //        d->ui->stopTaskTimer();
        d->base->clearTasks();
        d->base->setCurrentTask(ESURegistration::NoTasks);

        if( p.isSuccess() )
        {
            QString msg = "Регистрация успешна.\nПланшет зарегистрирован как:\n" + d->base->choosedRole();
            d->_u.registrationData = d->base->choosedData();
            d->_u.registrationData.time = p.userData.time;
            d->_u.registrationData.address = d->profileAddress;
            d->isRegistered = true;
            d->tableManager.addRecord(d->_u.registrationData);

            d->base->setRegistrationData(d->_u.registrationData);

            d->base->setCurrentMessage(msg);

            d->base->setRegistrationState(ESURegistration::RegisteredState);
            d->base->setState(ESURegistration::SteadyState);
            d->base->setOperationState(ESURegistration::NoOperationState);
            d->base->setError(ESURegistration::NoErrors);

            QMetaObject::invokeMethod(d->base->currentUiForm(), "finishProgress", Qt::QueuedConnection);
            qApp->processEvents();

            acceptOperation();

            QMetaObject::invokeMethod(this, "saveConfiguration", Qt::QueuedConnection);
        } else {
            d->base->setCurrentMessage("Регистратор отклонил запрос.");
            d->base->clearUserData();
            rejectOperation();
            QMetaObject::invokeMethod(d->base->currentUiForm(), "finishProgress", Qt::QueuedConnection);
        }
        break;
    }
    case SyncRegistrationTableMsg:
    {
        // [КЛИЕНТ - РЕГИСТРАТОР]: Синхронизация таблицы регистрации
        if( !p.records.enable || p.records.recordsList.isEmpty() )
            break;
        if( p.records.recordsList.size() == 1 ) {
            if( p.records.recordsList.first().netAddress == d->profileAddress )
                break;
        }
        d->tableManager.updateData(p);
        Q_EMIT d->base->registrationTableUpdated();

        break;
    }
    case UnknownMsg:
    default: break;
    }
}

/*!
 * \brief ESURegistrationEngine::processErrorMessage - Обоаботчик ошибок процесса регистрации
 * и других процедур, сваязанных с регистрацией (смена профиля, учетных данных и прочее).
 * \param p
 */
void ESURegistrationEngine::processErrorMessage(const RegistrationPackageNET &p)
{
    QString msg;

    switch( static_cast<ESURegistration::Error>(p.error) )
    {
    case ESURegistration::DataNotAvailableError:
    {
        if( d->base->state() == ESURegistration::RegisterUserState ) {
            d->base->setCurrentMessage("Ошибка регистрации. Регистратор не получил данные.");
            QMetaObject::invokeMethod(d->base->currentUiForm(), "closePage", Qt::QueuedConnection);
        } else if( d->base->state() == ESURegistration::ChangeRegistrationState ) {
            d->base->setCurrentMessage("Ошибка смены профиля. Регистратор не получил данные.");
            QMetaObject::invokeMethod(d->base->currentUiForm(), "closePage", Qt::QueuedConnection);
        }
        break;
    }
    case ESURegistration::RegistrarsNotAvailableError:
    {
        d->base->setCurrentMessage("Нет связи с регистраторами.");
        QMetaObject::invokeMethod(d->base->currentUiForm(), "closePage", Qt::QueuedConnection);
        break;
    }
    case ESURegistration::UserNotRegisteredError:
    {
        if( d->base->state() == ESURegistration::ChangeRegistrationState ) {
            d->base->setCurrentMessage("Планшет не зарегистрирован. Необходима процедура регистрации.\nСброс настроек регистрациии.");
            clearConfiguration();
            d->base->clearCurrentOperation();
            d->base->clearTasks();
        }
        break;
    }
    case ESURegistration::UserAlreadyRegisteredError:
    {
        d->base->setCurrentMessage("Планшет уже зарегистрирован.");
        d->_u.tempData = p;

        bool acceptRole = false;
        if( d->isRegistered ) {
            msg = QString("Планшет уже зарегестрирован как %1").arg(p.userData.role);
            if( !p.userData.name.isEmpty() ) {
                msg += QString(" %2").arg(p.userData.name);
            }
            if( d->_u.registrationData.role != p.userData.role ) {
                msg += "\nПринять зарегистрированный профиль?";
                acceptRole = true;
            }
        } else {
            msg = QString("Планшет уже зарегестрирован как %1").arg(p.userData.role);
            if( !p.userData.name.isEmpty() ) {
                msg += QString(" : %2").arg(p.userData.name);
            }
            msg += "\nПринять зарегистрированный профиль?";
            acceptRole = true;
        }

        if( acceptRole ) {
            QString title = "Внимание";
            QMetaObject::invokeMethod( d->base->currentUiForm(),
                                       "openConfirmationDialog",
                                       Qt::QueuedConnection,
                                       Q_ARG(QVariant, QVariant(msg)),
                                       Q_ARG(QVariant, QVariant(title)));
        }
        break;
    }
    case ESURegistration::RoleAlreadyOccupiedError:
    {
        d->base->setCurrentMessage("Планшет с такой ролью уже зарегистриирован: ");
        d->base->clearCurrentOperation();
        d->base->clearTasks();
        break;
    }
    case ESURegistration::RegistrationInProcessError:
        d->base->setCurrentMessage("Регистрация уже в процессе...");
        break;
    default:
        break;
    }
}


/*!
 * \brief ESURegistrationEngine::processChangeProfile - обработчик процесса смена профиля регистрации
 * \param msg
 * \param p
 */
void ESURegistrationEngine::processChangeProfile(Message msg, const RegistrationPackageNET &p)
{
    QHostAddress addr(p.address);

    switch(msg)
    {
    case UserChangeProfileRequestMsg:
    {
        // [РЕГИСТРАТОР]: Поступил запрос на смену профиля
        if( !d->isRegistrar ) break;
        if( !p.userData.enable ) break;

        if( d->_r.registrationQueries.contains(p.address) ) {
            sendErrorMsg(ESURegistration::RegistrationInProcessError, p.address);
            break;
        }

        // Есть зарегистрированная роль => проверяем совпадение адреса регистрации
        if( d->tableManager.containsRole(p.userData.role) ) {
            RegistrationUserInfo rec = d->tableManager.getRecordByRole(p.userData.role);

            if( rec.address == addr ) {
                // Адрес регистрации совпадает => сообщаем, что планшет уже зарегистрирован
                sendErrorMsg(ESURegistration::UserAlreadyRegisteredError, p.address);
            } else {
                // Адреса не совпадают, значит либо данный профиль уже занят, либо
                // пользователь поменял планшет. Сначала проверяем, существует ли планшет
                // с зарегистрированным адресом
                bool contains = false;
#ifdef ESU_NET_PROTO_SERVER
                contains = esuNetProto.discoverInterface().containsDevice(addr);
#endif
                if( contains ) {
                    // Планшет с таким адресом и ролью существует =>
                    // сообщаем, что выбранная роль занята
                    RegistrationPackageNET errPkg((int)ErrorMsg, (int)ESURegistration::RoleAlreadyOccupiedError);
                    errPkg.setUserData();
                    errPkg.userData.role = rec.role;
                    errPkg.userData.name = rec.name;

#ifdef ESU_REG_TEST
                    sendRegistrarMsg(errPkg, "::1");
#else
                    sendRegistrationMsg(errPkg, p.address);
#endif

                } else {
                    // Планшет с таким именем отсутствует => обновляем адрес регистрации
                    // и сообщаем об обновлении профиля
                    RegistrationUserInfo user( p.userData.role, p.address, p.userData.name );
                    if( user.isEmpty() ) return;
                    user.setTime(); // Дата и время запроса
                    user.request = registration::UpdateProfileRequest;
                    user.updateFlags = registration::AddressFlag;
                    user.message = "Обновление адреса регистрации";
                    d->_r.registrationQueries.insert( p.address, user );
                    //                    d->ui->modelQueries.appendData(user); // UI
                }
            }

            break;

        } else if( d->tableManager.containsAddress(p.address) ) {
            // Такая роль не зарегистрирована, но зарегистрован адрес.
            // Значит пользователь просто меняет роль на планшете. Возможно также, что данные регистрации
            // были сброшены на планшете и пользователь регистрируется по новой с новой ролью.
            // Выдаем запрос на обновление записи в таблице регистрации.

            RegistrationUserInfo currentUser = d->tableManager.getRecordByAddress(p.address);

            if( !p.hasUserData() || !p.hasUserRole() ) {
                RegistrationPackageNET errPkg((int)ErrorMsg, (int)ESURegistration::DataNotAvailableError);
                errPkg.message = "Не указана новая роль";
                sendRegistrationMsg(errPkg, p.address);
                return;
            }

            currentUser.setTime(); // Дата и время запроса
            currentUser.request = registration::ChangeProfileRequest;
            currentUser.message = "Смена роли";
            currentUser.role = p.userData.role;
            currentUser.address = QHostAddress(p.address);

            if( p.hasUserName() ) {
                if( !currentUser.name.isEmpty() && currentUser.name != p.userData.name )
                    currentUser.name = p.userData.name;
                else if( currentUser.name.isEmpty() )
                    currentUser.name = p.userData.name;
            }
            if( p.hasStreetAddress() ) {
                if( !currentUser.streetAddress.isEmpty() && currentUser.streetAddress != p.userData.streetAddress )
                    currentUser.streetAddress = p.userData.streetAddress;
                else if( currentUser.streetAddress.isEmpty() )
                    currentUser.streetAddress = p.userData.streetAddress;
            }
            if( p.hasUserSpeciality() ) {
                if( !currentUser.militarySpeciality.isEmpty() &&
                        currentUser.militarySpeciality != p.userData.militarySpeciality )
                    currentUser.militarySpeciality = p.userData.militarySpeciality;
                else if( currentUser.militarySpeciality.isEmpty() )
                    currentUser.militarySpeciality = p.userData.militarySpeciality;
            }
            if( p.hasUserBirthday() ) {
                if( currentUser.birthDate > 0 && currentUser.birthDate != p.userData.birthDate )
                    currentUser.birthDate = p.userData.birthDate;
                else if( currentUser.birthDate < 0 )
                    currentUser.birthDate = p.userData.birthDate;
            }
            if( p.hasUserRank() ) {
                if( currentUser.rank > 0 && currentUser.rank != p.userData.rank )
                    currentUser.rank = p.userData.rank;
                else if( currentUser.rank < 0 )
                    currentUser.rank = p.userData.rank;
            }

            d->_r.registrationQueries.insert( p.address, currentUser );
            //            d->ui->modelQueries.appendData(currentUser); // UI
            break;

        } else {
            // Ни роль, ни адрес не зарегистрированы.
            sendErrorMsg(ESURegistration::UserNotRegisteredError, p.address);
        }

        break;
    }
    case UserChangeProfileReplyMsg:
    {
        // Ответ регистратора на запрос смены роли.
        //        d->ui->stopTaskTimer();
        d->base->clearTasks();
        d->base->setCurrentTask(ESURegistration::NoTasks);

        if( p.isSuccess() ) {
            QString msg;
            msg = "Профиль изменен.\nПланшет зарегистрирован как:\n" + d->_u.tempData.role;
            d->_u.registrationData.role = d->_u.tempData.role;
            d->_u.registrationData.address = d->profileAddress;
            d->_u.registrationData.setTime( p.userData.time );

            if( d->_u.tempData.hasUserName() ) {
                d->_u.registrationData.name = d->_u.tempData.name;
            }
            if( d->_u.tempData.hasStreetAddress() ) {
                d->_u.registrationData.streetAddress = d->_u.tempData.streetAddress;
            }
            if( d->_u.tempData.hasUserSpeciality() ) {
                d->_u.registrationData.militarySpeciality = d->_u.tempData.militarySpeciality;
            }
            if( d->_u.tempData.hasUserBirthday() ) {
                d->_u.registrationData.birthDate = d->_u.tempData.birthDate;
            }
            if( d->_u.tempData.hasUserRank() ) {
                d->_u.registrationData.rank = d->_u.tempData.rank;
            }

            d->isRegistered = true;
            d->tableManager.updateRecord(d->_u.registrationData.address, d->_u.registrationData);

            d->base->setRegistrationData(d->_u.registrationData);
            acceptOperation();

            d->base->setCurrentMessage(msg);
            d->base->setRegistrationState(ESURegistration::RegisteredState);
            d->base->setState(ESURegistration::SteadyState);
            d->base->setError(ESURegistration::NoErrors);

            QMetaObject::invokeMethod(this, "saveConfiguration", Qt::QueuedConnection);

        } else {
            d->base->setCurrentMessage("Профиль не изменен. Регистратор отклонил запрос.");
            d->base->clearChoosedData();
            rejectOperation();
        }

        QMetaObject::invokeMethod(d->base->currentUiForm(), "finishProgress", Qt::QueuedConnection);
        break;
    }
    default:
        break;
    }
}

// }}} [ IListenerESU ]
