#-------------------------------------------------
#
# Project created by QtCreator 2011-04-14T23:50:00
#
#-------------------------------------------------
TEMPLATE = subdirs
SUBDIRS = lib test

OTHER_FILES += \
			TODO.txt

win32 {
	INCLUDEPATH += #$VS90COMNTOOLS/../../VC/include
}
