TEMPLATE = lib

QT +=
QT       -= gui
CONFIG   += console
CONFIG   -= app_bundle

CONFIG *= ezlog-buildlib

!include(libezlog.pri): error(could not find libezlog.pri)

#src
unix: SOURCES +=
else:win32: SOURCES +=


HEADERS += ezlog_global.h ezlog.h \
        ezthread.h \
        global.h \
        eztime.h \
    ezlog_global.h


SOURCES += \
        ezlog.cpp \
        ezthread.cpp

QMAKE_CXXFLAGS += "-std=c++0x"

!*msvc*: LIBS += -lpthread

*msvc* {
	SOURCES += ezthread_win.cpp
} else:*g++* {
	SOURCES += ezthread_posix.cpp
}
