#include "esu_registration_engine.h"
#include "esu_registration_engine_p.h"

#define NEW_ENGINE

// [Class ESURegistrationEngine]:

// [ INIT/DESTROY ]: {{{


ESURegistrationEngine::ESURegistrationEngine(QObject *parent):
    QThread(parent)
  , d(new ESURegistrationEnginePrivate(this))
{
    esuApp->addListener(this);
    esuNet->addListener(this, ESU::QtMetaInvokeDispatch);

#ifdef NEW_ENGINE
     d->init();
#endif

    //d->mutParams.lock();
    d->registered = ESUSettings::get(ESUSettings::Registered, ESUSettings::Registration).toBool();
    d->registrar = ESUSettings::get(ESUSettings::Registrar, ESUSettings::Registration).toBool();
    d->registrarAddress = ESUSettings::get(ESUSettings::RegistrarAddress, ESUSettings::Registration).toString();
    //d->mutParams.unlock();

    connect(this, &ESURegistrationEngine::saveConfigurationNotify,
            this, &ESURegistrationEngine::saveConfiguration, Qt::QueuedConnection);
}


ESURegistrationEngine::~ESURegistrationEngine()
{
    delete d;
}


// }}} [ INIT/DESTROY ]
// [ BASE METHODS ]:

void ESURegistrationEngine::run()
{
    if( !d->base ) return;

    RegistrarTaskSpec task;

    sleep(3);

    Q_FOREVER {

        d->semTasks.acquire();
        d->mutTasks.lock();
        if( !d->tasksQueue.isEmpty() )
            task = d->tasksQueue.dequeue();
        d->mutTasks.unlock();

        switch(task.task)
        {
        case InitTask:
        {
            runInit();
            if( !d->registrar && !d->registered ) {
                sendTask(RequestRegistrationTask);
            }
            break;
        }
        case RequestRegistrationTask:
        {
            runUserRegistration();
            break;
        }
        case RegisterUserTask:
        {
            if( !isRegistrar() ) break;
            runRegistrarTasks();
            break;
        }
        case UpdateRegTableTask:
        {
            QMutexLocker locker(&d->mutRegTable);

            if( d->regTableSync.isEmpty() ) break;

            d->syncRegTable(d->regTableSync);
            d->updateRegTable(d->regTableSync, d->regTable);
            d->saveRegTable(d->regTable);
            d->regTableSync.clear();
            break;
        }
        default:
            break;
        }
    }
}


void ESURegistrationEngine::runInit()
{
    Q_CHECK_PTR(d->base);

    // Инициализация
    if( d->mode != ESURegistration::InitMode ) return;

    // Загружаем таблицу регистрации
    d->loadRegTable();
    setState(ESURegistration::CheckProfile);

    // Проверяем режим
    if( d->registrar ) {
        if( d->registrarAddress.isEmpty() ) {
            d->setRegistrarAddress(esuNet->localAddress());
        }
        d->registrarState = RegistrarReady;
        setState(ESURegistration::RegistrarProfileDetected);
        d->mode = ESURegistration::RegistrarMode;
        d->registrarNotify();
    } else {
        setState(ESURegistration::UserProfileDetected);
        d->mode = ESURegistration::UserMode;
    }

    // Проверка регистрации
    if( d->registered ) {
        setState(ESURegistration::Registered);
    }
}


void ESURegistrationEngine::runUserRegistration()
{
    QString registrarAddr;

    setState(ESURegistration::RegistrationInProcess);
    sleep(2);

    d->mutParams.lock();
    registrarAddr = d->registrarAddress;
    d->mutParams.unlock();

    // Если имеется адрес регистратора, проверяем его валидность (доступность),
    // иначе запрашиваем адрес регистратора
    if( !d->checkRegistrarAvailability() ) {
        registrarAddr = d->requestRegistrarAddress();

        ESU_MESSAGE_LOG(QString("Registrar addr: [%1]").arg(registrarAddr), "ESU_REGISTRATION");
        // Не дождались ответа от регистратора, значит сами становимся регистратором
        if( d->state != ESURegistration::RegistrarAddrReceived && registrarAddr.isEmpty() )
        {
            // TODO: Опросить другие планшеты, стали ли они тоже регистраторами - если да,
            //       то регитсратором становится самый первый (по времени)
            // Оповещаем других, что мы становимся регистратором.
            d->registrarNotify();

            d->registrar = true;
            d->registrarState = RegistrarReady;
            d->setRegistrarAddress(esuNet->localAddress());
            setState(ESURegistration::BecomesRegistrar);
            Q_EMIT saveConfigurationNotify();

            // немного ждем
            sleep(2);
            return;
        }
    }

    // Регистратор доступен. Отправляем запрос на регистрацию
    setState(ESURegistration::RegistrationRequest);
    sleep(2);
    d->sendMsg(ID(), RegisterDeviceRequestMsg, registrarAddr);
}


