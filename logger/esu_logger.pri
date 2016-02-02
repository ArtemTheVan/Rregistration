#-------------------------------------------------
# Module:       ESUTZ Logger Module
# Version:      1.0
# LIBS:         libQtCore
# Qt Versions:  5.2.0 - 5.4.0
# Description:  ESUTZ Logger Module provides
#               debug functions
#-------------------------------------------------

DEFINES         += ESU_LOGGER
DEFINES         += ESU_LOGGER_ENABLE

QT              *= core

INCLUDEPATH     += $$PWD

HEADERS += \
    $$PWD/esu_logger.h


SOURCES += \
