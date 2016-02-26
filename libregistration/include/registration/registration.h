#ifndef ESU_REGISTRATION_H
#define ESU_REGISTRATION_H

#include <QtCore>

#include "registration_data.h"
#include "registration_global.h"

#include <interfaces/i_singleton.h>

// TODO: Пока Синглтон-реализация.
#define esuReg ESURegistration::instance()

class ESURegistrationEngine;
class ESURegistrationPrivate;


class ESURegistration : public QObject,
        public ISingleton<ESURegistration>
{
    Q_OBJECT
    I_SINGLETON(ESURegistration)

    Q_PROPERTY(RegistrationState registrationState READ registrationState WRITE setRegistrationState NOTIFY registrationStateChanged)
    Q_PROPERTY(Profile profile READ profile WRITE setProfile NOTIFY profileChanged)
    Q_PROPERTY(bool isRegistered READ isRegistered NOTIFY isRegisteredChanged)
    Q_PROPERTY(bool isRegistrar READ isRegistrar NOTIFY isRegistrarChanged)
    Q_PROPERTY(bool isRegistrarMain READ isRegistrarMain NOTIFY isRegistrarMainChanged)
    Q_PROPERTY(OperationState operationState READ operationState WRITE setOperationState NOTIFY operationStateChanged)
    Q_PROPERTY(Task currentTask READ currentTask WRITE setCurrentTask NOTIFY currentTaskChanged)
    Q_PROPERTY(GeneralState state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(Error error READ error WRITE setError NOTIFY errorChanged)
    Q_PROPERTY(bool hasError READ hasError NOTIFY hasErrorChanged)
    Q_PROPERTY(QString currentMessage READ currentMessage WRITE setCurrentMessage NOTIFY currentMessageChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage WRITE setErrorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(QString currentAddress READ currentAddress WRITE setCurrentAddress NOTIFY currentAddressChanged)
    Q_PROPERTY(QString currentRegistrarAddress READ currentRegistrarAddress WRITE setCurrentRegistrarAddress NOTIFY currentRegistrarAddressChanged)
    Q_PROPERTY(int currentRegistrarIndex READ currentRegistrarIndex WRITE setCurrentRegistrarIndex NOTIFY currentRegistrarIndexChanged)
    Q_PROPERTY(int registrarCount READ registrarCount WRITE setRegistrarCount NOTIFY registrarCountChanged)
    Q_PROPERTY(int registrarCheckCount READ registrarCheckCount WRITE setRegistrarCheckCount NOTIFY registrarCheckCountChanged)

    // [ UI ]:
    Q_PROPERTY(QObject* currentUiForm READ currentUiForm WRITE setCurrentUiForm NOTIFY currentUiFormChanged)

    Q_PROPERTY(QString choosedRole READ choosedRole WRITE setChoosedRole NOTIFY choosedRoleChanged)
    Q_PROPERTY(QString choosedName READ choosedName WRITE setChoosedName NOTIFY choosedNameChanged)
    Q_PROPERTY(QString choosedDuty READ choosedDuty NOTIFY choosedDutyChanged)
    Q_PROPERTY(QString choosedSpecialty READ choosedSpecialty WRITE setChoosedSpecialty NOTIFY choosedSpecialtyChanged)
    Q_PROPERTY(int choosedRankIndex READ choosedRankIndex WRITE setChoosedRankIndex NOTIFY choosedRankIndexChanged)
    Q_PROPERTY(QString choosedBirthday READ choosedBirthday WRITE setChoosedBirthday NOTIFY choosedBirthdayChanged)
    Q_PROPERTY(QString choosedAddress READ choosedAddress WRITE setChoosedAddress NOTIFY choosedAddressChanged)

    // [ USER PROFILE ]:
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString userRole READ userRole WRITE setUserRole NOTIFY userRoleChanged)
    Q_PROPERTY(QString userDuty READ userDuty NOTIFY userDutyChanged)
    Q_PROPERTY(QString userRank READ userRank WRITE setUserRank NOTIFY userRankChanged)
    Q_PROPERTY(int userRankIndex READ userRankIndex WRITE setUserRankIndex NOTIFY userRankIndexChanged)
    Q_PROPERTY(QString userSpecialty READ userSpecialty WRITE setUserSpecialty NOTIFY userSpecialtyChanged)
    Q_PROPERTY(QString userBirthday READ userBirthday WRITE setUserBirthday NOTIFY userBirthdayChanged)
    Q_PROPERTY(QString userAddress READ userAddress WRITE setUserAddress NOTIFY userAddressChanged)
    Q_PROPERTY(QString registrationTime READ registrationTime WRITE setRegistrationTimeString NOTIFY registrationTimeChanged)


public:
    enum RegistrationState {
        RegisteredState, UnregisteredState, RegistrationInProcessState
    };

    enum Profile {
        UnknownProfile, UserProfile, RegistrarProfile, RegistrarMainProfile
    };

    enum OperationState {
        NoOperationState, OperationExecuteState, WaitState, OperationSuccess, OperationFailure
    };

    enum RequestType { UnknownRequest, RegistrationRequest, ChangeProfileRequest, UpdateProfileRequest };

    enum Task {
        NoTasks,
        // User Tasks
        RegistrationBeginTask, CheckRegistrarsTask, CheckRegistrarTask,
        SendRegistrationDataTask, ChangeProfileTask,
        // General Tasks
        CheckRegistrationTask,
        // Registrar Tasks
        ProcessUserRegistrationTask, ConfirmUserRegistrationTask
    };

    enum GeneralState {
        SteadyState, RequestRegistrationState, RegisterUserState,
        ChangeRegistrationState, UpdateRegistrationDataState
    };

    enum Error {
        NoErrors = 0,
        RegistrarsNotAvailableError,
        UserAlreadyRegisteredError, UserNotRegisteredError,
        RoleAlreadyOccupiedError,
        TimeoutExpiredError,
        RegistrationInProcessError,
        DataNotAvailableError
    };

    Q_ENUMS(RegistrationState Profile OperationState Task RequestType GeneralState Error)

protected:
    explicit ESURegistration(QObject *parent = nullptr);
    ~ESURegistration();

public:
    // [PROPERTIES NON-QML]:
    void setConfigurationFilePath(const QString& path);

public:
    // [PROPERTIES]:
    RegistrationState registrationState() const;
    void setRegistrationState(RegistrationState regState);

    Profile profile() const;
    void setProfile(Profile profile);

    bool isRegistered() const;
    bool isRegistrar() const;
    bool isRegistrarMain() const;

    OperationState operationState() const;
    void setOperationState(OperationState operationState);
    inline bool operationSuccessfull() { return( operationState() == OperationSuccess ); }
    inline bool operationFailure() { return( operationState() == OperationFailure); }

    Task currentTask() const;
    void setCurrentTask(Task task);

    // Поместить текущую задачу в стек и сделать <task> текущей задачей
    Q_INVOKABLE void pushTask(Task task);    
    // Восстановить задачу, которая находится в стеке. Задача становится текущей и возвращается.
    // Если стек пуст, возвращается [Task.NoTasks]
    Q_INVOKABLE Task popTask();
    Q_INVOKABLE bool isTaskStackEmpty();
    Q_INVOKABLE void clearTasks();
    Q_INVOKABLE void executeCurrentTask();    
    Q_INVOKABLE void setTaskTimeout();

    GeneralState state() const;
    void setState(GeneralState state);

    Error error() const;
    void setError(Error error);

    bool hasError() const;

    // Текущее соообщение(статус) регистрации для индикатора прогресса или системных сообщений
    QString currentMessage() const;
    void setCurrentMessage(const QString& message);

    // Сообщение об ошибке (текстовое предсьавление перечисления Error)
    QString errorMessage() const;
    void setErrorMessage(const QString& errorMessage);

    // Текущий регистрируемый адрес
    QString currentAddress() const;
    void setCurrentAddress(const QString& address);

    // Текущий адрес регистратора
    QString currentRegistrarAddress() const;
    void setCurrentRegistrarAddress(const QString& address);

    // Индекс (порядковый номер) регистратора (текущий)
    int currentRegistrarIndex() const;
    void setCurrentRegistrarIndex(int index);

    // Общее кол-во регистраторов (по списку)
    int registrarCount() const;
    void setRegistrarCount(int count);

    // Кол-во регистраторов по списку проверки
    int registrarCheckCount() const;
    void setRegistrarCheckCount(int count);

    // Управление UI интерфейсом
    QObject *currentUiForm() const;
    void setCurrentUiForm(QObject* form);

    QString choosedRole() const;
    void setChoosedRole(const QString& choosedRole);

    QString choosedName() const;
    void setChoosedName(const QString& setChoosedName);

    QString choosedDuty() const;

    QString choosedSpecialty() const;
    void setChoosedSpecialty(const QString& specialty);

    int choosedRankIndex() const;
    void setChoosedRankIndex(int index);

    QString choosedBirthday() const;
    void setChoosedBirthday(const QString& value);

    QString choosedAddress() const;
    void setChoosedAddress(const QString& address);

    RegistrationUserInfo& choosedData() const;

    // [ USER PROFILE ]:
    // ФИО пользователя (бойца)
    QString userName() const;
    void setUserName(const QString& name);

    // Профиль для регистрации
    QString userRole() const;
    void setUserRole(const QString& role);

    // Должность бойца
    QString userDuty() const;    

    // Звание
    QString userRank() const;
    void setUserRank(const QString& value);

    // Порядковый номер звания (для индексации)
    int userRankIndex() const;
    void setUserRankIndex(int index);

    // ВУС (Военно-учётная специальность)
    QString userSpecialty() const;
    void setUserSpecialty(const QString& specialty);

    // Дата рождения
    QString userBirthday() const;
    void setUserBirthday(const QString& value);

    // Адрес проживания бойца
    QString userAddress() const;
    void setUserAddress(const QString& value);

    // Время регистрации
    QString registrationTime() const;
    void setRegistrationTime(int value);
    void setRegistrationTimeString(const QString& timeString);
    Q_INVOKABLE void updateRegistrationTime();

    RegistrationUserInfo& registrationData() const;
    void setRegistrationData(const RegistrationUserInfo& data);
    void clearRegistrationData();

public:
    ESURegistrationEngine   *engine() const;
//    ESURegistrationUI       *ui() const;

public Q_SLOTS:
    // [ USER INTERFACE ]:
    /*!
     * \brief registerUser - Запуск процесса регистрации с клиента
     */
    void registerUser();
    /*!
     * \brief changeUser - Запуск процесса смена профиля с клиента
     */
    void changeUser();

    /*!
     * \brief cancelRegistration - Отмена процедуры регистрации
     */
    void cancelRegistration();
    /*!
     * \brief clearCurrentOperation - Сброс текущий задачи (останов). Сброс все состояний.
     */
    void clearCurrentOperation();

    /*!
     * \brief clearUserData - очистка данных регистрации (только при сбросе/смене профиля)
     */
    void clearUserData();
    /*!
     * \brief clearChoosedData - очистка данных полей/свойств (QML interface)
     */
    void clearChoosedData();
    /*!
     * \brief initChoosedData - инициализация свойств и/или полей из
     *  существующих данных о регистрации (QML interface)
     */
    void initChoosedData();

    void acceptRegistrationData();
    void saveConfiguration();

    // [ REGISTRAR INTERFACE ]:
    /*!
     * \brief registerProfile - зарегистрировать планшет регистратора (сервера)
     */
    void registerProfile();

    void acceptRequest(const QString& address);
    void rejectRequest(const QString& address);

    void acceptAllRequests();
    void rejectAllRequests();

    void acceptRegistration(const QString& address);
    void acceptRegistrationAll();

    void rejectRegistration(const QString& address);
    void rejectRegistrationAll();

Q_SIGNALS:
    // [ PROPERTIES ]:
    void registrationStateChanged();
    void profileChanged();
    void isRegisteredChanged();
    void isRegistrarChanged();
    void isRegistrarMainChanged();
    void operationStateChanged();
    void currentTaskChanged();
    void stateChanged();
    void errorChanged();
    void hasErrorChanged();
    void currentMessageChanged();
    void errorMessageChanged();
    void currentAddressChanged();
    void currentRegistrarAddressChanged();
    void currentRegistrarIndexChanged();
    void registrarCountChanged();
    void registrarCheckCountChanged();

    void currentUiFormChanged();

    void choosedRoleChanged();
    void choosedNameChanged();
    void choosedDutyChanged();
    void choosedSpecialtyChanged();
    void choosedRankIndexChanged();
    void choosedBirthdayChanged();
    void choosedAddressChanged();

    void userNameChanged();
    void userRoleChanged();
    void userDutyChanged();
    void userRankChanged();
    void userRankIndexChanged();
    void userSpecialtyChanged();

    void userBirthdayChanged();
    void userAddressChanged();
    void registrationTimeChanged();

    // [ EVENTS ]:
    void errorEvent(Error error);
    void showRegistrarFormEvent();

    // [ GENERAL ]:
    void registrationTableUpdated();

public Q_SLOTS:
    void initialization();

    void onClearSettings();
    void onClearDatabase();

protected:
    ESURegistrationPrivate* d;

private:
    Q_DECLARE_PRIVATE_D(ESURegistration::d, ESURegistration)
    Q_DISABLE_COPY(ESURegistration)
    Q_FRIEND(ESURegistrationEngine)
};

Q_DECLARE_METATYPE(ESURegistration::RegistrationState)
Q_DECLARE_METATYPE(ESURegistration::Profile)
Q_DECLARE_METATYPE(ESURegistration::OperationState)
Q_DECLARE_METATYPE(ESURegistration::Task)
Q_DECLARE_METATYPE(ESURegistration::GeneralState)
Q_DECLARE_METATYPE(ESURegistration::Error)

#endif // ESU_REGISTRATION_H
