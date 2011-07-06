TEMPLATE = app
Qt		 -= gui
CONFIG   += console static
CONFIG   -= app_bundle

DESTDIR = ./
TARGET	= test
INCLUDEPATH += ../src
LIBS += -L../lib -lezlog

SOURCES += main.cpp
