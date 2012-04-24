TEMPLATE = lib

QT =
CONFIG   += console
CONFIG   -= app_bundle

CONFIG *= ezlog-buildlib

!include(libezlog.pri): error(could not find libezlog.pri)

#src
unix: SOURCES +=
else:win32: SOURCES +=


HEADERS += ezlog.h \
		ezthread.h \
		eztime.h \
		ezlog_global.h \
		list.h \
		appender.h \
		layout.h


SOURCES += \
    appender.c \
    ezlog.c \
    ezthread.c \
    eztime.c \
    layout.c

#QMAKE_CXXFLAGS += "-std=c++0x"

!*msvc*: LIBS += -lpthread

*msvc* {
        SOURCES += ezthread_win.c
} else:*cc* {
        SOURCES += ezthread_posix.c
}

lib.files =
sources.files = ezlog_global.h ezlog.h appender.h layout.h
sources.path = /opt/usr/src/ezlog

INSTALLS += lib sources
