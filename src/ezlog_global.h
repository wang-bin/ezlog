/******************************************************************************
    ezlog:  a tiny any flexible log system written in C
    Copyright (C) 2011-2013 Wang Bin <wbsecg1@gmail.com>

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

#ifndef EZLOG_GLOBAL_H
#define EZLOG_GLOBAL_H


#undef EZLOG_VERSION //0x010300

#define EZLOG_VERSION_MAJOR 1	//((EZLOG_VERSION&0xff0000)>>16)
#define EZLOG_VERSION_MINOR 4	//((EZLOG_VERSION&0xff00)>>8)
#define EZLOG_VERSION_PATCH 0	//(EZLOG_VERSION&0xff)

#define VERSION_CHK(major, minor, patch) \
	(((major&0xff)<<16) | ((minor&0xff)<<8) | (patch&0xff))

#define EZLOG_VERSION VERSION_CHK(EZLOG_VERSION_MAJOR, EZLOG_VERSION_MINOR, EZLOG_VERSION_PATCH)

/*! Stringify \a x. */
#define _TOSTR(x)   #x
/*! Stringify \a x, perform macro expansion. */
#define TOSTR(x)  _TOSTR(x)

static const char* const k_ezlog_version_string = TOSTR(EZLOG_VERSION_MAJOR) "." TOSTR(EZLOG_VERSION_MINOR) "." TOSTR(EZLOG_VERSION_PATCH) "(" __DATE__ ", " __TIME__ ")";
#define EZLOG_VERSION_STR			TOSTR(EZLOG_VERSION_MAJOR) "." TOSTR(EZLOG_VERSION_MINOR) "." TOSTR(EZLOG_VERSION_PATCH)
#define EZLOG_VERSION_STR_LONG	EZLOG_VERSION_STR "(" __DATE__ ", " __TIME__ ")"

#if defined(__GNUC__)
#  define EZLOG_FUNC __PRETTY_FUNCTION__
#else
#  define __attribute__(...)
#   if defined(_MSC_VER)
#      define EZLOG_FUNC __FUNCSIG__
#   else
#      define EZLOG_FUNC __FUNCTION__
#   endif
#endif

#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(WIN32) || defined(WINCE) || defined(_WIN32_WCE) || defined(UNDER_CE)
#define OS_WIN
#else
#define OS_POSIX
#endif

#define STD_THREAD 0


/*
 *EZLOG_EXPORT: steal from qglobal.h
*/
#ifdef ANDROID
#define Q_OS_ANDROID
#define Q_OS_LINUX
#endif

#if defined(__ARMCC__) || defined(__CC_ARM)
#  define Q_CC_RVCT
#endif
#if defined(MSDOS) || defined(_MSDOS)
#  define Q_OS_MSDOS
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define Q_OS_WIN32
#  define Q_OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  if defined(WINCE) || defined(_WIN32_WCE)
#	define Q_OS_WINCE
#  else
#	define Q_OS_WIN32
#  endif
#elif defined(__MWERKS__) && defined(__INTEL__)
#  define Q_OS_WIN32
#elif defined(hpux) || defined(__hpux)
#  define Q_OS_HPUX
#elif defined(__linux__) || defined(__linux)
#  define Q_OS_LINUX
#endif

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINCE)
#  define Q_OS_WIN
#endif

/* VxWorks' DIAB toolchain has an additional EDG type C++ compiler
   (see __DCC__ above). This one is for C mode files (__EDG is not defined) */
#if defined(__SUNPRO_CC) || defined(__SUNPRO_C)
/* 5.0 compiler or better
	'bool' is enabled by default but can be disabled using -features=nobool
	in which case _BOOL is not defined
		this is the default in 4.2 compatibility mode triggered by -compat=4 */
#  if __SUNPRO_CC >= 0x500
#	define QT_NO_TEMPLATE_TEMPLATE_PARAMETERS
   /* see http://developers.sun.com/sunstudio/support/Ccompare.html */
#	if __SUNPRO_CC >= 0x590
#	  define Q_ALIGNOF(type)   __alignof__(type)
#	  define Q_TYPEOF(expr)	__typeof__(expr)
#	  define Q_DECL_ALIGN(n)   __attribute__((__aligned__(n)))
#	endif
#	if __SUNPRO_CC >= 0x550
#	  define Q_DECL_EXPORT	 __global
#	endif
#  endif

/* CDS++ does not seem to define __EDG__ or __EDG according to Reliant
   documentation but nevertheless uses EDG conventions like _BOOL */
#elif defined(Q_OS_HPUX)
/* __HP_aCC was not defined in first aCC releases */
#  if defined(__HP_aCC) || __cplusplus >= 199707L
#	if __HP_aCC-0 < 060000
#	  define Q_DECL_EXPORT	 __declspec(dllexport)
#	  define Q_DECL_IMPORT	 __declspec(dllimport)
#	endif
#	if __HP_aCC-0 >= 061200
#	  define Q_DECL_ALIGN(n) __attribute__((aligned(n)))
#	endif
#	if __HP_aCC-0 >= 062000
#	  define Q_DECL_EXPORT	 __attribute__((visibility("default")))
#	  define Q_DECL_HIDDEN	 __attribute__((visibility("hidden")))
#	  define Q_DECL_IMPORT	 Q_DECL_EXPORT
#	endif
#  endif

#elif defined(__WINSCW__) && !defined(Q_CC_NOKIAX86)
#  define Q_CC_NOKIAX86
#endif

