#ifndef ESU_REGISTRATION_UI_PRIVATE_H
#define ESU_REGISTRATION_UI_PRIVATE_H

#include "esu_registration_ui.h"



class ESURegistrationUIPrivate
{
    Q_DECLARE_PUBLIC(ESURegistrationUI)

public:
    ESURegistrationUIPrivate(ESURegistrationUI *parent = nullptr):
        q_ptr(parent), base(nullptr)
      , context(nullptr), uiUserForm(nullptr), uiFormItem(nullptr)
      , uiRegistrarForm(nullptr)
      ,
    { }

    ~ESURegistrationUIPrivate()
    { }

public:
    ESURegistrationUI   *q_ptr;
    ESURegistration     *base;


    QQuickItem          *uiUserForm;
    QQuickItem          *uiFormItem;
    QQuickItem          *uiRegistrarForm;



    QHash<QString, QQuickItem*> uiUserPages;
    QHash<QString, QQuickItem*> uiRegistrarPages;


};

#endif // ESU_REGISTRATION_UI_PRIVATE_H
