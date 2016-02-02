#-------------------------------------------------
# Module:       ESUTZ Registration Core Module
# Version:      1.0
# LIBS:         libQtCore, libQtXml
# Qt Versions:  5.2.0 - 5.4.0
# Description:  ESUTZ Registration Module provides
#               base registration algorithms for
#               tablet devices.
#-------------------------------------------------

DEFINES         += ESU_REGISTRATION
DEFINES         += ESU_REGISTRATION_LOG
DEFINES         += ESU_REGISTRATION_TEST

DEFINES         += ESU_UI_REGISTRATION
DEFINES         += ESU_UI_REGISTRATION_LOG

INCLUDEPATH     += ./mods/registration/core
INCLUDEPATH     += ./mods/registration/core/engine

INCLUDEPATH     += ./mods/registration/ui/quick1/model


HEADERS += \
            mods/registration/core/esu_registration.h \
            mods/registration/core/private/esu_registration_p.h \
            mods/registration/core/esu_registration_global.h \
            mods/registration/core/engine/esu_registration_engine.h \
            mods/registration/core/engine/esu_registration_engine_p.h \
            mods/registration/core/engine/esu_registration_engine_types.h \
            mods/registration/core/engine/esu_registration_table_manager.h


SOURCES += \
            mods/registration/core/esu_registration.cpp \            
            mods/registration/core/private/esu_registration_p.cpp \
            mods/registration/core/engine/esu_registration_engine.cpp \
            mods/registration/core/engine/esu_registration_engine_p.cpp \
            mods/registration/core/engine/esu_registration_table_manager.cpp

# TEMP:

HEADERS += \
            mods/registration/ui/$$UI_PATH_PREFIX/model/ui_registration_base_model.h \
            mods/registration/ui/$$UI_PATH_PREFIX/model/ui_registration_soldiers_model.h
            #$$PWD/engine/temp/esu_registration_engine.h \
            #$$PWD/engine/temp/esu_registration_engine_p.h

SOURCES += \
            #$$PWD/engine/temp/esu_registration_engine.cpp \
            #$$PWD/engine/temp/esu_registration_engine_p.cpp


RESOURCES += \
            mods/registration/core/registration.qrc
