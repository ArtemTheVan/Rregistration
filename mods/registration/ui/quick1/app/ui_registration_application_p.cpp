#include "ui_registration_application_p.h"



UIRegistrationApplicationPrivate::UIRegistrationApplicationPrivate(UIRegistrationApplication *parent):
    q_ptr(parent)
  , registration(nullptr), context(nullptr), uiForm(nullptr)
  , currentMessage(""), progress(false)
  , mode(ESURegistration::UserMode)
{

}


UIRegistrationApplicationPrivate::~UIRegistrationApplicationPrivate()
{

}

