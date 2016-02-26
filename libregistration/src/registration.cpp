#include "registration.h"
#include "registration_p.h"

#include <staff/staff_globals.h>

#include <QDateTime>


// [ INIT/DESTROY ]: {{{

ESURegistration::ESURegistration(QObject *parent) : QObject(parent),
    d(new ESURegistrationPrivate(this))
{
    d->init();
    d->engine.setBase(this);

    //QTimer::singleShot( 3000, this, SLOT(initialization()) );
}


ESURegistration::~ESURegistration()
{
    delete d;
}

// }}} [ INIT/DESTROY ]
// [PROPERTIES NON-QML]: {{{

void ESURegistration::setConfigurationFilePath(const QString &path)
{
    d->engine.setConfigurationFilePath( path );
}

// }}}
// [PROPERTIES]: {{{

// [ REGISTRATION STATE ]:

ESURegistration::RegistrationState ESURegistration::registrationState() const
{
    return( d->registrationState );
}

void ESURegistration::setRegistrationState(ESURegistration::RegistrationState regState)
{
    if( d->registrationState != regState ) {
        d->registrationState = regState;
        Q_EMIT registrationStateChanged();
        Q_EMIT isRegisteredChanged();
    }
}

// [ PROFILE ]:

ESURegistration::Profile ESURegistration::profile() const
{
    return( d->profile );
}

void ESURegistration::setProfile(ESURegistration::Profile profile)
{
    if( d->profile != profile ) {
        d->profile = profile;

        Q_EMIT profileChanged();
        Q_EMIT isRegistrarChanged();
        Q_EMIT isRegistrarMainChanged();
    }
}


bool ESURegistration::isRegistered() const
{
    return( d->registrationState == RegisteredState );
}

bool ESURegistration::isRegistrar() const
{
    return( d->profile == RegistrarProfile ||
            d->profile == RegistrarMainProfile );
}

bool ESURegistration::isRegistrarMain() const
{
    return( d->profile == RegistrarMainProfile );
}

// [ OPERATION STATE ]:

ESURegistration::OperationState ESURegistration::operationState() const
{
    return( d->operationState );
}

void ESURegistration::setOperationState(ESURegistration::OperationState operationState)
{
    if( d->operationState != operationState ) {
        d->operationState = operationState;
        Q_EMIT operationStateChanged();
    }
}

// [ CURRENT TASK ]:

ESURegistration::Task ESURegistration::currentTask() const
{
    return( d->currentTask );
}

void ESURegistration::setCurrentTask(ESURegistration::Task task)
{
    d->currentTask = task;
    Q_EMIT currentTaskChanged();
}


void ESURegistration::pushTask(ESURegistration::Task task)
{
    d->taskStack.push(task);
    d->currentTask = task;
    Q_EMIT currentTaskChanged();
}

ESURegistration::Task ESURegistration::popTask()
{
    if( !d->taskStack.isEmpty() ) {
        d->currentTask = d->taskStack.pop();
        Q_EMIT currentTaskChanged();

        return( d->currentTask );
    }

    return( NoTasks );
}

bool ESURegistration::isTaskStackEmpty()
{
    return( d->taskStack.isEmpty() );
}

void ESURegistration::clearTasks()
{
    if( !d->taskStack.isEmpty() )
        d->taskStack.clear();
}

void ESURegistration::executeCurrentTask()
{
    if( d->currentTask == NoTasks ) {
        if( !d->taskStack.isEmpty() )
            d->currentTask = d->taskStack.pop();
        else return;
    }

    QMetaObject::invokeMethod( &d->engine, "executeCurrentTask", Qt::QueuedConnection );
}

void ESURegistration::setTaskTimeout()
{
    d->engine.onTaskTimeout();
}

// [ STATE ]:

ESURegistration::GeneralState ESURegistration::state() const
{
    return( d->state );
}

void ESURegistration::setState(GeneralState state)
{
    if( d->state != state ) {
        d->state = state;
        Q_EMIT stateChanged();
    }
}

// [ ERROR ]:

ESURegistration::Error ESURegistration::error() const
{
    return( d->error );
}

void ESURegistration::setError(ESURegistration::Error error)
{
    if( d->error != error ) {
        d->error = error;
        Q_EMIT errorChanged();
        Q_EMIT hasErrorChanged();
        Q_EMIT errorEvent(error);
    }
}

bool ESURegistration::hasError() const
{
    return( d->error != NoErrors );
}

