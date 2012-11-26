TEMPLATE = app
QT =
QT	   -= gui
CONFIG   += console
CONFIG   -= app_bundle
#DEFINES += NO_EZLOG
TARGET = tst_ezlog
STATICLINK = 0
PROJECTROOT = $$PWD/..
isEmpty(BUILD_DIR):BUILD_DIR=$$(BUILD_DIR)
isEmpty(BUILD_DIR):BUILD_DIR=$$[BUILD_DIR]
isEmpty(BUILD_DIR):BUILD_IN_SRC = yes

!isEmpty(BUILD_IN_SRC):BUILD_DIR=$$OUT_PWD/../out
include($${PROJECTROOT}/common.pri)
!include(../src/libezlog.pri): error(could not find libezlog.pri)
#include($${PROJECTROOT}/common.pri)
#win32:LIBS += -lUser32

SOURCES += \
    main.cpp
HEADERS +=
unix: LIBS += -lpthread
