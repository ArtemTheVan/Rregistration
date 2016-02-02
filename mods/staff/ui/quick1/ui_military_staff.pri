#-------------------------------------------------
# ESUTZ UI MilitatyStaff Module
# Version 1.0
# LIBS: libQtDeclarative(QtQuick 1)
# Qt Version: 5.3.1
#-------------------------------------------------

!contains(DEFINES, "ESU_MILITARY_STAFF") {
    error([ESUTZ]: Project Error in $$_FILE_ at $$_LINE_; project requers <MILITARY_STAFF> module)
}

DEFINES         += ESU_UI_MILITARY_STAFF
DEFINES         += ESU_UI_MILITARY_STAFF_LOG


INCLUDEPATH     += ./mods/military_staff/ui/quick1
INCLUDEPATH     += ./mods/military_staff/ui/quick1/app
INCLUDEPATH     += ./mods/military_staff/ui/quick1/viewer
INCLUDEPATH     += ./mods/military_staff/ui/quick1/qml
INCLUDEPATH     += ./mods/military_staff/ui/quick1/model
INCLUDEPATH     += ./mods/military_staff/ui/quick1/controller


HEADERS += \
            # [ UI APP ]
            mods/military_staff/ui/quick1/app/ui_staff_application.h \
            mods/military_staff/ui/quick1/app/ui_staff_application_p.h \
            # [ STAFF MODEL ]
            mods/military_staff/ui/quick1/model/staff_ui_model.h \
            mods/military_staff/ui/quick1/model/staff_ui_model_p.h \
            # [ STAFF PATH MODEL ]
            mods/military_staff/ui/quick1/model/staff_ui_path_model.h \
            # [ STAFF STRUCTURE MODEL ]
            mods/military_staff/ui/quick1/model/staff_structure_ui_model_p.h \
            mods/military_staff/ui/quick1/model/staff_structure_ui_model.h
            # [ UI VIEWER ]
            #mods/military_staff/ui/quick1/viewer/military_staff_viewer.h \
            #mods/military_staff/ui/quick1/viewer/military_staff_viewer_p.h \
            #mods/military_staff/ui/quick1/controller/staff_ui_application.h \
            #mods/military_staff/ui/quick1/private/military_staff_globals_p.h


SOURCES += \
            # [ UI APP ]
            mods/military_staff/ui/quick1/app/ui_staff_application.cpp \
            mods/military_staff/ui/quick1/app/ui_staff_application_p.cpp \
            # [ STAFF MODEL ]
            mods/military_staff/ui/quick1/model/staff_ui_model.cpp \
            # [ STAFF STRUCTURE MODEL ]
            mods/military_staff/ui/quick1/model/staff_structure_ui_model.cpp \
            # [ STAFF PATH MODEL ]
            mods/military_staff/ui/quick1/model/staff_ui_path_model.cpp
            # [ UI VIEWER ]
            #mods/military_staff/ui/quick1/viewer/military_staff_viewer.cpp \
            #mods/military_staff/ui/quick1/controller/staff_ui_application.cpp


RESOURCES += \
            mods/military_staff/ui/quick1/ui_military_staff.qrc
