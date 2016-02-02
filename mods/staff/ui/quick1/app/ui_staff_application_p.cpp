#include "ui_staff_application_p.h"


// [ INIT/DESTROY ]: {{{

UIStaffApplicationPrivate::UIStaffApplicationPrivate(UIStaffApplication *parent):
    q_ptr(parent)
  , context(nullptr), uiStaffForm(nullptr), uiStructureForm(nullptr)
  , mode(UIStaffApplication::ViewMode)
{

}


UIStaffApplicationPrivate::~UIStaffApplicationPrivate()
{

}

// }}} [ INIT/DESTROY ]
