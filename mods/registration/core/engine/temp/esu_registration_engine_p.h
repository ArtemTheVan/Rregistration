#ifndef ESU_REGISTRATION_ENGINE_P_H
#define ESU_REGISTRATION_ENGINE_P_H

#include "esu_registration_engine.h"
#include "esu_registration_engine_types.h"

#include "esu_application.h"
#include "esu_logger.h"
#include "esu_settings.h"
#include "esu_net_system.h"

#include <QtXml>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

#define WAIT_OPERATION_REPLY 10000



// [Class ESURegistrationEnginePrivate]:[Implementation]: {{{

class ESURegistrationEnginePrivate
{
    Q_DECLARE_PUBLIC(ESURegistrationEngine)

public:
    ESURegistrationEnginePrivate(ESURegistrationEngine* parent = nullptr);
    ~ESURegistrationEnginePrivate();

    void init();

public:
    void sendMsg(quint32 id, ESURegistrationEngine::Message message, const QString& addr = QString());
    void sendMsg(quint32 id, ESURegistrationEngine::Message message, const QByteArray& data, const QString& addr = QString());
    void sendMsgWide(quint32 id, ESURegistrationEngine::Message message, const QStringList& addrs);
    void sendError(quint32 id, ESURegistration::Error err, const QString& addr = QString());

    void registrarNotify();
    bool checkRegistrarAvailability();
    QString requestRegistrarAddress();

    // [ PROPERTIES ]:

    void setRegistrarAddress(const QString& addr);
    inline bool hasError() const { return( error != ESURegistration::NoErrors ); }
    inline void clearError() { error = ESURegistration::NoErrors; }

    // [ REGISTRATION TABLE METHODS ]:

    bool checkRegTableFile(bool* hasContent = nullptr);
    bool updateRegTable(const QString& addr, const QString& name);
    void updateRegTable(const RegistrationTableData& srcTable, RegistrationTableData& dstTable);

    bool loadRegTable();
    bool loadRegTableFromJson(const QByteArray& dataJSON);
    bool loadRegTableFromXML(const QByteArray& dataXML);

    bool syncRegTable(const RegistrationTableData& updateTable, const QString& addr = QString());
    bool syncFullRegTable(const RegistrationTableData& srcTable, const QString& addr = QString());
    bool saveRegTable(const RegistrationTableData& srcTable);

    QByteArray convertRegTableToXML(const RegistrationTableData& table);
    QByteArray convertRegTableToJSON(const RegistrationTableData& table);

    RegistrationTableData convertRegTableFromXML(const QByteArray& dataXML);
    RegistrationTableData convertRegTableFromJSON(const QByteArray& dataJSON);

public:
    ESURegistrationEngine* q_ptr;
    ESURegistration* base;
    QString regTablePath;
    QString registeredName;

    volatile bool registered;
    volatile bool registrar;
    volatile bool regTableReady;    
    QString registrarAddress;
    QString currentAddress;
    volatile ESURegistration::State state;
    volatile ESURegistrationEngine::RegistrarState registrarState;
    volatile ESURegistration::Mode mode;
    volatile ESURegistration::Error error;
    QMutex mutParams;

    volatile bool operationAccepted;
    volatile bool locked;
    QWaitCondition condWait;
    QMutex mutWait;

    QQueue<RegisterRecordSpec> regQueue;    
    QMutex mutRegQueue;

    QQueue<RegistrarTaskSpec> tasksQueue;
    QMutex mutTasks;
    QSemaphore semTasks;

    QMutex mutRegTable;
    RegistrationTableData regTableSync;
    RegistrationTableData regTable;     // <address, name>

    QBasicTimer timer;
};

// }}} [Class ESURegistrationEnginePrivate]

#endif // ESU_REGISTRATION_ENGINE_P_H
