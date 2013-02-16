#-------------------------------------------------
#
# Project created by QtCreator 2011-04-14T23:50:00
#
#-------------------------------------------------
TEMPLATE = subdirs
SUBDIRS = libezlog wrappers test
libezlog.file = src/libezlog.pro
test.depends = libezlog
wrappers.depends = libezlog

OTHER_FILES += README TODO.txt .gitignore

win32 {
	INCLUDEPATH += #$VS90COMNTOOLS/../../VC/include
}


#use the following lines when building as a sub-project, write cache to this project src dir.
#if build this project alone and do not have sub-project depends on this lib, those lines are not necessary
lessThan(QT_MAJOR_VERSION, 5):include(common.pri)
build_dir = "BUILD_DIR=$$OUT_PWD"
write_file($$PWD/.build.cache, build_dir)
