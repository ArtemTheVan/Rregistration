#-------------------------------------------------
# ESUTZ UI MilitatyStaff Module
# Version 1.0
# LIBS: libQtDeclarative(QtQuick 1)
# Qt Version: 5.3.1
#-------------------------------------------------

!contains(DEFINES, "ESU_STAFF") {
    error([ESUTZ]: Project Error in $$_FILE_ at $$_LINE_; project requers <MILITARY_STAFF> module)
}

DEFINES         += ESU_UI_MILITARY_STAFF
DEFINES         += ESU_UI_MILITARY_STAFF_LOG

INCLUDEPATH     += ./mods/staff/ui/quick2
INCLUDEPATH     += ./mods/staff/ui/quick2/qml
INCLUDEPATH     += ./mods/staff/ui/quick2/model


HEADERS += \
            # [ STAFF MODEL ]
            mods/staff/ui/quick2/model/staff_ui_model.h \
            mods/staff/ui/quick2/model/staff_ui_model_p.h \
            # [ STAFF PATH MODEL ]
            mods/staff/ui/quick2/model/staff_ui_path_model.h \
            # [ STAFF STRUCTURE MODEL ]
            mods/staff/ui/quick2/model/staff_structure_ui_model_p.h \
            mods/staff/ui/quick2/model/staff_structure_ui_model.h


SOURCES += \
            # [ STAFF MODEL ]
            mods/staff/ui/quick2/model/staff_ui_model.cpp \
            # [ STAFF STRUCTURE MODEL ]
            mods/staff/ui/quick2/model/staff_structure_ui_model.cpp \
            # [ STAFF PATH MODEL ]
            mods/staff/ui/quick2/model/staff_ui_path_model.cpp


RESOURCES += \
            mods/staff/ui/quick2/ui_military_staff.qrc
