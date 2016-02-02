#ifndef ESU_REGISTRATION_ENGINE_H
#define ESU_REGISTRATION_ENGINE_H

#include <QtCore>

#include "esu_registration.h"
#include "esu_globals_id.h"

#include "i_listener_esu.h"



class ESURegistrationEnginePrivate;

class ESURegistrationEngine : public QThread,
        public IListenerESU
{
    Q_OBJECT

public:
    enum Message {
        UnknownMsg,
        RegistrarAddrRequestMsg, RegistrarAddrReplyMsg, RegistrarAddrNotifyMsg,
        RegistrarEchoRequestMsg, RegistrarEchoReplyMsg,
        RegisterDeviceRequestMsg,
        WaitMsg,
        RegistrationBeginingMsg, RegistrationCanceledMsg, RegistrationSuccessfullMsg,
        RegistrationErrorMsg,
        RegTableSyncMsg, RegTableSyncFullMsg
    };

    enum RegistrarState {
        RegistrarSteady, RegistrarReady, RegistrarBusy
    };

    enum Task {
        UnknownTask,
        InitTask,
        RegisterUserTask,
        RequestRegistrationTask,
        UpdateRegTableTask,
        StopEngineTask
    };

public:
    explicit ESURegistrationEngine(QObject *parent = nullptr);
    ~ESURegistrationEngine();

protected:    
    void run();

    void runInit();
    void runUserRegistration();
    void runRegistrarTasks();    

    void waitOperation(quint64 time = ULONG_MAX);
    void resumeOperation(bool accept = true);
    bool isOperationAccepted();
    bool isOperationRejected();
    inline void acceptOperation() { resumeOperation(true); }
    inline void rejectOperation() { resumeOperation(false); }

    // [ PROTECTED PROPERTIES]:
    void setState(ESURegistration::State state);
    void setMode(ESURegistration::Mode mode);
    void setError(ESURegistration::Error error);

    void sendTask(Task task);

public:
    // [ PROPERTIES ]:
    bool isRegistrar() const;
    bool isRegistered() const;

    ESURegistration::State state() const;
    ESURegistration::Mode mode() const;
    ESURegistration::Error error() const;

    QString currentUserAddress() const;
    QString registeredName() const;

    void setRegistrationPtr(ESURegistration* parent);

    // [ IListenerESU ]: {{
public:
    quint32 ID() const { return(ESU_REGISTRATION_ID); }
    bool isObject() const { return(true); }
    QObject* toObject() { return(this); }

public Q_SLOTS:
    void onProcessAppMessage(quint32 senderID, int msg);
    void onReceivedData();

    // }} [ IListenerNET ]

protected:
    void customEvent(QEvent *);
    void timerEvent(QTimerEvent *e);

Q_SIGNALS:
    void saveConfigurationNotify();

private Q_SLOTS:
    // [PRIVATE/INTERNAL SLOTS]:
    void saveConfiguration();

protected:
    ESURegistrationEnginePrivate* d;

private:
    Q_DECLARE_PRIVATE_D(ESURegistrationEngine::d, ESURegistrationEngine)
    Q_DISABLE_COPY(ESURegistrationEngine)

    Q_FRIEND(ESURegistration)
    Q_FRIEND(ESURegistrationPrivate)
};

#endif // ESU_REGISTRATION_ENGINE_H
