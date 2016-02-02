#ifndef ESU_REGISTRATION_VIEWER_P
#define ESU_REGISTRATION_VIEWER_P

#include "esu_registration_viewer.h"

#include "esu_registration.h"
#include "ui_registration_application.h"


class ESURegistrationViewerPrivate
{
    Q_DECLARE_PUBLIC(ESURegistrationViewer)

public:
    ESURegistrationViewerPrivate(ESURegistrationViewer* parent = nullptr):
        q_ptr(parent)
    { }

    ~ESURegistrationViewerPrivate() { }

public:
    void init();

public:
    ESURegistrationViewer* q_ptr;
    UIRegistrationApplication appUI;
};


void ESURegistrationViewerPrivate::init()
{
    Q_Q(ESURegistrationViewer);

    UIRegistrationApplication::declareQmlTypes();
    appUI.registerQmlProperties(q->rootContext());
}

#endif // ESU_REGISTRATION_VIEWER_P

