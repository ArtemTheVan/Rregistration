#include "ui_registration_application.h"
#include "ui_registration_application_p.h"



UIRegistrationApplication::UIRegistrationApplication(QObject *parent):
    QObject(parent)
  , d(new UIRegistrationApplicationPrivate(this))
{

}


UIRegistrationApplication::~UIRegistrationApplication()
{
    delete d;
}


// [ BASE METHODS ]: {{{

void UIRegistrationApplication::registerQmlProperties(QDeclarativeContext *context)
{
    static bool registered = false;

    if( registered ) return;
    d->context = context;

    d->context->setContextProperty("regAppUI", this);
    if( d->registration != nullptr )
        d->context->setContextProperty("regApp", d->registration);

    registered = true;
}

// [STATIC]

void UIRegistrationApplication::declareQmlTypes()
{
    static bool declared = false;

    if( declared ) return;

    qmlRegisterUncreatableType<ESURegistration>(
                "MKBKompas.CSR.ESUTZ.Registration", 1, 0, "ESURegistration", "Can\'t create ESURegistration instance!");
    qmlRegisterUncreatableType<UIRegistrationApplication>(
                "MKBKompas.CSR.ESUTZ.Registration", 1, 0, "ESURegistrationUI", "Can\'t create ESURegistration instance!");
    declared = true;
}

// }}} [ BASE METHODS ]
// [ PROPERTIES ]: {{{

QString UIRegistrationApplication::currentMessage() const
{
    return( d->currentMessage );
}


void UIRegistrationApplication::setCurrentMessage(const QString &message)
{
    if( d->currentMessage != message ) {
        d->currentMessage = message;
        Q_EMIT currentMessageChanged();
    }
}


bool UIRegistrationApplication::progress() const
{
    return(d->progress);
}

void UIRegistrationApplication::setProgress(bool progress)
{
    if( d->progress != progress ) {
        d->progress = progress;
        Q_EMIT progressChanged();
    }
}


int UIRegistrationApplication::mode() const
{
    return((int)d->mode);
}

void UIRegistrationApplication::setMode(int mode)
{
    ESURegistration::Mode m = static_cast<ESURegistration::Mode>(mode);
    if( d->mode != m ) {
        d->mode = m;
        Q_EMIT modeChanged();
    }
}


QDeclarativeItem *UIRegistrationApplication::uiForm() const
{
    return( d->uiForm );
}

void UIRegistrationApplication::setUiForm(QDeclarativeItem *uiForm)
{
    if( d->uiForm != uiForm ) {
        d->uiForm = uiForm;
        Q_EMIT uiFormChanged();
    }
}


void UIRegistrationApplication::setRegistration(ESURegistration *regPtr)
{
    Q_CHECK_PTR(regPtr);

    d->registration = regPtr;
    connect( d->registration, &ESURegistration::stateChangedNotify,
             this, &UIRegistrationApplication::onStateChanged, Qt::QueuedConnection );
}

// }}} [ PROPERTIES ]
// [ IEventListener ]: {{{

quint32 UIRegistrationApplication::ID() const
{
    return(ESU_REGISTRATION_UI_ID);
}

// [ Q_SLOTS ]:

void UIRegistrationApplication::onProcessAppMessage(quint32 senderID, int msg)
{
    Q_UNUSED(senderID);
    Q_UNUSED(msg);

    if( d->uiForm == nullptr ) return;
}

// }}} [ IEventListener ]
// [ UI METHODS/SLOTS ]: {{{

void UIRegistrationApplication::requestRegistration()
{
    Q_CHECK_PTR(d->registration);

    d->registration->requestRegistration();
}

// }}} [ UI METHODS/SLOTS ]
// [ Q_EVENTS ]: {{{

void UIRegistrationApplication::customEvent(QEvent *event)
{
    ESURegistrationEvent* e = static_cast<ESURegistrationEvent*>(event);

    if( !e || event->type() != (QEvent::Type)ESUEvent::Registration ) {
        event->ignore();
        QObject::customEvent(event);
        return;
    }

    switch(e->eventType())
    {
    case ESURegistrationEvent::UserAlreadyRegistered:
    {
        //QString msg = QString("");
        //setCurrentMessage();
        break;
    }
    default: break;
    }

    QObject::customEvent(event);
}

// }}} [ Q_EVENTS ]
// [ PRIVATE Q_SLOTS ]: {{{