#if defined(Q_OS_LINUX) && defined(Q_CC_RVCT)
#  define Q_DECL_EXPORT	 __attribute__((visibility("default")))
#  define Q_DECL_IMPORT	 __attribute__((visibility("default")))
#  define Q_DECL_HIDDEN	 __attribute__((visibility("hidden")))
#endif

#ifndef Q_DECL_EXPORT
#  if defined(Q_OS_WIN) || defined(Q_CC_NOKIAX86) || defined(Q_CC_RVCT)
#	define Q_DECL_EXPORT __declspec(dllexport)
#  elif defined(QT_VISIBILITY_AVAILABLE)
#	define Q_DECL_EXPORT __attribute__((visibility("default")))
#	define Q_DECL_HIDDEN __attribute__((visibility("hidden")))
#  endif
#  ifndef Q_DECL_EXPORT
#	define Q_DECL_EXPORT
#  endif
#endif
#ifndef Q_DECL_IMPORT
#  if defined(Q_OS_WIN) || defined(Q_CC_NOKIAX86) || defined(Q_CC_RVCT)
#	define Q_DECL_IMPORT __declspec(dllimport)
#  else
#	define Q_DECL_IMPORT
#  endif
#endif
#ifndef Q_DECL_HIDDEN
#  define Q_DECL_HIDDEN
#endif


#if defined(Q_DLL_LIBRARY)
#  undef EZLOG_EXPORT
#  define EZLOG_EXPORT Q_DECL_EXPORT
#else
#  undef EZLOG_EXPORT
#  define EZLOG_EXPORT //Q_DECL_IMPORT //only for vc?
#endif




/***from webkit/JavascriptCore/wtf***/
#define COMPILER(WTF_FEATURE) (defined WTF_COMPILER_##WTF_FEATURE  && WTF_COMPILER_##WTF_FEATURE)

/* COMPILER(MSVC) Microsoft Visual C++ */
/* COMPILER(MSVC7_OR_LOWER) Microsoft Visual C++ 2003 or lower*/
/* COMPILER(MSVC9_OR_LOWER) Microsoft Visual C++ 2008 or lower*/
#if defined(_MSC_VER)
#define WTF_COMPILER_MSVC 1
#if _MSC_VER < 1400
#define WTF_COMPILER_MSVC7_OR_LOWER 1
#elif _MSC_VER < 1600
#define WTF_COMPILER_MSVC9_OR_LOWER 1
#endif
#endif

/* COMPILER(GCC) - GNU Compiler Collection */
/* --gnu option of the RVCT compiler also defines __GNUC__ */
#if defined(__GNUC__) && !COMPILER(RVCT)
#define WTF_COMPILER_GCC 1
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#define GCC_VERSION_AT_LEAST(major, minor, patch) (GCC_VERSION >= (major * 10000 + minor * 100 + patch))
#else
/* Define this for !GCC compilers, just so we can write things like GCC_VERSION_AT_LEAST(4, 1, 0). */
#define GCC_VERSION_AT_LEAST(major, minor, patch) 0
#endif


/* COMPILER(RVCT)  - ARM RealView Compilation Tools */
/* COMPILER(RVCT4_OR_GREATER) - ARM RealView Compilation Tools 4.0 or greater */
#if defined(__CC_ARM) || defined(__ARMCC__)
#define WTF_COMPILER_RVCT 1
#define RVCT_VERSION_AT_LEAST(major, minor, patch, build) (__ARMCC_VERSION >= (major * 100000 + minor * 10000 + patch * 1000 + build))
#else
/* Define this for !RVCT compilers, just so we can write things like RVCT_VERSION_AT_LEAST(3, 0, 0, 0). */
#define RVCT_VERSION_AT_LEAST(major, minor, patch, build) 0
#endif

/* COMPILER(MINGW) - MinGW GCC */
/* COMPILER(MINGW64) - mingw-w64 GCC - only used as additional check to exclude mingw.org specific functions */
#if defined(__MINGW32__)
#define WTF_COMPILER_MINGW 1
#endif /* __MINGW32__ */

#ifdef __GNUC__
#    define AV_GCC_VERSION_AT_LEAST(x,y) (__GNUC__ > x || __GNUC__ == x && __GNUC_MINOR__ >= y)
#else
#    define AV_GCC_VERSION_AT_LEAST(x,y) 0
#endif


#ifndef ALWAYS_INLINE
#if COMPILER(GCC) && AV_GCC_VERSION_AT_LEAST(3,1) && !COMPILER(MINGW)
#define ALWAYS_INLINE inline __attribute__((__always_inline__))
#elif (COMPILER(MSVC) || COMPILER(RVCT))
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE inline
#endif
#endif //ALWAYS_INLINE

#ifndef NEVER_INLINE
#if COMPILER(GCC) && AV_GCC_VERSION_AT_LEAST(3,1)
#define NEVER_INLINE __attribute__((__noinline__))
#elif COMPILER(RVCT)
#define NEVER_INLINE __declspec(noinline)
#else
#define NEVER_INLINE
#endif
#endif //NEVER_INLINE

#ifndef UNLIKELY
#if COMPILER(GCC) || (RVCT_VERSION_AT_LEAST(3, 0, 0, 0) && defined(__GNUC__))
#define UNLIKELY(x) __builtin_expect((x), 0)
#else
#define UNLIKELY(x) (x)
#endif
#endif //UNLIKELY

#ifndef LIKELY
#if COMPILER(GCC) || (RVCT_VERSION_AT_LEAST(3, 0, 0, 0) && defined(__GNUC__))
#define LIKELY(x) __builtin_expect((x), 1)
#else
#define LIKELY(x) (x)
#endif
#endif //LIKELY

#endif // EZLOG_GLOBAL_H

