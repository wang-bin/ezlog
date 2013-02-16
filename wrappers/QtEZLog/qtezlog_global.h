/******************************************************************************
    QtEZLog:  a Qt wrapper for ezlog
    Copyright (C) 2012-2013 Wang Bin <wbsecg1@gmail.com>

*   This file is part of ezlog

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/

#ifndef QTEZLOG_GLOBAL_H
#define QTEZLOG_GLOBAL_H

#include <qglobal.h>


#define QTEZLOG_MAJOR 1	//((QTEZLOG_VERSION&0xff0000)>>16)
#define QTEZLOG_MINOR 4	//((QTEZLOG_VERSION&0xff00)>>8)
#define QTEZLOG_PATCH 0	//(QTEZLOG_VERSION&0xff)


#define QTEZLOG_VERSION_MAJOR(V) ((V & 0xff0000) >> 16)
#define QTEZLOG_VERSION_MINOR(V) ((V & 0xff00) >> 8)
#define QTEZLOG_VERSION_PATCH(V) (V & 0xff)

#define QTEZLOG_VERSION_CHK(major, minor, patch) \
    (((major&0xff)<<16) | ((minor&0xff)<<8) | (patch&0xff))

#define QTEZLOG_VERSION QTEZLOG_VERSION_CHK(QTEZLOG_MAJOR, QTEZLOG_MINOR, QTEZLOG_PATCH)

/*! Stringify \a x. */
#define _TOSTR(x)   #x
/*! Stringify \a x, perform macro expansion. */
#define TOSTR(x)  _TOSTR(x)


/* the following are compile time version */
/* C++11 requires a space between literal and identifier */
static const char* const qtezlog_version_string = TOSTR(QTEZLOG_MAJOR) "." TOSTR(QTEZLOG_MINOR) "." TOSTR(QTEZLOG_PATCH) "(" __DATE__ ", " __TIME__ ")";
#define QTEZLOG_VERSION_STR         TOSTR(QTEZLOG_MAJOR) "." TOSTR(QTEZLOG_MINOR) "." TOSTR(QTEZLOG_PATCH)
#define QTEZLOG_VERSION_STR_LONG	QTEZLOG_VERSION_STR "(" __DATE__ ", " __TIME__ ")"


#if defined(Q_DLL_LIBRARY)
#  undef Q_EXPORT
#  define Q_EXPORT Q_DECL_EXPORT
#else
#  undef Q_EXPORT
#  define Q_EXPORT //Q_DECL_IMPORT //only for vc?
#endif

#if defined(QTEZLOG_LIBRARY)
#  define QTEZLOGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTEZLOGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTEZLOG_GLOBAL_H
