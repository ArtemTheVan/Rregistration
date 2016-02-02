#-------------------------------------------------
# Module:       ESUTZ Registration UI Module
# Version:      1.0
# LIBS:         libQtCore, libQtXml, libQtDeclarative
# Qt Versions:  5.2.0 - 5.4.0
# Description:  UI for Registration Core Module
#-------------------------------------------------

DEFINES         += ESU_UI_REGISTRATION
DEFINES         += ESU_UI_REGISTRATION_LOG

INCLUDEPATH     += ./mods/registration/ui/quick1
INCLUDEPATH     += ./mods/registration/ui/quick1/viewer
INCLUDEPATH     += ./mods/registration/ui/quick1/qml
INCLUDEPATH     += ./mods/registration/ui/quick1/app


HEADERS += \
            mods/registration/ui/quick1/viewer/esu_registration_viewer.h \
            mods/registration/ui/quick1/viewer/esu_registration_viewer_p.h \
            mods/registration/ui/quick1/app/ui_registration_application.h \
            mods/registration/ui/quick1/app/ui_registration_application_p.h \
            mods/registration/ui/quick1/model/ui_registration_base_model.h \
            mods/registration/ui/quick1/model/ui_registration_soldiers_model.h


SOURCES += \
            mods/registration/ui/quick1/viewer/esu_registration_viewer.cpp \
            mods/registration/ui/quick1/app/ui_registration_application.cpp \
            mods/registration/ui/quick1/app/ui_registration_application_p.cpp \
            mods/registration/ui/quick1/model/ui_registration_base_model.cpp \
            mods/registration/ui/quick1/model/ui_registration_soldiers_model.cpp


RESOURCES += \
            mods/registration/ui/quick1/ui_registration.qrc