void UIRegistrationApplication::onStateChanged(int state)
{
    switch(static_cast<ESURegistration::State>(state))
    {
    case ESURegistration::Unregistered:
        setProgress(false);
        break;
    case ESURegistration::Registered:
        setProgress(false);
        break;
    case ESURegistration::UserRegistrationRequired:
    {
        // Show UI
        setProgress(true);
        //esuWinManager->showWindowName("RegistrationForm");
        if( d->uiForm == nullptr ) break;
        QMetaObject::invokeMethod(d->uiForm, "stateChanged", Qt::QueuedConnection,
                                  Q_ARG(QVariant, QVariant(d->registration->state())) );
        break;
    }
    case ESURegistration::RegistrationInProcess: // [1] Начало регистрации
    {
        setProgress(true);
        setCurrentMessage("Регистрация началась");        
        QMetaObject::invokeMethod(d->uiForm, "stateChanged", Qt::QueuedConnection,
                                  Q_ARG(QVariant, QVariant(d->registration->state())) );
        break;
    }
    case ESURegistration::CheckRegistrar: // [1.1]
        setCurrentMessage("Установка связи с регистратором");
        break;
    case ESURegistration::CheckRegistrarSuccess: // [1.2]
        setCurrentMessage("Регистратор доступен");
        break;
    case ESURegistration::CheckRegistrarFailure: // [1.3]
        setCurrentMessage("Регистратор недоступен.");
        break;
    case ESURegistration::RequestRegistrar: // [2] Поиск регистратора
        setCurrentMessage("Поиск регистратора");
        break;
    case ESURegistration::BecomesRegistrar: // [2.1] Становимся новым регистратором
    {
        setCurrentMessage("Регистратор не найден.\nСтановимся новым регистратором");        
        QMetaObject::invokeMethod(d->uiForm, "stateChanged", Qt::QueuedConnection,
                                  Q_ARG(QVariant, QVariant(d->registration->state())) );
        break;
    }
    case ESURegistration::RegistrarAddrReceived: // [2.2] Получен адрес регистратора
        setCurrentMessage("Регистратор обнаружен");
        break;
    case ESURegistration::RegistrationRequest: // [3] Отправка запроса на регистрацию
        setCurrentMessage("Отправка запроса на регистрацию");
        break;
    case ESURegistration::WaitState: // [3.1] Запрос находится в очереди на регистрацию
        setCurrentMessage("Ожидание очереди на регистрацию");
        break;
    case ESURegistration::RegistrationBegining: // [4] Регистрация началась
        setCurrentMessage("Регистрация началась");
        break;
    case ESURegistration::RegistrationSuccessfull: // [4.1] Пользователь зарегестрирован
        setCurrentMessage("Планшет зарегестрирован");
        QMetaObject::invokeMethod(d->uiForm, "stateChanged", Qt::QueuedConnection,
                                  Q_ARG(QVariant, QVariant(d->registration->state())) );
        break;
    case ESURegistration::RegistrationCanceled: // [4.2] Регистрация отменена
        setCurrentMessage("Регистрация отклонена");
        QMetaObject::invokeMethod(d->uiForm, "stateChanged", Qt::QueuedConnection,
                                  Q_ARG(QVariant, QVariant(d->registration->state())) );
        break;
    case ESURegistration::RegistrarProfileDetected:

        break;
    case ESURegistration::UserProfileDetected:

        break;                
    case ESURegistration::RegistrationError: // [6] Ошибка регистрации
        switch (d->registration->error()) {
        case ESURegistration::UserAlreadyRegisteredError:
            setCurrentMessage(QString("Ошибка регистрации: %1").arg(d->registration->errorMessage()));
            break;
        case ESURegistration::TimeoutExpiredError:
            setCurrentMessage(QString("Ошибка регистрации: %1").arg(d->registration->errorMessage()));
            break;
        case ESURegistration::UnknownError:
        case ESURegistration::NoErrors:
        default:
            break;
        }

        QMetaObject::invokeMethod(d->uiForm, "stateChanged", Qt::QueuedConnection,
                                  Q_ARG(QVariant, QVariant(d->registration->state())) );
        break;
    case ESURegistration::UnknownState:
    default: break;
    }
}


void UIRegistrationApplication::onNotifySend(QString msg)
{
    Q_UNUSED(msg);
}


void UIRegistrationApplication::onNotifyAccepted()
{

}

// }}} [ PRIVATE Q_SLOTS ]
