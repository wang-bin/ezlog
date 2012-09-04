TEMPLATE = app
QT =
QT	   -= gui
CONFIG   += console
CONFIG   -= app_bundle
#DEFINES += NO_EZLOG
TARGET = tst_ezlog
PROJECTROOT = $$PWD/..
!include(../src/libezlog.pri): error(could not find libezlog.pri)
#include($${PROJECTROOT}/common.pri)
#win32:LIBS += -lUser32

SOURCES += \
    main.cpp
HEADERS +=
unix: LIBS += -lpthread
