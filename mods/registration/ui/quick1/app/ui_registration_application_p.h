#ifndef UIREGISTRATIONAPPLICATIONPRIVATE_H
#define UIREGISTRATIONAPPLICATIONPRIVATE_H

#include "ui_registration_application.h"

#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDeclarativeItem>

#include "esu_window_manager.h"

#include "esu_application.h"
#include "esu_globals_id.h"
#include "esu_registration_engine.h"


class UIRegistrationApplicationPrivate
{
    Q_DECLARE_PUBLIC(UIRegistrationApplication)

public:
    UIRegistrationApplicationPrivate(UIRegistrationApplication* parent = nullptr);
    ~UIRegistrationApplicationPrivate();

public:
    UIRegistrationApplication* q_ptr;
    ESURegistration* registration;    
    QDeclarativeContext* context;
    QDeclarativeItem* uiForm;

    QString currentMessage;
    bool progress;

    ESURegistration::Mode mode;

    QQueue<QString> notifyQueue;
    bool notifyAccepted;
};

#endif // UIREGISTRATIONAPPLICATIONPRIVATE_H
