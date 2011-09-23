QT       =

TEMPLATE = lib
CONFIG += static
TARGET = ezlog
QMAKE_CXXFLAGS += "-std=c++0x"

!*msvc*: LIBS += -lpthread

include(../config.pri)
DESTDIR=$$replace(DESTDIR, $${DESTDIR}, ../$${DESTDIR})

SOURCES += \
	ezlog.cpp \
	ezthread.cpp


HEADERS += \
	ezlog.h \
	ezthread.h \
	global.h \
	eztime.h

*msvc* {
	SOURCES += ezthread_win.cpp
} else:*g++* {
	SOURCES += ezthread_posix.cpp
}