// [ CURRENT MESSAGE ]:

QString ESURegistration::currentMessage() const
{
    return( d->currentMessage );
}

void ESURegistration::setCurrentMessage(const QString &message)
{
    if( d->currentMessage != message ) {
        d->currentMessage = message;
        Q_EMIT currentMessageChanged();
    }
}

// [ ERROR MESSAGE ]:

QString ESURegistration::errorMessage() const
{
    return( d->errorMessage );
}

void ESURegistration::setErrorMessage(const QString &errorMessage)
{
    if( d->errorMessage != errorMessage ) {
        d->errorMessage = errorMessage;
        Q_EMIT errorMessageChanged();
    }
}

// [ CURRENT ADDRESS ]:

QString ESURegistration::currentAddress() const
{
    return( d->currentAddress );
}

void ESURegistration::setCurrentAddress(const QString &address)
{
    if( d->currentAddress != address ) {
        d->currentAddress = address;
        Q_EMIT currentAddressChanged();
    }
}


QString ESURegistration::currentRegistrarAddress() const
{
    return( d->currentRegistrarAddress );
}

void ESURegistration::setCurrentRegistrarAddress(const QString &address)
{
    d->currentRegistrarAddress = address;
    Q_EMIT currentRegistrarAddressChanged();
}


int ESURegistration::currentRegistrarIndex() const
{
    return( d->currentRegistrarIndex );
}

void ESURegistration::setCurrentRegistrarIndex(int index)
{
    d->currentRegistrarIndex = index;
    Q_EMIT currentRegistrarIndexChanged();
}


int ESURegistration::registrarCount() const
{
    return( d->registrarCount );
}

void ESURegistration::setRegistrarCount(int count)
{
    d->registrarCount = count;
    Q_EMIT registrarCountChanged();
}


int ESURegistration::registrarCheckCount() const
{
    return( d->registrarCheckCount );
}

void ESURegistration::setRegistrarCheckCount(int count)
{
    d->registrarCheckCount = count;
    Q_EMIT registrarCheckCountChanged();
}

// [ CURRENT UI FORM ]:

QObject *ESURegistration::currentUiForm() const
{
    return( d->currentUiForm );
}

void ESURegistration::setCurrentUiForm(QObject *form)
{
    if( d->currentUiForm != form && form != nullptr ) {
        d->currentUiForm = form;
        Q_EMIT currentUiFormChanged();
    }
}

// [ MENU CHOICE ]:

QString ESURegistration::choosedRole() const
{
    return( d->choosedData.role );
}

void ESURegistration::setChoosedRole(const QString &choosedRole)
{
    if( d->choosedData.role != choosedRole ) {
        d->choosedData.role = choosedRole;
        d->choosedData.dutyName = staff::getDutyDescrByName(choosedRole.split('.').first().toUpper());
        //d->dataInfo.duty = staff::getUnitDutyByDescr(d->dataInfo.dutyName);

        Q_EMIT choosedRoleChanged();
        Q_EMIT choosedDutyChanged();
    }
}


QString ESURegistration::choosedName() const
{
    return( d->choosedData.name );
}

void ESURegistration::setChoosedName(const QString &choosedName)
{
    if( d->choosedData.name != choosedName ) {
        d->choosedData.name = choosedName;
        Q_EMIT choosedNameChanged();
    }
}


QString ESURegistration::choosedDuty() const
{
    return( d->choosedData.dutyName );
}


QString ESURegistration::choosedSpecialty() const
{
    return( d->choosedData.militarySpeciality );
}

void ESURegistration::setChoosedSpecialty(const QString &specialty)
{
    if( d->choosedData.militarySpeciality != specialty ) {
        d->choosedData.militarySpeciality = specialty;
        Q_EMIT choosedSpecialtyChanged();
    }
}


int ESURegistration::choosedRankIndex() const
{
    return( d->choosedData.rank );
}

void ESURegistration::setChoosedRankIndex(int index)
{
    if( d->choosedData.rank != index ) {
        d->choosedData.rank = index;
        Q_EMIT choosedRankIndexChanged();
    }
}


QString ESURegistration::choosedBirthday() const
{
    return( d->choosedData.birthDateString() );
}

void ESURegistration::setChoosedBirthday(const QString &value)
{
    if( d->choosedData.birthDateString() != value ) {
        d->choosedData.birthDate = QDateTime::fromString(value, "dd.MM.yyyy").toTime_t();
        Q_EMIT choosedBirthdayChanged();
    }
}


