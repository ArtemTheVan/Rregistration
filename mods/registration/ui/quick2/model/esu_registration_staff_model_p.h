#ifndef ESU_REGISTRATION_STAFF_MODEL_P
#define ESU_REGISTRATION_STAFF_MODEL_P

#include "esu_registration_staff_model.h"
#include "esu_registration_path_model.h"

#include "esu_registration_table_manager.h"

#include "esu_staff_system.h"
#include "esu_staff_structure.h"


class ESURegistrationStaffModelPrivate
{
    Q_DECLARE_PUBLIC(ESURegistrationStaffModel)

public:
    ESURegistrationStaffModelPrivate(ESURegistrationStaffModel* parent):
        q_ptr(parent)
      , dataSource(nullptr)
      , checkable(false)
    { }

    ~ESURegistrationStaffModelPrivate()
    { }

public:
    ESURegistrationStaffModel*      q_ptr;

    ESURegistrationTableManager*    dataSource;

    bool                    checkable;

    StaffDivision           rootElement;
    StaffDivision           currentElement;
    StaffObjectList         data;

    QHash<int, QByteArray>  roleNames;

    ESURegistrationPathModel    modelPath;
};

#endif // ESU_REGISTRATION_STAFF_MODEL_P

