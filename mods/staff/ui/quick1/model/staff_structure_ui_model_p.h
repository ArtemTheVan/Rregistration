#ifndef STAFF_STRUCTURE_UI_MODEL_P_H
#define STAFF_STRUCTURE_UI_MODEL_P_H

#include "staff_structure_ui_model.h"

#include "ui_staff_application.h"
#include "esu_military_staff.h"
#include "staff_ui_path_model.h"


class StaffStructureUIModelPrivate
{
    Q_DECLARE_PUBLIC(StaffStructureUIModel)

public:
    StaffStructureUIModelPrivate(StaffStructureUIModel* parent = nullptr):
        q_ptr(parent)
    { }

    ~StaffStructureUIModelPrivate()
    { }

    void init()
    {
        if( !staffESU->hasStructure() ) return;

        currentElement = staffESU->staffStructure();
        rootElement = currentElement;
        if( !currentElement.isNull() ) {
            data = currentElement.objects();            
        }
    }

public:
    StaffStructureUIModel*   q_ptr;

    StaffDivision           rootElement;
    StaffDivision           currentElement;

    StaffObjectList         data;

    QHash<int, QByteArray>  roleNames;
};

#endif // STAFF_STRUCTURE