QString ESURegistration::choosedAddress() const
{
    return( d->choosedData.streetAddress );
}

void ESURegistration::setChoosedAddress(const QString &address)
{
    if( d->choosedData.streetAddress != address ) {
        d->choosedData.streetAddress = address;
        Q_EMIT choosedAddressChanged();
    }
}


RegistrationUserInfo &ESURegistration::choosedData() const
{
    return( d->choosedData );
}

// }}}
// [ USER PROPERTIES ]:

QString ESURegistration::userName() const
{
    return( d->dataInfo.name );
}

void ESURegistration::setUserName(const QString &name)
{
    d->dataInfo.name = name;
    Q_EMIT userNameChanged();
}


QString ESURegistration::userRole() const
{
    return( d->dataInfo.role );
}

void ESURegistration::setUserRole(const QString &role)
{
    if( !role.isEmpty() ) {
        d->dataInfo.role = role;
        d->dataInfo.dutyName = staff::getDutyDescrByName(role.split('.').first().toUpper());
        d->dataInfo.duty = staff::getUnitDutyByDescr(d->dataInfo.dutyName);

        Q_EMIT userRoleChanged();
        Q_EMIT userDutyChanged();
    }
}


QString ESURegistration::userDuty() const
{
    return( d->dataInfo.dutyName );
}


QString ESURegistration::userRank() const
{
    return( d->dataInfo.rankName );
}

void ESURegistration::setUserRank(const QString &value)
{
    staff::UnitRank rank = staff::getUnitRankByDescr(value);
    if( rank == staff::UnknownRank ) return;

    d->dataInfo.rank = rank;
    d->dataInfo.rankName = value;

    Q_EMIT userRankChanged();
    Q_EMIT userRankIndexChanged();
}


int ESURegistration::userRankIndex() const
{
    return( d->dataInfo.rank );
}

void ESURegistration::setUserRankIndex(int index)
{
    if( d->dataInfo.rank != index ) {
        QString val = staff::getRankDescription(static_cast<staff::UnitRank>(index));
        if( val.isEmpty() ) return;

        d->dataInfo.rank = index;
        d->dataInfo.rankName = val;

        Q_EMIT userRankChanged();
        Q_EMIT userRankIndexChanged();
    }
}


QString ESURegistration::userSpecialty() const
{
    return( d->dataInfo.militarySpeciality );
}

void ESURegistration::setUserSpecialty(const QString &specialty)
{
    if( d->dataInfo.militarySpeciality != specialty ) {
        d->dataInfo.militarySpeciality = specialty;
        Q_EMIT userSpecialtyChanged();
    }
}


QString ESURegistration::userBirthday() const
{
    if( d->dataInfo.birthDate <= 0 ) return(QString(""));
    return( QDateTime::fromTime_t(d->dataInfo.birthDate).toString("dd.MM.yyyy") );
}

void ESURegistration::setUserBirthday(const QString &value)
{
    QDateTime birthday;
    birthday.setDate(QDate::fromString(value, "dd.MM.yyyy"));
    d->dataInfo.birthDate = birthday.toTime_t();

    Q_EMIT userBirthdayChanged();
}


QString ESURegistration::userAddress() const
{
    return(d->dataInfo.streetAddress);
}

void ESURegistration::setUserAddress(const QString &value)
{
    d->dataInfo.streetAddress = value;

    Q_EMIT userAddressChanged();
}


QString ESURegistration::registrationTime() const
{
    if( d->dataInfo.time <= 0  ) return( QString() );
    else if( d->dataInfo.timeString.isEmpty() ) d->dataInfo.updateTimeString();
    return( d->dataInfo.timeString ); // FIXIT
}

void ESURegistration::setRegistrationTime(int value)
{
    d->dataInfo.setTime(value);
    Q_EMIT registrationTimeChanged();
}

void ESURegistration::setRegistrationTimeString(const QString &timeString)
{
    QDateTime time = QDateTime::fromString(timeString, "dd.MM.yyyy hh:mm");
    if( time.isNull() || !time.isValid() ) return;
    d->dataInfo.setTime(time.toTime_t());

    Q_EMIT registrationTimeChanged();
}

void ESURegistration::updateRegistrationTime()
{
    d->dataInfo.updateTime();
    Q_EMIT registrationTimeChanged();
}


