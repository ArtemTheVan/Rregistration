#ifndef STAFF_UI_MODEL_P
#define STAFF_UI_MODEL_P

#include "staff_ui_model.h"
#include "esu_military_staff.h"


class StaffUIModelPrivate
{
    Q_DECLARE_PUBLIC(StaffUIModel)

public:
    StaffUIModelPrivate(StaffUIModel* parent):
        q_ptr(parent)
    { }

    ~StaffUIModelPrivate()
    { }

public:
    StaffUIModel* q_ptr;

    StaffDivision           rootElement;
    StaffDivision           currentElement;
    StaffObjectList         data;

    QHash<int, QByteArray>  roleNames;
};

#endif // STAFF_UI_MODEL_P

