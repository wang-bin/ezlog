#include "ezlog.h"
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#ifdef WINCE
#include <windows.h>
#endif //WINCE

static FILE* _out;
static int log_mode;
//__func__
//strftime()

int _ezlog_print(FILE* out, const char* file, const int line, const char* func, const char* fmt, ...)
{
	int r=0;

	/* use the static pointer if out==NULL, or use out if it is stderr or stdout*/
	if(!out) out = _out;
#ifdef WINCE
	SYSTEMTIME stUTC;
	::GetSystemTime(&stUTC);
#else
	time_t seconds = time(NULL);
	struct tm* t = localtime(&seconds);
#endif //WINCE
	va_list args;
	va_start(args, fmt);

	r += fprintf(out, "%04d-%02d-%02d %02d:%02d:%02d-[%s] %s @%d: \t"
#ifdef WINCE
	, stUTC.wYear, stUTC.wMonth, stUTC.wDay, stUTC.wHour, stUTC.wMinute, stUTC.wSecond//, stUTC.wMilliseconds,stUTC.wDayOfWeek
#else
	, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec
#endif //WINCE
	, file, func, line);
	r += vfprintf(out, fmt, args);
	r += fprintf(out, "\n");
	if(out!=_out) {
		fprintf(_out, "%04d-%02d-%02d %02d:%02d:%02d-[%s] %s @%d: \t"
		#ifdef WINCE
			, stUTC.wYear, stUTC.wMonth, stUTC.wDay, stUTC.wHour, stUTC.wMinute, stUTC.wSecond//, stUTC.wMilliseconds,stUTC.wDayOfWeek
		#else
			, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec
		#endif //WINCE
			, file, func, line);
		vfprintf(_out, fmt, args);
		fprintf(_out, "\n");
	}
	va_end(args);
	//fflush(out);
	fflush(NULL);

	return r;
}

//init_stdout, init_stderr, init_file
int ezlog_init_output(const char* path, LogOpenMode mode) {
	if(!strcmp(path, "stdout")) {
		_out = stdout;
		return 0;
	} else if(!strcmp(path, "stderr")) {
		_out = stderr;
		return 0;
	} else {
		const char* m = "ab";
		if(mode==New) m = "wb";

		_out = fopen(path, m);
		/*if(!_out) {
			//perror("open log file failed!"); //wince does not support
			return 1;
		}*/
		//perror, errno
		return _out==NULL;
	}
}

void ezlog_fini() {
	if(_out!=stdout &&_out!=stderr && _out!=NULL) {
		fclose(_out);
		_out = NULL;
	}
}