void ESURegistrationEngine::runRegistrarTasks()
{
    RegisterRecordSpec requestRec;
    RegisterRecordSpec existsRec;
    bool userExists = false;

    Q_FOREVER {
        d->mutRegQueue.lock();
        if( !d->regQueue.isEmpty() ) {
            requestRec = d->regQueue.dequeue();
            //ESU_MESSAGE_LOG(QString("Registrar: process [%1]").arg(currentUser), "ESU_REG");
            if( requestRec.isEmpty() ) { d->mutRegQueue.unlock(); continue; }
        } else { d->mutRegQueue.unlock(); break; }        

        d->mutRegTable.lock();
        if( d->regTable.contains(requestRec.address) ) {
            existsRec = d->regTable[requestRec.address];
            userExists = true;
        } else userExists = false;
        d->mutRegTable.unlock();

        if( userExists ) {
            //d->sendError(ID(), ESURegistration::UserAlreadyRegisteredError, currentUser);
            // TODO: !!! Add new processing !!!
            ESURegistrationEvent* e = new ESURegistrationEvent(ESURegistrationEvent::UserAlreadyRegistered);
            e->setAddress( requestRec.address );
            e->setName( requestRec.name );
            esuApp->sendEvent(ESU_REGISTRATION_UI_ID, e, ID(), ESU::QtMetaInvokeDispatch);
            waitOperation();
            // Уведомляем UI регистратора, что пользователь уже зарегестрирован. Выдаем запрос, нужно ли
            // выполнить перерегистрацию или отклонить запрос.


            d->mutRegQueue.unlock();
            continue;
        }
        d->mutRegTable.unlock();

        // Если очередь непустая, посылаем остальным уведомление на ожидание в очереди
        if( !d->regQueue.isEmpty() ) {
            QStringList addrList;
            for( const RegisterRecordSpec& r: d->regQueue  )
                addrList.append(r.address);
            d->sendMsgWide(ID(), WaitMsg, addrList);
        }
        d->mutRegQueue.unlock();

        // Уведомляем пользователя о начале регистрации
        d->sendMsg(ID(), RegistrationBeginingMsg, requestRec.address);
        d->registrarState = RegistrarBusy;

        // Запускаем UI для выбора профиля пользователя
        //esuApp->sendMessage(ESU_STAFF_UI_ID, (int)ESUEvent::RegisterUser, currentUser.toLocal8Bit(), ID(), ESU::QtDispatch);
        ESURegistrationEvent* event = new ESURegistrationEvent(ESURegistrationEvent::RegisterUser, requestRec.address);
        esuApp->sendEvent(ESU_STAFF_UI_ID, event, ID(), ESU::QtMetaInvokeDispatch);

        // TODO: Make time check for all wait operations
        // Ждем уведомления от модуля UI штатного состава
        waitOperation();
    }
}

// [ WAIT/RESUME ]

void ESURegistrationEngine::waitOperation(quint64 time)
{
    d->mutWait.lock();
    d->locked = true;
    d->condWait.wait(&d->mutWait, time);
    d->mutWait.unlock();
}

void ESURegistrationEngine::resumeOperation(bool accept)
{
    d->mutWait.lock();
    if( d->locked ) {
        d->condWait.wakeAll();
        d->locked = false;
    }
    d->operationAccepted = accept;
    d->mutWait.unlock();
}

bool ESURegistrationEngine::isOperationAccepted()
{
    return( (bool)d->operationAccepted );
}

