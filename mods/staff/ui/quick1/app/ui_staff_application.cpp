#include "ui_staff_application.h"
#include "ui_staff_application_p.h"


// [ INIT/DESTROY ]: {{{

UIStaffApplication::UIStaffApplication(QObject *parent):
    QObject(parent)
  , d(new UIStaffApplicationPrivate(this))
{
    esuApp->addListener(this);
}


UIStaffApplication::~UIStaffApplication()
{

}

// }}} [ INIT/DESTROY ]
// [ BASE METHODS ]: {{{

void UIStaffApplication::registerQmlProperties(QDeclarativeContext *context)
{
    static bool registered = false;

    if( registered ) return;
    d->context = context;

    d->context->setContextProperty("staffAppUI", this);
    d->context->setContextProperty("staffApp", esuStaff);
    d->context->setContextProperty("staffModel", &d->staffModel);
    d->context->setContextProperty("staffStructModel", &d->structModel);
    d->context->setContextProperty("staffPathModel", &d->staffPathModel);
    d->context->setContextProperty("staffStructPathModel", &d->structPathModel);

    registered = true;
}


void UIStaffApplication::declareQmlTypes()
{
    static bool declared = false;

    if( declared ) return;

    qmlRegisterUncreatableType<UIStaffApplication>(
                "MKBKompas.CSR.ESUTZ.Staff", 1, 0, "ESUStaffUI",
                "Can\'t create ESURegistration instance!");

    declared = true;
}


void UIStaffApplication::loadQmlModels()
{
    if( staffESU->create() ) {
        d->structModel.initData();
        //d->staffModel.setModelData(d->structModel.modelRootElement());  // TEMP

        d->structPathModel.add(d->structModel.rootElementName(), d->structModel.rootElementNameFull());
        //d->staffPathModel.add(d->staffModel.rootElementName(), d->staffModel.rootElementNameFull());
    }
}

// }}} [ BASE METHODS ]
// [ PROPERTIES ]: {{{

QDeclarativeItem *UIStaffApplication::uiStaffForm() const
{
    return( d->uiStaffForm );
}

void UIStaffApplication::setUiStaffForm(QDeclarativeItem *uiStaffForm)
{
    if( d->uiStaffForm != uiStaffForm ) {
        d->uiStaffForm = uiStaffForm;
        Q_EMIT uiStaffFormChanged();
    }
}


QDeclarativeItem *UIStaffApplication::uiStructureForm() const
{
    return( d->uiStructureForm );
}

void UIStaffApplication::setUiStructureForm(QDeclarativeItem *uiStructureForm)
{
    if( d->uiStructureForm != uiStructureForm ) {
        d->uiStructureForm = uiStructureForm;
        Q_EMIT uiStructureFormChanged();
    }
}


int UIStaffApplication::mode() const
{
    return( (int)d->mode );
}

void UIStaffApplication::setMode(int mode)
{
    if( mode != (int)d->mode ) {
        d->mode = static_cast<Mode>( mode );

        switch(d->mode)
        {
        case ViewMode:
            setCaption("СТРУКТУРА");
            break;
        case ChooseProfileMode:
            setCaption("ВЫБОР ПРОФИЛЯ");
            break;
        default: break;
        }

        Q_EMIT modeChanged();
    }
}


QString UIStaffApplication::caption() const
{
    return( d->caption );
}

void UIStaffApplication::setCaption(const QString &caption)
{
    if( !caption.isEmpty() ) {
        d->caption = caption;
        Q_EMIT captionChanged();
    }
}

// }}} [ PROPERTIES ]
// [ IEventListener ]: {{{

quint32 UIStaffApplication::ID() const
{
    return( ESU_STAFF_UI_ID );
}


bool UIStaffApplication::isObject() const
{
    return(true);
}

// }}} [ IEventListener ]
// [ UI METHODS ]: {{{

void UIStaffApplication::registerProfile(const QString& name)
{
    QByteArray updateData;

    if( !esuStaff->updateStaff(name, d->registrationAddress, &updateData) )
        return;

    d->staffModel.updateModel(esuStaff->staff());

    qDebug() << QString(updateData);
    ESURegistrationEvent* e = new ESURegistrationEvent(ESURegistrationEvent::RegistrationSuccessfull,
                                                       d->registrationAddress, updateData, name);
    esuApp->sendEvent(ESU_REGISTRATION_ID, e, ESU_STAFF_UI_ID, ESU::QtMetaInvokeDispatch);
}

// }}} [ UI METHODS ]
// [ Q_EVENTS ]: {{{

void UIStaffApplication::customEvent(QEvent *e)
{
    ESURegistrationEvent* event = static_cast<ESURegistrationEvent*>(e);

    if( event == nullptr || event->type() != (QEvent::Type)ESUEvent::Registration ) {
        e->ignore();
        QObject::customEvent(e);
        return;
    }

    switch(event->eventType())
    {
    case ESURegistrationEvent::RegisterUser:
        e->accept();
        //if( d->uiStructureForm == nullptr ) break;

        d->registrationAddress = event->address();
        setMode((int)ChooseProfileMode);
        esuWinManager->showWindowName("StructureForm");

        //QMetaObject::invokeMethod(d->uiStructureForm, "modeChanged", Qt::QueuedConnection);
        break;
    case ESURegistrationEvent::UpdateRegistrationData:
    {
        esuStaff->updateStaff(event->data());
        d->staffModel.updateModel(esuStaff->staff());
        break;
    }
    default:
        e->ignore();
        break;
    }

    QObject::customEvent(e);
}

// }}} [ Q_EVENTS ]
