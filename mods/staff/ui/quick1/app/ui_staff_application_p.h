#ifndef UI_STAFF_APPLICATION_PRIVATE_H
#define UI_STAFF_APPLICATION_PRIVATE_H

#include "ui_staff_application.h"
#include "esu_military_staff.h"

#include "esu_application.h"
#include "esu_globals_id.h"

#include "esu_window_manager.h"

#include "staff_ui_model.h"
#include "staff_structure_ui_model.h"
#include "staff_ui_path_model.h"

#ifdef ESU_UI_DECLARATIVE
    #include <QtDeclarative> // QtQuick 1.1
#else
    #include <QtQml> // QtQuick 2.0 or higher
#endif


class UIStaffApplicationPrivate
{
    Q_DECLARE_PUBLIC(UIStaffApplication)

public:
    UIStaffApplicationPrivate(UIStaffApplication* parent = nullptr);
    ~UIStaffApplicationPrivate();

public:
    UIStaffApplication* q_ptr;
    QDeclarativeContext* context;
    QDeclarativeItem* uiStaffForm;
    QDeclarativeItem* uiStructureForm;

    UIStaffApplication::Mode mode;
    QString caption;
    QString registrationAddress;

    StaffUIModel            staffModel;
    StaffStructureUIModel   structModel;
    StaffUIPathModel        staffPathModel;
    StaffUIPathModel        structPathModel;
};

#endif // UI_STAFF_APPLICATION_PRIVATE_H