bool ESURegistrationEngine::isOperationRejected()
{
    return( (bool)(!d->operationAccepted) );
}

// }}} [ BASE METHODS ]
// [ PROTECTED PROPERTIES]: {{{

void ESURegistrationEngine::setState(ESURegistration::State state)
{
    d->state = state;
    Q_EMIT d->base->stateChangedNotify((int)d->state);
}


void ESURegistrationEngine::setMode(ESURegistration::Mode mode)
{
    d->mode = mode;
    Q_EMIT d->base->modeChanged();
}


void ESURegistrationEngine::setError(ESURegistration::Error error)
{     
    d->error = error;

    d->state = ESURegistration::RegistrationError;
    Q_EMIT d->base->stateChangedNotify((int)d->state);
    Q_EMIT d->base->errorChanged();
}


void ESURegistrationEngine::sendTask(ESURegistrationEngine::Task task)
{
    d->mutTasks.lock();
    d->tasksQueue.enqueue(RegistrarTaskSpec(task));
    d->mutTasks.unlock();
    d->semTasks.release();
}

// }}} [ PROTECTED PROPERTIES]
// [ PROPERTIES ]: {{{

bool ESURegistrationEngine::isRegistrar() const
{
    return( d->registrar );
}


bool ESURegistrationEngine::isRegistered() const
{
    return( d->registered );
}


ESURegistration::State ESURegistrationEngine::state() const
{
    return( d->state );
}


ESURegistration::Mode ESURegistrationEngine::mode() const
{
    return( d->mode );
}


ESURegistration::Error ESURegistrationEngine::error() const
{
    return( d->error );
}


QString ESURegistrationEngine::currentUserAddress() const
{
    return( QString() );
}


QString ESURegistrationEngine::registeredName() const
{
    QMutexLocker locker(&d->mutParams);
    return( d->registeredName );
}


void ESURegistrationEngine::setRegistrationPtr(ESURegistration *parent)
{
    d->base = parent;
}

// }}} [ PROPERTIES ]
// [IEventListener]: {{{

void ESURegistrationEngine::onProcessAppMessage(quint32 senderID, int msg)
{
    Q_UNUSED(senderID);

    ESURegistrationEvent::EventType type = static_cast<ESURegistrationEvent::EventType>(msg);

    switch(type)
    {
    case ESURegistrationEvent::RegistrationSuccessfull:

        break;
    case ESURegistrationEvent::RegistrationCanceled:

        break;
    default:
        break;
    }
}

// }}} [IEventListener]
// [IListenerNET]: {{{

