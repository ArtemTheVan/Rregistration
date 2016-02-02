#--------------------------------------------------------
# Module:       ESUTZ Military Staff Core Module
# Version:      1.0
# LIBS:         libQtXml, libQtNetwork, libQtXmlPatterns
# Qt Versions:  5.2.0 - 5.4.0
# Description:  ESUTZ Staff module
#--------------------------------------------------------

QT              *= xml network

DEFINES         += ESU_STAFF
DEFINES         += ESU_STAFF_LOG


INCLUDEPATH     += ./mods/staff/core
INCLUDEPATH     += ./mods/staff/core/conf
INCLUDEPATH     += ./mods/staff/core/engine
INCLUDEPATH     += ./mods/staff/core/structures


HEADERS += \
            # [Base]
            mods/staff/core/esu_staff_system.h \
            mods/staff/core/esu_staff_system_p.h \
            mods/staff/core/esu_staff_structure.h \
            mods/staff/core/esu_staff_structure_p.h \
            # [Engine]
            mods/staff/core/engine/esu_staff_engine.h \
            # [Staff Globals]
            mods/staff/core/structures/staff_globals.h \
            mods/staff/core/structures/staff_types.h \
            # [Data structures]
            mods/staff/core/structures/staff_base_unit.h \
            mods/staff/core/structures/staff_base_unit_p.h \
            mods/staff/core/structures/staff_soldier.h \
            mods/staff/core/structures/staff_soldier_p.h \            
            mods/staff/core/structures/staff_commander.h \
            mods/staff/core/structures/staff_commander_p.h \
            mods/staff/core/structures/staff_division.h \
            mods/staff/core/structures/staff_division_p.h \
            mods/staff/core/structures/staff_object.h \
            mods/staff/core/structures/staff_object_p.h
            # [Data list structures]
            #mods/staff/core/structures/staff_division_list.h \
            #mods/staff/core/structures/staff_division_list_p.h


SOURCES += \
            # [Base]
            mods/staff/core/esu_staff_system.cpp \
            mods/staff/core/esu_staff_system_p.cpp \
            mods/staff/core/esu_staff_structure.cpp \
            # [Engine]
            mods/staff/core/engine/esu_staff_engine.cpp \
            # [Staff Globals]
            mods/staff/core/structures/staff_globals.cpp \
            mods/staff/core/structures/staff_types.cpp \
            # [Data structures]
            mods/staff/core/structures/staff_base_unit.cpp \
            mods/staff/core/structures/staff_soldier.cpp \
            mods/staff/core/structures/staff_commander.cpp \
            mods/staff/core/structures/staff_division.cpp \
            mods/staff/core/structures/staff_division_p.cpp \
            mods/staff/core/structures/staff_base_unit_p.cpp \
            mods/staff/core/structures/staff_commander_p.cpp \
            mods/staff/core/structures/staff_soldier_p.cpp \
            mods/staff/core/structures/staff_object.cpp \
            mods/staff/core/structures/staff_object_p.cpp
            # [Data list structures]
            #mods/staff/core/structures/staff_division_list.cpp


RESOURCES += \
              mods/staff/core/staff.qrc
