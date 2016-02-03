PROJECT(LIB$ENV{_NAME_} CXX C)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(_TARGET_ $ENV{_NAME_})

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/$ENV{_DEFARCH_})

MESSAGE("-----------------------------------------")
MESSAGE("--Build debug/release version of ${_TARGET_} for $ENV{_DEFARCH_}.----")
MESSAGE("-----------------------------------------")

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE( ${QT_USE_FILE} )

SET(_QTDIR_ $ENV{_QTDIR_})
SET(CMAKE_CXX_COMPILER gcc)
SET(QT_MOC_EXECUTABLE $ENV{_QTMOC_})

#
SET(_ROOTFSINCDIR_ "-I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtSql -I/usr/include/qt4/QtXml -I/usr/include/qt4")
SET(_TARGETINCDIR_ "-I${CMAKE_CURRENT_SOURCE_DIR}/../../include -I${CMAKE_CURRENT_SOURCE_DIR}/../../include/${_TARGET_} -I$ENV{_MAINDIR_}/include")
SET(_INC_SOURCES_ "${_ROOTFSINCDIR_} ${_TARGETINCDIR_}")

SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGSENVS} $ENV{DEFENVS} ${_INC_SOURCES_}")

SET(_ROOTFSLIBDIR_ "-L/usr/lib/i386-linux-gnu -Wl,--rpath-link=/usr/lib/i386-linux-gnu")
SET(_TARGETLIBDIR_ "-L${CMAKE_CURRENT_SOURCE_DIR}/../../lib/$ENV{_DEFARCH_} -Wl,-rpath,${CMAKE_CURRENT_SOURCE_DIR}/../../lib/$ENV{_DEFARCH_}")
SET(_LIBS_ "-lQtGui -lQtXml")
SET(CMAKE_EXE_LINKER_FLAGS "${_ROOTFSLIBDIR_} ${_TARGETLIBDIR_} ${_LIBS_}")


# Headers and sources (APPEND NEW FILES HERE)
FILE(GLOB _HEADERS_ ../../src/*.h ../../include/${_TARGET_}/*.h)
FILE(GLOB _SOURCES_ ../../src/*.cpp)

# Make moc files
QT_WRAP_CPP(${_TARGET_} _MOC_SOURCES_ ${_HEADERS_})

ADD_LIBRARY(${_TARGET_} SHARED ${_SOURCES_} ${_HEADERS_} ${_MOC_SOURCES_})

TARGET_LINK_LIBRARIES(${_TARGET_} ${QT_LIBRARIES} ${_LIBS_})
