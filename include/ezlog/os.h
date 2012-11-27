/******************************************************************************
	os.h: OS detection. From Qt5 and wtf
	Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>
	
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
******************************************************************************/


#ifndef OS_H
#define OS_H

/*
   The operating system, must be one of: (Q_OS_x)

	 DARWIN   - Darwin OS (synonym for Q_OS_MAC)
	 MSDOS    - MS-DOS and Windows
	 OS2      - OS/2
	 OS2EMX   - XFree86 on OS/2 (not PM)
	 WIN32    - Win32 (Windows 2000/XP/Vista/7 and Windows Server 2003/2008)
	 WINCE    - WinCE (Windows CE 5.0)
	 CYGWIN   - Cygwin
	 SOLARIS  - Sun Solaris
	 HPUX     - HP-UX
	 ULTRIX   - DEC Ultrix
	 LINUX    - Linux
	 FREEBSD  - FreeBSD
	 NETBSD   - NetBSD
	 OPENBSD  - OpenBSD
	 BSDI     - BSD/OS
	 IRIX     - SGI Irix
	 OSF      - HP Tru64 UNIX
	 SCO      - SCO OpenServer 5
	 UNIXWARE - UnixWare 7, Open UNIX 8
	 AIX      - AIX
	 HURD     - GNU Hurd
	 DGUX     - DG/UX
	 RELIANT  - Reliant UNIX
	 DYNIX    - DYNIX/ptx
	 QNX      - QNX
	 QNX6     - QNX RTP 6.1
	 LYNX     - LynxOS
	 BSD4     - Any BSD 4.4 system
	 UNIX     - Any UNIX BSD/SYSV system
	 Android    Android system (unix)
	 iOS        Apple iOS (unix)
	 Symbian    Nokia Symbian (unix)
*/

/* OS(ANDROID) - Android */
#ifdef ANDROID
#define Q_OS_ANDROID 1
#endif

/* OS(HAIKU) - Haiku */
#ifdef __HAIKU__
#define Q_OS_HAIKU 1
#endif

/* OS(SYMBIAN) - Symbian */
#if defined (__SYMBIAN32__)
#define Q_OS_SYMBIAN 1
#endif

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  define Q_OS_DARWIN
#  define Q_OS_BSD4
#  ifdef __LP64__
#    define Q_OS_DARWIN64
#  else
#    define Q_OS_DARWIN32
#  endif
#elif defined(__CYGWIN__)
#  define Q_OS_CYGWIN
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define Q_OS_WIN32
#  define Q_OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  if defined(WINCE) || defined(_WIN32_WCE)
#    define Q_OS_WINCE
#  else
#    define Q_OS_WIN32
#  endif
#elif defined(__sun) || defined(sun)
#  define Q_OS_SOLARIS
#elif defined(hpux) || defined(__hpux)
#  define Q_OS_HPUX
#elif defined(__ultrix) || defined(ultrix)
#  define Q_OS_ULTRIX
#elif defined(sinix)
#  define Q_OS_RELIANT
#elif defined(__native_client__)
#  define Q_OS_NACL
#elif defined(__linux__) || defined(__linux)
#  define Q_OS_LINUX
#elif defined(__FreeBSD__) || defined(__DragonFly__)
#  define Q_OS_FREEBSD
#  define Q_OS_BSD4
#elif defined(__NetBSD__)
#  define Q_OS_NETBSD
#  define Q_OS_BSD4
#elif defined(__OpenBSD__)
#  define Q_OS_OPENBSD
#  define Q_OS_BSD4
#elif defined(__bsdi__)
#  define Q_OS_BSDI
#  define Q_OS_BSD4
#elif defined(__sgi)
#  define Q_OS_IRIX
#elif defined(__osf__)
#  define Q_OS_OSF
#elif defined(_AIX)
#  define Q_OS_AIX
#elif defined(__Lynx__)
#  define Q_OS_LYNX
#elif defined(__GNU__)
#  define Q_OS_HURD
#elif defined(__DGUX__)
#  define Q_OS_DGUX
#elif defined(__QNXNTO__)
#  define Q_OS_QNX
#elif defined(_SEQUENT_)
#  define Q_OS_DYNIX
#elif defined(_SCO_DS) /* SCO OpenServer 5 + GCC */
#  define Q_OS_SCO
#elif defined(__USLC__) /* all SCO platforms + UDK or OUDK */
#  define Q_OS_UNIXWARE
#elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */
#  define Q_OS_UNIXWARE
#elif defined(__INTEGRITY)
#  define Q_OS_INTEGRITY
#elif defined(VXWORKS) /* there is no "real" VxWorks define - this has to be set in the mkspec! */
#  define Q_OS_VXWORKS
#elif defined(__MAKEDEPEND__)
#else
#  error "Qt has not been ported to this OS - see http://www.qt-project.org/"
#endif

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINCE)
#  define Q_OS_WIN
#endif

#if defined(Q_OS_DARWIN)
#include <TargetConditionals.h>
#  if (defined(TARGET_OS_EMBEDDED) && TARGET_OS_EMBEDDED)  \
	  || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE)   \
	  || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
#     define Q_OS_IOS
#  elif defined(TARGET_OS_MAC) && TARGET_OS_MAC
#     define Q_OS_MAC
#     define Q_OS_MACX /* Q_OS_MACX is only for compatibility.*/
#    if defined(Q_OS_DARWIN64)
#        define Q_OS_MAC64
#    elif defined(Q_OS_DARWIN32)
#        define Q_OS_MAC32
#    endif
#  endif
#endif

#if defined(Q_OS_WIN)
#  undef Q_OS_UNIX
#elif !defined(Q_OS_UNIX)
#  define Q_OS_UNIX
#endif

#ifdef Q_OS_DARWIN
#  ifdef MAC_OS_X_VERSION_MIN_REQUIRED
#    undef MAC_OS_X_VERSION_MIN_REQUIRED
#  endif
#  define MAC_OS_X_VERSION_MIN_REQUIRED MAC_OS_X_VERSION_10_4
#  include <AvailabilityMacros.h>
#  if !defined(MAC_OS_X_VERSION_10_3)
#     define MAC_OS_X_VERSION_10_3 MAC_OS_X_VERSION_10_2 + 1
#  endif
#  if !defined(MAC_OS_X_VERSION_10_4)
#       define MAC_OS_X_VERSION_10_4 MAC_OS_X_VERSION_10_3 + 1
#  endif
#  if !defined(MAC_OS_X_VERSION_10_5)
#       define MAC_OS_X_VERSION_10_5 MAC_OS_X_VERSION_10_4 + 1
#  endif
#  if !defined(MAC_OS_X_VERSION_10_6)
#       define MAC_OS_X_VERSION_10_6 MAC_OS_X_VERSION_10_5 + 1
#  endif
#  if !defined(MAC_OS_X_VERSION_10_7)
#       define MAC_OS_X_VERSION_10_7 MAC_OS_X_VERSION_10_6 + 1
#  endif
#  if !defined(MAC_OS_X_VERSION_10_8)
#       define MAC_OS_X_VERSION_10_8 MAC_OS_X_VERSION_10_7 + 1
#  endif
#  if (MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_8)
#    warning "This version of Mac OS X is unsupported"
#  endif
#endif

#ifdef __LSB_VERSION__
#  if __LSB_VERSION__ < 40
#    error "This version of the Linux Standard Base is unsupported"
#  endif
#ifndef QT_LINUXBASE
#  define QT_LINUXBASE
#endif
#endif

#endif // OS_H
