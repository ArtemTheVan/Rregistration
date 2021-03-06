#ifndef ESU_REGISTRATION_ENGINE_H
#define ESU_REGISTRATION_ENGINE_H

#include <QObject>
#include <QStringList>

#include "registration.h"
#include "registration_global.h"
#include "registration_engine_types.h"
#include "registration_table_manager.h"


class ESURegistrationEnginePrivate;
class ESURegistrationTableManager;

class ESURegistrationEngine : public QObject
{
    Q_OBJECT

public:
    enum Message {
        UnknownMsg,
        // Сообщение об ошибке
        ErrorMsg,
        // Проверка доступности регистратора
        RegistrarEchoRequestMsg, RegistrarEchoReplyMsg,
        RegistrarAddrRequestMsg, RegistrarAddrReplyMsg, RegistrarAddrNotifyMsg,
        // Регистрация пользователя
        UserRegistrationRequestMsg, UserRegistrationReplyMsg, CheckRegistrationMsg,
        // Смена профиля (регистрации) / Сброс регистрации
        UserChangeProfileRequestMsg, UserChangeProfileReplyMsg, RegistrationRecordRemoveMsg,
        // Синхронизация данных (по сети)
        SyncRegistrationTableMsg, SyncRegistrationTableAllMsg,
        RequestRegistrationTableSyncMsg
    };

public:
    explicit ESURegistrationEngine(QObject *parent = 0);
    explicit ESURegistrationEngine(ESURegistration* base, QObject *parent = 0);
    ~ESURegistrationEngine();

    void setBase(ESURegistration* base);
//    void setUI(ESURegistrationUI* ui);

public:
    // [ PROPERTIES ]:
    ESURegistrationTableManager *tableManager() const;
    RegistrationUserInfo& registrationData();

    void setConfigurationFilePath(const QString& path);
public:
    // [ Opertation status ]:
    void acceptOperation();
    void rejectOperation();

public Q_SLOTS:
    // [ Configuration ]:
    void loadConfiguration(const QString&);
    void saveConfiguration();
    void clearConfiguration();
    bool loadRegistrarList();

public:
    // [ Messages ]:
    void sendSystemMsg(Message message, const QString& addr = QString());
    void sendSystemMsgWide(Message message, const QStringList& addrs);

    void sendUserMsg(const RegistrationPackageNET& p, const QString& addr = QString());
    void sendRegistrationMsg(const RegistrationPackageNET& p, const QString& addr = QString());

    void sendErrorMsg(ESURegistration::Error error, const QString& addr = QString());
    void sendErrorMsg(ESURegistration::Error error, const QStringList& addrs);

public:    
    // [ User methods ]:
    bool checkRegistrator();

    //! Запросить полную синхронизацию
    void requestRegistrationTableSync();

    void getRegistrationQueries(QHash<QString, RegistrationUserInfo> &);

    // Методы принятия или отклонения  регистратором запросов
    // [ Registrar methods ]:
    void processRegistrationRequest(bool accept, const QString& address);
    void processRegistrationRequests(bool accept);

    //! brief runRegistrationTableSync - Запустить полную синхронизацию
    void runRegistrationTableSync();

    // [ General methods ]:
    void registerCurrentData();

    //! clearRegistration - Сделать сброс регистрации
    void clearRegistration();

    //! registerProfile - регистрация сервера (регистратора) с занесением в базу данных и синхронизацией
    void registerProfile();

public Q_SLOTS:
    void runUserRegistration();
    void changeUserProfile();    
    void onTaskTimeout();

protected Q_SLOTS:
    void processUserRegistration();

public:
    // [ IListenerESU ]: {{{
    QObject*    toObject() { return(this); }
    quint32     ID() const { return( ESU_REGISTRATION_ID ); }
    QString     nameID() { return( QString(ESU_REGISTRATION_NAME_ID) ); }

public Q_SLOTS:
    void onReceivedData(RegistrationPackageNET p);

    void processErrorMessage(const RegistrationPackageNET& p);
    void processChangeProfile(Message msg, const RegistrationPackageNET& p);

    // }}} [ IListenerESU ]

Q_SIGNALS:
    void execUserRegistration();
    void execRegistrarTasks();

#ifndef ESU_NET_PROTO_SERVER
    void emitsendRegistrationMsg(RegistrationPackageNET, QStringList addrs=QStringList());
#endif

protected:
    ESURegistrationEnginePrivate* d;

private:
    Q_DECLARE_PRIVATE_D(ESURegistrationEngine::d, ESURegistrationEngine)
    Q_DISABLE_COPY(ESURegistrationEngine)

    Q_FRIEND(ESURegistration)
    Q_FRIEND(ESURegistrationPrivate)
};

#endif // ESU_REGISTRATION_ENGINE_H
