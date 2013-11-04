TEMPLATE = app
QT =
CONFIG   += console
#DEFINES += NO_EZLOG
TARGET = tst_ezlog
STATICLINK = 0
PROJECTROOT = $$PWD/../..
include($$PROJECTROOT/src/libezlog.pri)
preparePaths($$OUT_PWD/../../out)

SOURCES += main.c