RegistrationUserInfo &ESURegistration::registrationData() const
{
    return( d->dataInfo );
}

void ESURegistration::setRegistrationData(const RegistrationUserInfo &data)
{
    d->dataInfo = data;
    setUserRole(data.role);
    setUserRankIndex(data.rank);
    d->dataInfo.updateTimeString();

    Q_EMIT userNameChanged();
    Q_EMIT userSpecialtyChanged();
    Q_EMIT userBirthdayChanged();
    Q_EMIT userAddressChanged();
    Q_EMIT registrationTimeChanged();
}


void ESURegistration::clearRegistrationData()
{
    d->dataInfo.clear();

    Q_EMIT userNameChanged();
    Q_EMIT userRoleChanged();
    Q_EMIT userDutyChanged();
    Q_EMIT userRankChanged();
    Q_EMIT userRankIndexChanged();
    Q_EMIT userSpecialtyChanged();

    Q_EMIT userBirthdayChanged();
    Q_EMIT userAddressChanged();
    Q_EMIT registrationTimeChanged();
}

// [ ENGINE ]:

ESURegistrationEngine *ESURegistration::engine() const
{
    return(&(d->engine));
}

//ESURegistrationUI *ESURegistration::ui() const
//{
//    return( d->ui );
//}

// }}} [PROPERTIES]
// [ BASE INTERFACE/QML ]: {{{

// [ USER INTERFACE [:

void ESURegistration::registerUser()
{
    d->engine.runUserRegistration();
}


void ESURegistration::changeUser()
{
    d->engine.changeUserProfile();
}


void ESURegistration::cancelRegistration()
{
    setCurrentMessage("Регистрация прервана");
    clearCurrentOperation();
}


void ESURegistration::clearCurrentOperation()
{
    setState(SteadyState);
    setOperationState(NoOperationState);
    setError(NoErrors);    
}


void ESURegistration::clearUserData()
{
    d->dataInfo.clear();

    Q_EMIT userNameChanged();
    Q_EMIT userRoleChanged();
    Q_EMIT userDutyChanged();
    Q_EMIT userRankChanged();
    Q_EMIT userRankIndexChanged();
    Q_EMIT userSpecialtyChanged();
    Q_EMIT userBirthdayChanged();
    Q_EMIT userAddressChanged();
    Q_EMIT registrationTimeChanged();
}


void ESURegistration::clearChoosedData()
{
    d->choosedData.clear();

    Q_EMIT choosedNameChanged();
    Q_EMIT choosedRoleChanged();
    Q_EMIT choosedAddressChanged();
    Q_EMIT choosedBirthdayChanged();
    Q_EMIT choosedDutyChanged();
    Q_EMIT choosedRankIndexChanged();
    Q_EMIT choosedSpecialtyChanged();
}


void ESURegistration::acceptRegistrationData()
{
    d->engine.registerCurrentData();
}


void ESURegistration::saveConfiguration()
{
    d->engine.saveConfiguration();
}


void ESURegistration::registerProfile()
{
    d->engine.registerProfile();
}


// [ REGISTRAR INTERFACE ]:

void ESURegistration::acceptRequest(const QString &address)
{
    d->engine.processRegistrationRequest(true, address);
}

void ESURegistration::rejectRequest(const QString &address)
{
    d->engine.processRegistrationRequest(false, address);
}

void ESURegistration::acceptAllRequests()
{
    d->engine.processRegistrationRequests(true);
}

void ESURegistration::rejectAllRequests()
{
    d->engine.processRegistrationRequests(false);
}


void ESURegistration::acceptRegistration(const QString &address)
{
    d->engine.processRegistrationRequest(true, address);
}

void ESURegistration::acceptRegistrationAll()
{
    d->engine.processRegistrationRequests(true);
}


void ESURegistration::rejectRegistration(const QString &address)
{
    d->engine.processRegistrationRequest(false, address);
}

void ESURegistration::rejectRegistrationAll()
{
    d->engine.processRegistrationRequests(false);
}

// }}} [ BASE INTERFACE/QML ]
// [ Q_EVENTS ]: {{{

void ESURegistration::initialization()
{
    d->engine.loadConfiguration();
}


void ESURegistration::onClearSettings()
{
    clearUserData();
}


void ESURegistration::onClearDatabase()
{
    d->engine.tableManager()->clearData();
    Q_EMIT registrationTableUpdated();
}

// }}} [ Q_EVENTS ]
