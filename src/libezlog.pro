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


HEADERS += ezlog.h \
	eztime.h \
	ezlog_global.h \
	list.h \
	appender.h \
	layout.h \
	os.h \
	cpu.h \
	ezmutex.h \
	prepost.h


SOURCES += \
	appender.c \
	ezlog.c \
	eztime.c \
	layout.c \
    ezmutex.c

#QMAKE_CXXFLAGS += "-std=c++0x"

#unix: LIBS += -lpthread
!*msvc*: LIBS += -lpthread

#lib.files =
#sources.files = ezlog_global.h ezlog.h appender.h layout.h
#sources.path = /usr/local/include/ezlog

#INSTALLS += sources #lib
