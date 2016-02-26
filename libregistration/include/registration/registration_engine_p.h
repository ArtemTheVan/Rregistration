#ifndef ESU_REGISTRATION_ENGINE_PRIVATE_H
#define ESU_REGISTRATION_ENGINE_PRIVATE_H


#include "registration_engine.h"
#include "registration_table_manager.h"
#include "registration_events.h"
#include "registration_net.h"

//#include <QtXml>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#endif

#ifdef ESU_APPLICATION
#include "application.h"
#include "settings.h"
#endif

#include <logger/logger.h>
#include <storage/localstoragemanager.h>


#define CHECK_REGISTRAR_TIMEOUT         60000//60000
#define REGISTRATION_REQUEST_TIMEOUT    2*60000


class ESURegistrationEnginePrivate
{
    Q_DECLARE_PUBLIC(ESURegistrationEngine)

public:
    ESURegistrationEnginePrivate(ESURegistrationEngine* parent = nullptr):
        q_ptr(parent), base(nullptr)/*, ui(nullptr)*/
      , configurationFileName(""), configurationFile("")
      , isRegistered(false), isRegistrar(false), profileName(""), profileAddress(QHostAddress::Null)
      , registrarReady(false), registrarCheckTime(0)
    {
        timeout.registrarReply      = CHECK_REGISTRAR_TIMEOUT;
        timeout.registrationReply   = REGISTRATION_REQUEST_TIMEOUT;
    }

    ~ESURegistrationEnginePrivate()
    { }

    void init()
    {
        Q_Q(ESURegistrationEngine);

        configurationFileName = "registration_config.xml";
        configurationFile = QString(ESU_APP_CONFIG_PATH) + configurationFileName;

#ifdef ESU_NET_PROTO_SERVER
        RegistrationProtoInterfaceNET& net = esuNetProto.registrationInterface();
        QObject::connect( &net, &RegistrationProtoInterfaceNET::registrationMessageReceived,
                          q, &ESURegistrationEngine::onReceivedData, Qt::QueuedConnection );
#endif
#ifdef QT4_ETU
        QObject::connect( q, SIGNAL(execUserRegistration()),
                          q, SLOT(processUserRegistration()), Qt::QueuedConnection );
#else
        QObject::connect( q, &ESURegistrationEngine::execUserRegistration,
                          q, &ESURegistrationEngine::processUserRegistration, Qt::QueuedConnection );
#endif

        m_tableSceme["role"]        = "VARCHAR(255) NOT NULL PRIMARY KEY"; // unique
        m_tableSceme["address"]     = "VARCHAR(255) NOT NULL UNIQUE";
        m_tableSceme["name"]        = "VARCHAR(255)";
        m_tableSceme["time"]        = "INTEGER";        // UNIX TIMESTAMP SINCE 1970
        m_tableSceme["speciality"]  = "VARCHAR(100)";
        m_tableSceme["rankIndex"]   = "INTEGER";
        m_tableSceme["homeAddress"] = "BLOB";
        m_tableSceme["birthDate"]   = "INTEGER";        // UNIX TIMESTAMP SINCE 1970

    }

    inline void setUserTask(ESURegistration::Task t);
    inline void setRegistrarTask(ESURegistration::Task t);

public:
    ESURegistrationEngine   *q_ptr;
    ESURegistration         *base;
//    ESURegistrationUI       *ui;

    QString configurationFileName;
    QString configurationFile;

    bool isRegistered;
    bool isRegistrar;
    QString profileName;
    QHostAddress profileAddress;

    bool    registrarReady;     // Флаг, указывающий, что регистратор доступен
    int     registrarCheckTime; // Время последней проверки регистратора(ов).
                                // Регистратор(ы) должен проверяться через каждый час

    ESURegistrationTableManager tableManager;

    QList<RegistrarInfoSpec>    registrarList;
    RegistrarInfoSpec           registrarMain;    
    QStack<RegistrarInfoSpec>   registrarCheckStack;
    QStringList                 registrarActiveAddrs;

    struct {
        QQueue<ESURegistration::Task>   userTasks;
        RegistrationUserInfo            registrationData;
        RegistrationUserInfo            tempData;
    } _u;

    struct {
        QQueue<ESURegistration::Task>           registrarTasks;
        QHash<QString, RegistrationUserInfo>    registrationQueries;
        QHash<QString, RegistrationUserInfo>    updateQueries;
        RegistrationUserInfo                    currentUser;
    } _r;

    struct {
        int registrarReply;
        int registrationReply;
    } timeout;

    QTimer waitTimer;
    int waitTimeout;

    QVariantMap m_tableSceme;
};



void ESURegistrationEnginePrivate::setUserTask(ESURegistration::Task t)
{
    base->setCurrentTask(t);
    _u.userTasks.enqueue(t);
}


void ESURegistrationEnginePrivate::setRegistrarTask(ESURegistration::Task t)
{
    base->setCurrentTask(t);
    _r.registrarTasks.enqueue(t);
}

#endif // ESU_REGISTRATION_ENGINE_PRIVATE_H
