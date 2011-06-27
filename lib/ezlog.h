#pragma once

#include <stdio.h>

#if __GNUC__
#else
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(WIN32) || defined(WINCE) || defined(_WIN32_WCE) || defined(UNDER_CE)
#define OS_WIN
#endif

typedef enum {
	Append, New
} LogOpenMode;

/*!
	fmt"" : than we can write the macro without param, but ezlog_msg() will put a warning while ezlog_msg( ) will not
	a better way is use macro #, ""#fmt
	'#' is followed by a macro parameter
*/

/*
  If path is empty, then just output log messages to stdout or stderr or do nothing. Otherwise, record log to a log file at the same time.
*/
int ezlog_init_output(const char* path, LogOpenMode mode);
void ezlog_init_format(const char* format);

#define ezlog_msg(fmt, ...) _ezlog_print(stdout, __FILE__, __LINE__, __PRETTY_FUNCTION__, ""#fmt, ##__VA_ARGS__)
#define ezlog_error(fmt, ...) _ezlog_print(stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__, ""#fmt, ##__VA_ARGS__)
#define ezlog_log(fmt, ...) _ezlog_print(0, __FILE__, __LINE__, __PRETTY_FUNCTION__, ""#fmt, ##__VA_ARGS__)
//DO NOT use (fmt, args...), MSVC does not support it. use (fmt, ...)
//#define ezlog_msg(fmt,args...) fprintf(stdout,"[%s] %s @%d: \t"fmt"\n",__FILE__,__PRETTY_FUNCTION__,__LINE__,## args); fflush(stdout)
//#define ezlog_error(fmt,args...) fprintf(stderr,"[%s] %s @%d: \t"fmt"\n",__FILE__,__PRETTY_FUNCTION__,__LINE__,## args); fflush(stderr)

void ezlog_fini();

/*
  out is stdout, stderr or 0. Each will put log message to a log file if exists.
*/
int _ezlog_print(FILE* out, const char* file, const int line, const char* func, const char* fmt, ...);
