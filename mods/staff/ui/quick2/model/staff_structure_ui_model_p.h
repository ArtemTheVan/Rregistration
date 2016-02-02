#ifndef STAFF_STRUCTURE_UI_MODEL_P_H
#define STAFF_STRUCTURE_UI_MODEL_P_H

#include "staff_structure_ui_model.h"

#include "esu_staff_system.h"
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
        /*
        currentElement = staffESU->staffStructure();
        rootElement = currentElement;
        if( !currentElement.isNull() ) {
            data = currentElement.objects();            
        }
        */
    }

public:
    StaffStructureUIModel*   q_ptr;

    StaffDivision           rootElement;
    StaffDivision           currentElement;

    StaffObjectList         data;

    QHash<int, QByteArray>  roleNames;
};

#endif // STAFF_STRUCTURE
