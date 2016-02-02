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

DEFINES         += ESU_UI_REGISTRATION
DEFINES         += ESU_UI_REGISTRATION_LOG

QT              *= network

INCLUDEPATH     += ./mods/registration/core
INCLUDEPATH     += ./mods/registration/core/engine

#INCLUDEPATH     += ./mods/registration/ui/$$UI_PATH_PREFIX/
#INCLUDEPATH     += ./mods/registration/ui/$$UI_PATH_PREFIX/model


# [ CORE ]:

HEADERS += \
            mods/registration/core/esu_registration.h \
            mods/registration/core/private/esu_registration_p.h \
            mods/registration/core/esu_registration_global.h \
            mods/registration/core/esu_registration_data.h \
            mods/registration/core/esu_registration_events.h \
            mods/registration/core/esu_registration_net.h \
            mods/registration/core/esu_registration_types_net.h \
            mods/registration/core/engine/esu_registration_engine.h \
            mods/registration/core/engine/esu_registration_engine_p.h \
            mods/registration/core/engine/esu_registration_engine_types.h \
            mods/registration/core/engine/esu_registration_table_manager.h


SOURCES += \
            mods/registration/core/esu_registration.cpp \            
            mods/registration/core/private/esu_registration_p.cpp \
            mods/registration/core/engine/esu_registration_engine.cpp \
            mods/registration/core/engine/esu_registration_engine_p.cpp \
            mods/registration/core/engine/esu_registration_table_manager.cpp \
            mods/registration/core/esu_registration_data.cpp


# [ UI ]:

#HEADERS += \
#            mods/registration/ui/$$UI_PATH_PREFIX/esu_registration_ui.h \            
#            mods/registration/ui/$$UI_PATH_PREFIX/model/esu_registration_models.h \
#            mods/registration/ui/$$UI_PATH_PREFIX/model/esu_registration_path_model.h \
#            mods/registration/ui/$$UI_PATH_PREFIX/model/esu_registration_staff_model.h \
#            mods/registration/ui/$$UI_PATH_PREFIX/model/esu_registration_staff_model_p.h \
#            mods/registration/ui/$$UI_PATH_PREFIX/model/esu_registration_queries_model.h \
#            mods/registration/ui/$$UI_PATH_PREFIX/model/esu_registration_ranks_model.h \
#            mods/registration/ui/$$UI_PATH_PREFIX/model/esu_registration_table_model.h


#SOURCES += \
#            mods/registration/ui/$$UI_PATH_PREFIX/esu_registration_ui.cpp \            
#            mods/registration/ui/$$UI_PATH_PREFIX/model/esu_registration_path_model.cpp \
#            mods/registration/ui/$$UI_PATH_PREFIX/model/esu_registration_staff_model.cpp \
#            mods/registration/ui/$$UI_PATH_PREFIX/model/esu_registration_queries_model.cpp \
#            mods/registration/ui/$$UI_PATH_PREFIX/model/esu_registration_ranks_model.cpp \
#            mods/registration/ui/$$UI_PATH_PREFIX/model/esu_registration_table_model.cpp
#

#RESOURCES += \
#            mods/registration/core/registration.qrc \
#            mods/registration/ui/$$UI_PATH_PREFIX/ui_registration.qrc
