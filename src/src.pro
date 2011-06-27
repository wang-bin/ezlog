
QT       =

TEMPLATE = lib
CONFIG += static
DESTDIR = ../lib
TARGET = ezlog

SOURCES += \
	ezlog.cpp \
	ezthread.cpp


HEADERS += \
	ezlog.h \
	ezthread.h \
	global.h \
	eztime.h

win32 {
	OBJECTS_DIR = ../.obj/win32
	SOURCES += ezthread_win.cpp
} else {
	unix:OBJECTS_DIR = ../.obj/unix
	macx:OBJECTS_DIR = ../.obj/macx
	SOURCES += ezthread_posix.cpp
}
