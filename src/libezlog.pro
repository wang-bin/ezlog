TEMPLATE = lib

QT =
CONFIG += console
CONFIG -= app_bundle

CONFIG *= ezlog-buildlib

PROJECTROOT = $$PWD/..
include($$PROJECTROOT/deploy.pri)
isEmpty(BUILD_DIR):BUILD_DIR=$$(BUILD_DIR)
isEmpty(BUILD_DIR):BUILD_DIR=$$[BUILD_DIR]
isEmpty(BUILD_DIR):BUILD_IN_SRC = yes

!isEmpty(BUILD_IN_SRC):BUILD_DIR=$$OUT_PWD/../out
!include(libezlog.pri): error(could not find libezlog.pri)

#src
unix: SOURCES +=
else:win32: SOURCES +=


HEADERS += $$HEADER_PATH/ezlog/*.h


SOURCES += \
	appender.c \
	ezlog.c \
	eztime.c \
	layout.c \
        ezmutex.c

#unix: LIBS += -lpthread
!*msvc*: LIBS += -lpthread

#lib.files =
devel.files = $$HEADERS
devel.path = /usr/local/include/ezlog

INSTALLS += devel #lib
