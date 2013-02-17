QT     -= gui
CONFIG *= ezlog-qt-buildlib
TARGET = ezlog-qt
TEMPLATE = lib

STATICLINK = 0
PROJECTROOT = $$PWD/../..
#the include order is important. it affects BUILD_DIR(here we want $$OUT_PWD/../../out by default)
!include(libezlog-qt.pri): error("Could not find libezlog-qt.pri")
!include($$PROJECTROOT/src/libezlog.pri): error("Could not find libezlog.pri")
preparePaths($$OUT_PWD/../../out)

win32:RC_FILE = $${PROJECTROOT}/res/ezlog-qt.rc
OTHER_FILES += $$RC_FILE

DEFINES += QTEZLOG_LIBRARY

SOURCES += qtezlog.cpp

HEADERS += qtezlog.h\
        qtezlog_global.h

