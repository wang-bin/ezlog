#-------------------------------------------------
#
# Project created by QtCreator 2011-04-14T23:50:00
#
#-------------------------------------------------
TEMPLATE = subdirs
SUBDIRS = libezlog test
libezlog.file = src/libezlog.pro
test.depends = libezlog

OTHER_FILES += README TODO.txt .gitignore

win32 {
	INCLUDEPATH += #$VS90COMNTOOLS/../../VC/include
}
