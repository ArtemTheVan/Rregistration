#ifndef ESU_REGISTRATION_P_H
#define ESU_REGISTRATION_P_H

#include "registration.h"
#include "registration_engine.h"


class ESURegistrationPrivate
{
    Q_DECLARE_PUBLIC(ESURegistration)

public:
    ESURegistrationPrivate(ESURegistration* parent = nullptr);
    ~ESURegistrationPrivate();

    void init();

public:
    ESURegistration* q_ptr;

    ESURegistration::RegistrationState  registrationState;
    ESURegistration::Profile            profile;
    ESURegistration::OperationState     operationState;
    ESURegistration::GeneralState       state;
    ESURegistration::Error              error;
    ESURegistration::Task               currentTask;

    QString     currentMessage;
    QString     errorMessage;
    QString     currentAddress;
    QString     currentRegistrarAddress;
    int         currentRegistrarIndex;
    int         registrarCount;
    int         registrarCheckCount;

    QObject             *currentUiForm;
//    ESURegistrationUI   *ui;

    RegistrationUserInfo choosedData;

    QStack<ESURegistration::Task> taskStack;
    RegistrationUserInfo dataInfo;

    ESURegistrationEngine engine;

private:
    Q_FRIEND(ESURegistrationEngine)
};

#endif // ESU_REGISTRATION_P_H
