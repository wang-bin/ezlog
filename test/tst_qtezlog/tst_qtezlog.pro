TEMPLATE = app
QT -= gui
CONFIG   += console
#DEFINES += NO_EZLOG
TARGET = tst_qtezlog
STATICLINK = 0
PROJECTROOT = $$PWD/../..
include($$PROJECTROOT/wrappers/QtEZLog/libezlog-qt.pri)
preparePaths($$OUT_PWD/../../out)

SOURCES += main.cpp
