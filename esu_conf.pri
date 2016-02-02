#-------------------------------------------------
# ESUTZ Project Config
# Version 1.0
#-------------------------------------------------

CONFIG += rector_conf

# VERSION CONFIG:
VERSION =  1.0.0

# PROJECT DEFINES
DEFINES += ESU_LOG_ENABLE

# USER SPECS
QMAKE_USER=$$system(whoami)

# ARCH CONFIG:
QMAKE_ARCH=$$system(g++ -v 2>&1 | grep '^Target:' | sed 's/^Target:\s//')
isEmpty(QMAKE_ARCH): error([ESUTZ]: Project Error in $$_FILE_ at $$_LINE_ --> Unknown Arch)

contains(QMAKE_ARCH, arm-linux-gnueabihf) {
    message ("[ESUTZ]: ARMv7 Build")
    DEFINES += ARMv7
    CONFIG  += armv7
    CONFIG  += debug declarative_debug =qml_debug

} else: contains(QMAKE_ARCH, i686-linux-gnu) {
    message("[ESUTZ]: Host i686 Build")
    CONFIG  += i686
} else: contains(QMAKE_ARCH, x86_64-linux-gnu) {
    message("[ESUTZ]: Host i64 Build")
    CONFIG  += i64
}

# DEVICE SPEC
#DEFINES += ESU_DEV_NPI
#DEFINES += ESU_DEV_ZKPK
DEFINES += ESU_DEV_PKK
#DEFINES += ESU_DEV_DESKTOP

# COMPILER FLAGS
QMAKE_CXXFLAGS += #-fpermissive

# PCH CONFIG:
CONFIG += precompile_header
PRECOMPILED_HEADER = ./pch.h

precompile_header:!isEmpty(PRECOMPILED_HEADER) {
    DEFINES += USING_PCH
    HEADERS += ./pch.h
}

# CODEC CONFIG:
CODECFORTR	= UTF-8
CODECFORSRC	= UTF-8

# BUILD CONFIG:
CONFIG(debug, debug|release) {
    TARGET	= registration

    armv7:  BUILDDIR = build/debug/arm
    else:   BUILDDIR = build/debug/desktop
    message([ESUTZ]: Debug Build)
} else {
    TARGET	= registration
    BUILDDIR	= build/release
    message([ESUTZ]: Release Build)
}

DESTDIR         = $$BUILDDIR
OBJECTS_DIR	= $$BUILDDIR/.obj
MOC_DIR		= $$BUILDDIR/.moc
RCC_DIR		= $$BUILDDIR/.qrc
UI_DIR		= $$BUILDDIR/.ui
UI_HEADERS_DIR  = $$BUILDDIR/.ui