void ESURegistrationEngine::onReceivedData()
{
    Message msg;    
    IncomingRawQueueNET dataQueue;

    if( !esuNet->getData(dataQueue, ID()) ) return;

    while( !dataQueue.isEmpty() )
    {
        InputRawPackageNET p = dataQueue.dequeue();
        if( p.data.isEmpty() ) continue;
        msg = static_cast<Message>( p.data.at(0) );
        p.data.remove(0, 1);

        ESU_STATUS_LOG(QString("Received from [%1]").arg(p.address), "ESU_REG");

        switch(msg)
        {
        case RegistrarEchoRequestMsg:
        {
            // [РЕГИСТРАТОР]: Запрос на валидность(доступность) регистратора -
            // посылаем ответ, что регистратор доступен
            if( !d->registrar ) break;
            ESU_STATUS_LOG(QString("Registrar Echo request from [%1]").arg(p.address), "ESU_REG");
            d->sendMsg(ID(), RegistrarEchoReplyMsg, p.address);

            break;
        }
        case RegistrarEchoReplyMsg:
        {
            // [КЛИЕНТ]: Получили подтверждение, что регистратор доступен
            setState(ESURegistration::CheckRegistrarSuccess);
            ESU_STATUS_LOG(QString("Registrar[%1] confirmed").arg(p.address), "ESU_REG");
            resumeOperation();

            break;
        }
        case RegistrarAddrRequestMsg:
        {
            // [РЕГИСТРАТОР]: Запрашиваем адрес регистратора
            if( !isRegistrar() ) break;

            // Мы - регистратор. Посылаем отклик.
            d->sendMsg(ID(), RegistrarAddrReplyMsg, p.address);

            break;
        }
        case RegistrarAddrReplyMsg:
        {
            // [КЛИЕНТ]: Получили адрес регистратора.
            if( isRegistrar() ) break;

            d->mutParams.lock();
            d->registrarAddress = p.address;
            d->mutParams.unlock();
            setState(ESURegistration::RegistrarAddrReceived);
            // Если мы находимся в состоянии ожидания, снимаем блокировку потока.
            resumeOperation();
            break;
        }
        case RegistrarAddrNotifyMsg:
        {
            // [КЛИЕНТ]: Получили уведомление о регистраторе.
            if( d->registrar ) break; // TODO: Сделать правильную обработку

            d->mutParams.lock();
            d->registrarAddress = p.address;
            d->mutParams.unlock();

            // Ноходимся в состоянии ожидания отклика от регистратора
            if( d->state == ESURegistration::RequestRegistrar ) {
                setState(ESURegistration::RegistrarAddrReceived);
                resumeOperation();
            } else if( d->state == ESURegistration::CheckRegistrar ) {
                setState(ESURegistration::CheckRegistrarSuccess);
                resumeOperation();
            }

            break;
        }
        case RegisterDeviceRequestMsg:
        {
            // [РЕГИСТРАТОР]: Запрос на регистрацию. Обновляем очередь запросов.
            // TODO: Сделать обработку для ФИО
            if( !d->registrar ) break;

            d->mutRegQueue.lock();
            RegisterRecordSpec spec(p.address);
            if( !d->regQueue.contains( spec ) ) {
                d->regQueue.enqueue( spec );
                d->regTableSync.insert(p.address, spec);
            }
            d->mutRegQueue.unlock();
            sendTask(RegisterUserTask);

            break;
        }
        case WaitMsg:
        {
            // [КЛИЕНТ]: Регистратор занят. Уведомляем UI, что запрос находится в очереди на обработку.
            setState(ESURegistration::WaitState);
            break;
        }
        // [ REGISTRATIONS STATES ]:
        case RegistrationBeginingMsg:
        {
            // [КЛИЕНТ]: Регистрация началась. Уведомляем UI.
            setState(ESURegistration::RegistrationBegining);
            d->timer.start(2*60000, this);
            break;
        }
        case RegistrationCanceledMsg:
            // [КЛИЕНТ]: Регистрация отменена (отклонена регистратором). Уведомляем UI.
            setState(ESURegistration::RegistrationCanceled);
            d->timer.stop();
            break;
        case RegistrationSuccessfullMsg:
        {
            // [КЛИЕНТ]: Регистрация завершена успешно. Уведомляем UI и сохраняем полученную конфигурацию.
            //           Обновляем модуль штатного состава.
            d->timer.stop();

            QString name;
            QByteArray xmlData;
            char msg_buf[2048];
            int off = 0;
            int size = 0;
            const char* data = (p.data.constData());
            memset(msg_buf, 0x00, sizeof(msg_buf));

            // REG_NAME [COMPRESSED]
            memcpy(&size, (data + off), sizeof(int)); off += sizeof(int);
            qDebug() << "name size = " << size;
            memcpy(msg_buf, (data + off), size); off += size;

            name = QString::fromLocal8Bit( qUncompress( (const uchar*)msg_buf, size ) );
            qDebug() << "name = " << name;

            // XML_DATA [COMPRESSED]
            size = 0;
            memset(msg_buf, 0x00, sizeof(msg_buf));
            memcpy(&size, (data + off), sizeof(int)); off += sizeof(int);
            qDebug() << "xml size = " << size;
            memcpy(msg_buf, (data + off), size); off += size;
            xmlData = qUncompress( (const uchar*)msg_buf, size );
            qDebug() << "[XML]:\n" << QString(xmlData);

            if( xmlData.isEmpty() ) break;

            d->mutParams.lock();
            d->registeredName = name;
            d->mutParams.unlock();

            d->registered = true;
            Q_EMIT saveConfigurationNotify();

            ESURegistrationEvent* e = new ESURegistrationEvent(ESURegistrationEvent::UpdateRegistrationData,
                                                               p.address, xmlData, name);
            esuApp->sendEvent(ESU_STAFF_UI_ID, e, ID());

            setState(ESURegistration::RegistrationSuccessfull);

            break;
        }
        // [ REG TABLE SYNC ]:
        case RegTableSyncMsg:
        {
            QByteArray syncData = qUncompress(p.data);
            if( syncData.isEmpty() ) break;

            RegistrationTableData regTableSync = d->convertRegTableFromJSON(syncData);
            if( regTableSync.isEmpty() ) break;

            d->mutRegTable.lock();
            d->updateRegTable(d->regTable, regTableSync);
            d->saveRegTable(d->regTable);
            d->mutRegTable.unlock();

            break;
        }
        case RegTableSyncFullMsg:
        {
            break;
        }
        // [ ERRORS ]:
        case RegistrationErrorMsg:
        {
            // [КЛИЕНТ]: Ошибка регистрации. Уведомляем UI.
            setError(static_cast<ESURegistration::Error>( p.data.at(1) ));
            break;
        }
        default:
            break;
        }
    }
}

