#include "esu_registration_ui.h"
#include "esu_registration_engine.h"

#include "esu_registration_events.h"
#include "esu_staff_system.h"

#ifdef ESU_APPLICATION
#include "esu_application.h"
#include "esu_logger.h"
#endif
#ifdef ESU_UI_MODULE
#include "esu_window_manager.h"
#endif

#ifdef ESU_UI_QUICK
#include <QtQml>
#endif


#undef ESU_REGISTRATION_TEST

ESURegistrationUI::ESURegistrationUI(ESURegistration *_base, QObject *parent):
    QObject(parent)
  , base(_base)
#ifdef ESU_UI_QUICK
  , m_context(nullptr)
#endif
  , m_fieldHeight(40.)
  , m_taskTimerRunning(false), m_taskTimerInterval(0)
{
    //
}


ESURegistrationUI::~ESURegistrationUI()
{
    //
}

// [ QML Methods ]: {{{
#ifdef ESU_UI_QUICK

void ESURegistrationUI::registerQmlProperties(QQmlContext *context)
{
    static bool registered = false;
    if( registered || !base ) return;

    if( m_context != context && context != nullptr )
        m_context = context;

    if( m_context == nullptr ) {
        ESU_WARNING_LOG("Wrong QMl Context", "EsuRegistration");
        return;
    }

    m_context->setContextProperty("registrationEsu", base);
    m_context->setContextProperty("registrationEsuUI", this);

    connect( base, &ESURegistration::showRegistrarFormEvent,
             this, &ESURegistrationUI::showFormEvent, Qt::QueuedConnection );

    registered = true;
}


void ESURegistrationUI::declareQmlTypes()
{
    static bool declared = false;
    if( declared ) return;

    qmlRegisterUncreatableType<ESURegistration>(
                "MKBKompas.CSR.ESUTZ.Registration", 1, 0, "EsuRegistration", "Can\'t create ESURegistration instance!");
    qmlRegisterUncreatableType<ESURegistrationUI>(
                "MKBKompas.CSR.ESUTZ.Registration", 1, 0, "EsuRegistrationUI", "Can\'t create ESURegistration instance!");

    qmlRegisterType<ESURegistrationPathModel>("MKBKompas.CSR.ESUTZ.Registration", 1, 0, "EsuRegistrationPathModel");
    qmlRegisterType<ESURegistrationRanksModel>("MKBKompas.CSR.ESUTZ.Registration", 1, 0, "EsuRegistrationRanksModel");

    declared = true;
}


void ESURegistrationUI::createQmlModels(QQmlContext* context)
{
    static bool registered = false;
    if( registered || !m_context || !base ) return;

    if( m_context != context && context ) m_context = context;
    if( m_context == nullptr ) {
        ESU_WARNING_LOG("Wrong QMl Context", "EsuRegistration");
        return;
    }

    modelRegistrationTable.setDataSource(base->engine()->tableManager());

    m_context->setContextProperty("regRanksModel", &modelRanks);
    m_context->setContextProperty("regQueriesModel", &modelQueries);
    m_context->setContextProperty("regUpdateQueriesModel", &modelUpdateQueries);
    m_context->setContextProperty("registrationStaffModel", &modelStaff);
    m_context->setContextProperty("staffPathModel", modelStaff.pathModel());
    m_context->setContextProperty("registrationTableModel", &modelRegistrationTable);

    //if( !staffBase.load(":/mods/staff/conf/staff_structure_msb.xml") ) return;
    modelStaff.setModelData( esuStaff->data().rootDivision() );
    modelStaff.setDataSource( base->engine()->tableManager() );

    registered = true;
}

#else

void ESURegistrationUI::createModels()
{
    static bool registered = false;
    if( registered || !base ) return;

    modelRegistrationTable.setDataSource( base->engine()->tableManager() );
    modelStaff.setModelData( esuStaff->data().rootDivision() );
    modelStaff.setDataSource( base->engine()->tableManager() );

    registered = true;
}

#endif
// }}} [ QML Methods ]
// [ PROPERTIES ]: {{{

qreal ESURegistrationUI::fieldHeight() const
{
    return( m_fieldHeight );
}

void ESURegistrationUI::setFieldHeight(qreal height)
{
    m_fieldHeight = height;
    Q_EMIT fieldHeightChanged();
}

// }}} [ PROPERTIES ]
// [ UI METHODS ]: {{{

void ESURegistrationUI::startTaskTimer(int msecs)
{
    QMetaObject::invokeMethod(base->currentUiForm(), "startTaskProgress", Qt::QueuedConnection,
                              Q_ARG(QVariant, QVariant(msecs)));
}


void ESURegistrationUI::stopTaskTimer()
{
    QMetaObject::invokeMethod(base->currentUiForm(), "stopTaskProgress", Qt::DirectConnection);
}

// }}}
// [ EVENTS/SLOTS ]:

void ESURegistrationUI::showFormEvent()
{
#ifdef ESU_UI_MODULE
    esuWinManager->showWindowName("RegistrationForm");
#endif
}
