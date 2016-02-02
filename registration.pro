#-------------------------------------------------
# Project created by QtCreator 2016-01-26
# RECTOR Registration Systemm
# Version 1.0 (QQuick(1-2) based UI)
#-------------------------------------------------

QT  += core gui network xml widgets svg

QTPLUGIN += qsvg

DEFINES += RECTOR_PROJECT

TEMPLATE    = app
CONFIG     += qt warn_on debug

# Заглушка
UI_PATH_PREFIX   = quick2

# [project base config]
!include(esu_conf.pri) {
    error([RECTOR]: Project Error in $$_FILE_ at $$_LINE_; project requers <esutz_conf> file)
}

!include(interfaces/interfaces.pri) {
    error([RECTOR]: Project Error in $$_FILE_ at $$_LINE_; project requers <interfaces> file)
}

# [Logger]
!include(logger/esu_logger.pri) {
    error([RECTOR]: Project Error in $$_FILE_ at $$_LINE_; project requers <esu_logger> file)
}

# [network]
#include(net/esu_net.pri)

# [modules]: {

# [local storage]
include(mods/local_storage/esu_data_storage.pri)

# [staff]
include(mods/staff/core/staff.pri)

# [registration]
include(mods/registration/registration.pri)

# } [modules]


SOURCES += main.cpp