// }}} [IListenerNET]
// [ EVENTS PROCESS ]: {{{

void ESURegistrationEngine::customEvent(QEvent *e)
{
    ESURegistrationEvent* event = static_cast<ESURegistrationEvent*>(e);

    if( event == nullptr || event->type() != (QEvent::Type)ESUEvent::Registration ) {
        e->ignore();
        QThread::customEvent(e);
        return;
    }

    switch(event->eventType())
    {
    case ESURegistrationEvent::RegistrationSuccessfull:
    {
        e->accept();

        char msg_buf[2048];
        int off = 0;
        int size;
        QByteArray data;
        bool sync = false;

        // [ REGISTRATION TABLE ]
        d->mutRegQueue.lock();
        sync = d->regQueue.isEmpty();
        d->mutRegQueue.unlock();
        if( sync ) sendTask(UpdateRegTableTask);

        // [ SYNC STAFF DATA ]

        // MSG
        msg_buf[0] = (qint8)RegistrationSuccessfullMsg;
        off += 1;
        // REG_NAME [COMPRESSED]
        data = qCompress(event->name().toLocal8Bit(), 9);
        size = data.size();
        memcpy( (msg_buf+off), &size, sizeof(int) ); off += sizeof(int);
        memcpy( (msg_buf+off), data.constData(), size ); off += size;

        // XML_DATA [COMPRESSED]
        data = qCompress(event->data(), 9);
        size = data.size();
        memcpy( (msg_buf+off), &size, sizeof(int) ); off += sizeof(int);
        memcpy( (msg_buf+off), data.constData(), size ); off += size;
        size = off;

        esuNet->sendData(msg_buf, size, ID(), ID(), event->address());
        resumeOperation();

        break;
    }
    case ESURegistrationEvent::RegistrationCanceled:
    {
        e->accept();
        d->sendMsg( ID(), RegistrationCanceledMsg, event->address() );
        resumeOperation();
        break;
    }
    default:
        e->ignore();
        break;
    }

    QThread::customEvent(e);
}


void ESURegistrationEngine::timerEvent(QTimerEvent *e)
{
    if( e->timerId() != d->timer.timerId() ) {
            QObject::timerEvent(e);
            return;
    }

    switch(d->state)
    {
    case ESURegistration::RegistrationBegining:
    {
        e->accept();
        if( !d->registered )
            setError(ESURegistration::TimeoutExpiredError);
        d->timer.stop();
        break;
    }
    default:        
        e->ignore();
        d->timer.stop();
        break;
    }

    QObject::timerEvent(e);
}


// }}} [ EVENTS PROCESS ]
// [PRIVATE/INTERNAL SLOTS]: {{{

void ESURegistrationEngine::saveConfiguration()
{
    ESUSettings::set(ESUSettings::Registered, ESUSettings::Registration) = QVariant(d->registered);
    ESUSettings::set(ESUSettings::Registrar, ESUSettings::Registration) = QVariant(d->registrar);
    ESUSettings::set(ESUSettings::RegistrarAddress, ESUSettings::Registration) = QVariant(d->registrarAddress);
}

// }}} [PRIVATE/INTERNAL SLOTS]
