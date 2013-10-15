TEMPLATE = lib
TARGET = ezlog
QT =
CONFIG += console
CONFIG -= app_bundle

CONFIG *= ezlog-buildlib

#var with '_' can not pass to pri?
STATICLINK = 0
PROJECTROOT = $$PWD/..
!include(libezlog.pri): error("could not find libezlog.pri")
preparePaths($$OUT_PWD/../out)

win32:RC_FILE = $${PROJECTROOT}/res/ezlog.rc
OTHER_FILES += $$RC_FILE


HEADERS += ezlog.h \
	eztime.h \
	ezlog_global.h \
	list.h \
	appender.h \
	layout.h \
	os.h \
	cpu.h \
	ezmutex.h \
	prepost.h \
	private.h


SOURCES += \
	appender.c \
	ezlog.c \
	eztime.c \
	layout.c \
	ezmutex.c \
	private.c

#QMAKE_CXXFLAGS += "-std=c++0x"

#unix: LIBS += -lpthread
!*msvc*:!*android*: LIBS += -lpthread
QMAKE_LFLAGS += -u _deinit_ezlog_fini -u _init_print_version
#lib.files =
#sources.files = ezlog_global.h ezlog.h appender.h layout.h
#sources.path = /usr/local/include/ezlog

#INSTALLS += sources #lib
