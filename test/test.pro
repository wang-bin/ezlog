TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

DESTDIR = ./
TARGET	= test
INCLUDEPATH += ../lib
LIBS += -L../lib -lezlog

SOURCES += main.cpp
