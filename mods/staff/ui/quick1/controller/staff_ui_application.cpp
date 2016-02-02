#include "staff_ui_application.h"

#include "esu_military_staff.h"

#include "staff_ui_model.h"
#include "staff_structure_ui_model.h"
#include "staff_ui_path_model.h"

#include "private/military_staff_globals_p.h"


// [ Class ]:[ StaffUIApplicationPrivate ]: {{{

class StaffUIApplicationPrivate
{
public:
    StaffUIApplicationPrivate():
        checkMode(false), context(nullptr)
      , viewModeUI(MilitaryStaffGlobals::StaffUIMode)
      , userAction(MilitaryStaffGlobals::UnknownAction)
    { }

    ~StaffUIApplicationPrivate()
    { }

public:
    bool checkMode;
    QDeclarativeContext* context;

    MilitaryStaffGlobals::ViewMode viewModeUI;
    MilitaryStaffGlobals::UserAction userAction;

    QString address;
    QString formHeader;

    StaffUIModel            staffModel;
    StaffStructureUIModel   structModel;
    StaffUIPathModel        staffPathModel;
    StaffUIPathModel        structPathModel;
};

// }}} [ Class ]:[ StaffUIApplicationPrivate ]
// [ INIT/DESTROY ]: {{{

StaffUIApplication::StaffUIApplication(QObject *parent) :
    QObject(parent), d(new StaffUIApplicationPrivate())
{

}


StaffUIApplication::~StaffUIApplication()
{
    delete d;
}


void StaffUIApplication::declareQML()
{
    static bool declared = false;

    if( !declared )
    {
        MilitaryStaffGlobals::declareQML();
    }

    declared = true;
}


void StaffUIApplication::initData()
{
    if( staffESU->create() ) {
        d->structModel.initData();
        //d->staffModel.setModelData(d->structModel.modelRootElement());  // TEMP

        d->structPathModel.add(d->structModel.rootElementName(), d->structModel.rootElementNameFull());
        //d->staffPathModel.add(d->staffModel.rootElementName(), d->staffModel.rootElementNameFull());
    }
}


void StaffUIApplication::declareQmlModels(QDeclarativeContext *context)
{
    Q_CHECK_PTR(context);

    static bool declared = false;

    if( !declared )
    {
        d->context = context;

//        d->context->setContextProperty("staffAppUI", this);
//        d->context->setContextProperty("staffModel", &d->staffModel);
//        d->context->setContextProperty("staffStructModel", &d->structModel);
//        d->context->setContextProperty("staffPathModel", &d->staffPathModel);
//        d->context->setContextProperty("staffStructPathModel", &d->structPathModel);
    }

    declared = true;
}

// }}} [ INIT/DESTROY ]
// [ PROPERTIES ]: {{{

bool StaffUIApplication::checkMode() const
{
    return( d->checkMode );
}


void StaffUIApplication::setCheckMode(bool enable)
{
    if( d->checkMode != enable ) {
        d->checkMode = enable;
        Q_EMIT checkModeChanged();
    }
}

// [ STAFF MODE ]:

bool StaffUIApplication::staffMode() const
{
    return( d->viewModeUI == MilitaryStaffGlobals::StaffUIMode );
}


void StaffUIApplication::setStaffMode(bool enabled)
{
    if( enabled ) {
        d->viewModeUI = MilitaryStaffGlobals::StaffUIMode;
    } else {
        d->viewModeUI = MilitaryStaffGlobals::StructureUIMode;
    }

    Q_EMIT viewModeUIChanged();
    Q_EMIT staffModeChanged();
}

// [ UI VIEW MODE ]:

int StaffUIApplication::viewModeUI() const
{
    return(int(d->viewModeUI));
}


void StaffUIApplication::setViewModeUI(int mode)
{
    if( mode != int(d->viewModeUI) ) {
        d->viewModeUI = static_cast<MilitaryStaffGlobals::ViewMode>(mode);

        Q_EMIT viewModeUIChanged();
        Q_EMIT staffModeChanged();
    }
}

// [ USER ACTION ]:

int StaffUIApplication::userAction() const
{
    return(d->userAction);
}


void StaffUIApplication::setUserAction(int userAction)
{
    if( userAction != int(d->userAction) ) {
        d->userAction = static_cast<MilitaryStaffGlobals::UserAction>(userAction);

        Q_EMIT userActionChanged();
    }
}


void StaffUIApplication::setStaffAddress(const QString &address)
{
    d->address = address;
}


QString StaffUIApplication::formHeader() const
{
    return(d->formHeader);
}


void StaffUIApplication::setFormHeader(const QString &header)
{
    if( d->formHeader == header ) {
        d->formHeader = header;
        Q_EMIT formHeaderChanged();
    }
}

// [ UI METHODS ]: {{{

void StaffUIApplication::closeUI()
{
    Q_EMIT close();
}


void StaffUIApplication::createStaff(int index)
{
    StaffObject obj = d->structModel.modelDataFromIndex(index);
    if( obj.isNull() ) return;

    //d->address = "::1";

    if( staffESU->createStaff(obj.toBaseUnit(), QHostAddress(d->address)) )
    {
        //d->staffModel.setModelData(staffEsutz->staff());
        Q_EMIT staffCreated(d->address, obj.nameFull());
    }
}

// }}} [ UI METHODS ]
// [ MODELS PROPERTIES ]: {{{

StaffUIModel &StaffUIApplication::staffModel() const
{
    return(d->staffModel);
}


StaffStructureUIModel &StaffUIApplication::structureModel() const
{
    return(d->structModel);
}


StaffUIPathModel &StaffUIApplication::staffPathModel() const
{
    return(d->staffPathModel);
}


StaffUIPathModel &StaffUIApplication::structurePathModel() const
{
    return(d->structPathModel);
}

// }}} [ MODELS PROPERTIES ]
