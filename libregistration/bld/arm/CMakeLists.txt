PROJECT(LIB$ENV{_NAME_} CXX C)

# Более ранние версии cmake не пробовали.
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
SET(_TARGET_ $ENV{_NAME_})

SET(MAIN_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/$ENV{_DEFARCH_})

MESSAGE("-----------------------------------------")
MESSAGE("--Build debug/release version of ${_TARGET_} for $ENV{_DEFARCH_}.----")
MESSAGE("-----------------------------------------")

SET(GCC_VER $ENV{_GCCVER_})

SET(OPEN_EMB_CXX_COMPILER $ENV{_CROSSDIR_})
SET(ARM_ANG_LINUX_GNU ${OPEN_EMB_CXX_COMPILER}/$ENV{_CROSSBIN_})

SET(_QTDIR_ $ENV{_QTDIR_})
SET(OPEN_ROOTFS ${_QTDIR_})

SET(CMAKE_CXX_COMPILER ${OPEN_EMB_CXX_COMPILER}/bin/$ENV{_CROSSBIN_}-g++)
SET(QT_MOC_EXECUTABLE $ENV{_QTMOC_})

#
SET(_ROOTFSINCDIR_ "-I${OPEN_EMB_CXX_COMPILER}/include -I${OPEN_ROOTFS}/usr/include/c++ -I${OPEN_ROOTFS}/usr/include/c++/$ENV{_CROSSBIN_} -I${ARM_ANG_LINUX_GNU}/include/c++ -I${ARM_ANG_LINUX_GNU}/include/c++/${GCC_VER}/arm-angstrom-linux-gnueabi -I${ARM_ANG_LINUX_GNU}/include/c++/${GCC_VER}/arm-angstrom-linux-gnueabi/bits -I${ARM_ANG_LINUX_GNU}/include -I${_QTDIR_}/usr/include -I${_QTDIR_}/usr/include/qt4 -I${_QTDIR_}/usr/include/qt4/Qt -I${_QTDIR_}/usr/include/qt4/QtCore -I${_QTDIR_}/usr/include/qt4/QtGui -I${_QTDIR_}/usr/include/qt4/QtSql -I${_QTDIR_}/usr/include/qt4/QtXml")
SET(_TARGETINCDIR_ "-I${MAIN_PROJECT_PATH}/include -I${MAIN_PROJECT_PATH}/../src -I${MAIN_PROJECT_PATH}/../include -I${MAIN_PROJECT_PATH}/include/${_TARGET_}")
SET(_INC_SOURCES_ "${_ROOTFSINCDIR_} ${_TARGETINCDIR_}")

SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGSENVS} $ENV{DEFENVS} ${_INC_SOURCES_}")

SET(_ROOTFSLIBDIR_ "-L${OPEN_ROOTFS}/usr/lib -Wl,-rpath,${OPEN_ROOTFS}/usr/lib")
SET(_TARGETLIBDIR_ "-L${MAIN_PROJECT_PATH}/lib/$ENV{_DEFARCH_} -Wl,-rpath,${MAIN_PROJECT_PATH}/lib/$ENV{_DEFARCH_} -L$ENV{_MAINDIR_}/lib/arm -Wl,-rpath,$ENV{_MAINDIR_}/lib/arm")
SET(_LIBS_ "")
SET(CMAKE_EXE_LINKER_FLAGS "${_ROOTFSLIBDIR_} ${_TARGETLIBDIR_}${_LIBS_}")

# Headers and sources (APPEND NEW FILES HERE)
FILE(GLOB _HEADERS_ ${MAIN_PROJECT_PATH}/src/*.h ${MAIN_PROJECT_PATH}/include/${_TARGET_}/*.h)
FILE(GLOB _SOURCES_ ${MAIN_PROJECT_PATH}/src/*.cpp)

# Make moc files
QT_WRAP_CPP(${_TARGET_} _MOC_SOURCES_ ${_HEADERS_})

ADD_LIBRARY(${_TARGET_} SHARED ${_SOURCES_} ${_HEADERS_} ${_MOC_SOURCES_})

TARGET_LINK_LIBRARIES(${_TARGET_} ${CMAKE_EXE_LINKER_FLAGS})
