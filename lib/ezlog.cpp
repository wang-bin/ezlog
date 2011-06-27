#include "ezlog.h"
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ezthread.h"

#ifdef OS_WIN
#include <windows.h>
#endif //WINCE

//const char* log_file; //replace File* _out, close when finish current log
static FILE* _out;
static int log_mode;
static int format_strlen = 0;
static char* format_str = NULL;

ezmutex mutex;
//__func__
//strftime()

typedef struct _eztime {
	int year, month, day;
	int hour, min, sec;
	int msec;
	void init(){
#ifdef OS_WIN
		SYSTEMTIME stUTC;
		::GetSystemTime(&stUTC);
		year = stUTC.wYear, month = stUTC.wMonth, day = stUTC.wDay;
		hour = stUTC.wHour, min = stUTC.wMinute, sec = stUTC.wSecond;
		msec = stUTC.wMilliseconds;
#else
		time_t seconds = time(NULL);
		struct tm* t = localtime(&seconds);
		year = t->tm_year + 1900, month = t->tm_mon + 1, day = t->tm_mday;
		hour = t->tm_hour, min = t->tm_min, sec = t->tm_sec, msec = 0;
#endif //WINCE
	}
	_eztime() { init();}
} eztime;

typedef	int (*ezfprintf)(const struct tm*);//(FILE*, const char*, ...);
const char* fomat_list[] = {
	"%04d-%02d-%02d %02d:%02d:%02d [tid:0x%lx pid:0x%lx]-[%s] %s @%d: \t"//, geit_time}//,fprintf}//(stdout, "%04d-%02d-%02d %02d:%02d:%02d-", geitTime()->tm_year + 1900, geitTime()->tm_mon + 1, geitTime()->tm_mday, geitTime()->tm_hour, geitTime()->tm_min, geitTime()->tm_sec)}
};

const char* formats[] = {
	"YY%-%MM%-%DD% %hh%:%mm%:%ss% [tid:%tid% pid:%pid%]-[%file%] %func% @%line%: %"
};
/*
	keywords {
		const char* key;
		void (*print)(const char*);
	}
*/
/*!
	va_list: fmt
	format_print() will not change the source string. Parameter str MUST BE END WITH '%'!
	The reason is in strtok(), but i don't know it now :(
*/
int format_print(FILE* f, char* str, const eztime& t, const char* file, const char* func, int line)
{
	ezscoped_lock lock(mutex);
	int r = 0;
	char *pch = strtok (str,"%");
	while (pch != NULL) {
		//printf ("%s\n",pch);
		if(!strcmp(pch, "YY"))
			r += fprintf(f, "%d", t.year);
		else if(!strcmp(pch, "MM"))
			r += fprintf(f, "%d", t.month);
		else if(!strcmp(pch, "DD"))
			r += fprintf(f, "%d", t.day);
		else if(!strcmp(pch, "hh"))
			r += fprintf(f, "%d", t.hour);
		else if(!strcmp(pch, "mm"))
			r += fprintf(f, "%d", t.min);
		else if(!strcmp(pch, "ss"))
			r += fprintf(f, "%d", t.sec);
		else if(!strcmp(pch, "ms"))
			r += fprintf(f, "%d", t.msec);
		else if(!strcmp(pch, "file"))
			r += fprintf(f, "%s", file);
		else if(!strcmp(pch, "func"))
			r += fprintf(f, "%s", func);
		else if(!strcmp(pch, "line"))
			r += fprintf(f, "%d", line);
		else if(!strcmp(pch, "tid"))
			r += fprintf(f, "0x%lx", threadId());
		else if(!strcmp(pch, "pid"))
			r += fprintf(f, "0x%lx", pid());
		else
			r += fprintf(f, "%s", pch);
		*(pch+strlen(pch)) = '%'; //strtok will replace '%' with '\0'.
		pch = strtok (NULL, "%"); //why NULL?
	}
	fflush(f);
	return r;
}

int _ezlog_print(FILE* out, const char* file, const int line, const char* func, const char* fmt, ...)
{
	int r=0;
	/* use the static pointer if out==NULL, or use out if it is stderr or stdout*/
	if(!out) out = _out;
	if(!out) return 0; //Do nothing if log file is not set

	va_list args;
	va_start(args, fmt);

	eztime t;

	//r += fprintf(out, fomat_list[0],
	//	t.year, t.month, t.day, t.hour, t.min, t.sec
	//, threadId(), pid(), file, func, line);
	if (format_str==NULL) ezlog_init_format(formats[0]);
	format_print(out, format_str, t, file, func, line);
	r += vfprintf(out, fmt, args);
	r += fprintf(out, "\n");
	if(out!=_out && _out!=NULL) {
		//fprintf(_out, fomat_list[0],
		//	t.year, t.month, t.day, t.hour, t.min, t.sec
		//	, threadId(), pid(), file, func, line);
		format_print(_out, format_str, t, file, func, line);
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
	ezscoped_lock lock(mutex);
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
		if(!_out) {
			//perror("open log file failed!"); //wince does not support
			return 1;
		}
		return _out==NULL;
	}
}

void ezlog_init_format(const char *format)
{
	ezscoped_lock lock(mutex);
	if(format_str!=NULL) {
		free(format_str);
		format_str = NULL;
	}
	format_strlen = strlen(format);
	if(format[format_strlen]!='%') {
		format_strlen += 1;
	}
	format_str = (char*)malloc(format_strlen+1);
	memset(format_str, 0, format_strlen+1);
	memcpy(format_str, format, strlen(format));
	/*
		force end with '%'. If already is, no effect. If not, it will be '%'
	*/
	*(format_str+format_strlen-1) = '%';
}

void ezlog_fini() {
	ezscoped_lock lock(mutex);
	if(_out!=stdout &&_out!=stderr && _out!=NULL) {
		fclose(_out);
		_out = NULL;
	}
	if (format_str!=NULL) {
		free(format_str);
		format_str = NULL;
	}
}
