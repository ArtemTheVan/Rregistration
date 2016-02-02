#include "private/esu_registration_p.h"



ESURegistrationPrivate::ESURegistrationPrivate(ESURegistration *parent):
    q_ptr(parent)
  , registrationState(ESURegistration::UnregisteredState), profile(ESURegistration::UnknownProfile)
  , operationState(ESURegistration::NoOperationState), state(ESURegistration::SteadyState)
  , error(ESURegistration::NoErrors), currentTask(ESURegistration::NoTasks)
  , currentMessage("Нет задач"), errorMessage("Нет ошибок")
  , currentAddress(""), currentRegistrarAddress("")
  , currentRegistrarIndex(0), registrarCount(0), registrarCheckCount(0)
  , currentUiForm(nullptr), ui(new ESURegistrationUI(parent, parent))
{
    engine.setBase(parent);
    engine.setUI(ui);

    dataInfo.role = "ПРОФИЛЬ НЕ ЗАДАН";
    dataInfo.name = "";
    //choosedRankIndex = -1;
}


ESURegistrationPrivate::~ESURegistrationPrivate()
{

}


void ESURegistrationPrivate::init()
{
    //
}

