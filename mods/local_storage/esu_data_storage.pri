#-------------------------------------------------
# Module:       ESUTZ Local Data Storage Module
# Version:      1.0
# LIBS:         libQtCore, libQtSql
# Qt Versions:  5.2.0 - 5.4.0
# Description:  ESUTZ Data storage module
#-------------------------------------------------

DEFINES += ESU_LOCAL_STORAGE
#DEFINES += ESU_LOCAL_STORAGE_LOG

QT          *= core sql

INCLUDEPATH += ./mods/local_storage


SOURCES += \
            mods/local_storage/localstoragemanager.cpp \
            mods/local_storage/dictionarytable.cpp \
            mods/local_storage/varianttable.cpp \
            mods/local_storage/abstractlocalstoragetable.cpp


HEADERS += \
            mods/local_storage/localstoragemanager.h \
            mods/local_storage/dictionarytable.h \
            mods/local_storage/varianttable.h \
            mods/local_storage/abstractlocalstoragetable.h


