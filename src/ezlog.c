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

#include "ezlog.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "private.h"
#include "layout.h"
#include "ezmutex.h"
#include "prepost.h"

extern char* const g_global_layout;
extern struct list_head g_global_printers_head;
extern void __format_msg(char* result_msg, ezlog_context* context);
extern void __log_to_appenders(const char* msg);
extern void __init_printers_with_layout(struct list_head *printers_head, const char *format);
extern void __log_to_appender(appender_t *appender, const char* msg);
extern void __format_msg_with_printers(char *result_msg, ezlog_context *context, struct list_head *printers_head);

typedef struct {
    appender_t *appender;
    struct list_head list;
} appenders_t;

static void print_version() {
	printf("ezlog version: %s\n"
           "Distributed under the terms of LGPLv2.1 or later."
           "Copyright (C) 2011-2013 Wangbin <wbsecg1@gmail.com>  https://github.com/wang-bin/ezlog\n"
		   , ezlog_version_string());
	fflush(stdout);
}
PRE_FUNC_ADD(print_version,)
POST_FUNC_ADD(ezlog_fini,)
//__func__
//strftime()

/*!
 * \fn int ezlog_version()
 * \brief Get ezlog's version
 *
 */
int ezlog_version()
{
	return EZLOG_VERSION;
}

const char* ezlog_version_string()
{
	return k_ezlog_version_string;
}

void ezlog_init_default()
{
	ezlog_init_layout(layout_format[DEFAULT_LAYOUT]);
    ezlog_set_default_appender(console_appender());
}

LIST_HEAD(layout_appenders_map); /*layout_appenders_map_t*/

static void __insert_appender(struct list_head* appenders_head, appender_t *appender)
{
	appenders_t *appender_node = (appenders_t*)malloc(sizeof(appenders_t));
    appender_node->appender = appender;
	list_add_tail(&appender_node->list, appenders_head);
}

void ezlog_set_appender_with_layout(appender_t *appender, const char *format)
{
	struct list_head *layout_pos = &layout_appenders_map;
	struct list_head *appender_pos;
	layout_appenders_map_t *layout_node = 0;
	layout_appenders_map_t *old_layout_node = 0;
	appenders_t *appender_node = 0;
	int layout_exists = 0;
	/*layout already has this appender. Found in the same loop*/
	int layout_appender_exists = 0;
	list_for_each(layout_pos, &layout_appenders_map) {
		layout_appender_exists = 0;
		layout_node = list_entry(layout_pos, layout_appenders_map_t, list);
		if (!strcmp(layout_node->format, format)) {
			printf("===Layout %s found. Use old layout\n", format);
			fflush(0);
			old_layout_node = layout_node;
			layout_exists = 1;
			layout_appender_exists = 1;
			/*DO NOT break. Check whether the appender already exists in a layout_appenders*/
		}
		list_for_each(appender_pos, &layout_node->appenders_head) {
			appender_node = list_entry(appender_pos, appenders_t, list);
			/*old appender found. remove it from list*/
            if (appender_node->appender == appender) {
				printf("===Appender exists...\n");
				if (layout_appender_exists) {
					printf("===The appender's layout is the same as new\n");
					fflush(0);
					return;
				}
				/*Remove from list anyway. The appender will be new*/
				list_del(&appender_node->list);
				//appender_node->list.prev->next = appender_node->list.next;
				//appender_node->list.next->prev = appender_node->list.prev;
				free(appender_node); /*do not free for the last?*/

				/*remove layout_node if it's appenders is empty*/
				if (list_empty(&layout_node->appenders_head)) {
					printf("===appenders for layout [%s] is empty. remove this layout\n"
						   , layout_node->format);
					list_del(&layout_node->list);
					free(layout_node);
				}
				/*old layout and appender, but appender does not has layout*/
				if (layout_exists)
					break;
			} /*appender not found yet*/
		}
	}
	/*now appender is new.*/
	if (layout_exists) { /*printers(layout) exists*/
		printf("===Old layout %s to appender\n", format);
		fflush(0);
        __insert_appender(&old_layout_node->appenders_head, appender);
	} else { /*add new appender to new layout*/
		printf("===New layout %s to appender\n", format);
		fflush(0);
		layout_node = (layout_appenders_map_t*)malloc(sizeof(layout_appenders_map_t));
		layout_node->format = (char*)malloc((strlen(format) + 1) * sizeof(char));
		memset(layout_node->format, 0, strlen(format) + 1);
		memcpy(layout_node->format, format, strlen(format));
		if (format == g_global_layout || !strcmp(format, g_global_layout)) {
			/*g_global_printers_head will be cleaned only once*/
			layout_node->printers_head = &g_global_printers_head;
			/*layout_node->printers_head.next = g_global_printers_head.next;
			layout_node->printers_head.prev = g_global_printers_head.prev;*/
		} else {
			layout_node->printers_head = (struct list_head*)malloc(sizeof(struct list_head));
			INIT_LIST_HEAD(layout_node->printers_head);
			__init_printers_with_layout(layout_node->printers_head, format);
		}
		INIT_LIST_HEAD(&layout_node->appenders_head);
        __insert_appender(&layout_node->appenders_head, appender);
		list_add_tail(&layout_node->list, &layout_appenders_map);
	}
}

static void cleanup_layout_appender_map()
{
	layout_appenders_map_t *layout_node = 0;
	key_print_node *printer_node;
	appenders_t *appender_node = 0;
	/*TODO: cleanup member func for every list type*/
	while (!list_empty(&layout_appenders_map)) {
		layout_node = list_entry(layout_appenders_map.next, layout_appenders_map_t, list);
		printf("Cleaning layout [%s]. printers empty=%d\n", layout_node->format, list_empty(layout_node->printers_head));
		fflush(0);
		while (!list_empty(layout_node->printers_head)) {
			printer_node = list_entry(layout_node->printers_head->next, key_print_node, list);
			list_del(layout_node->printers_head->next);
			free(printer_node);
		}
		while (!list_empty(&layout_node->appenders_head)) {
			appender_node = list_entry(layout_node->appenders_head.next, appenders_t, list);
			printf("	Cleaning appender: %p\n", appender_node);
			fflush(0);
			list_del(layout_node->appenders_head.next);
			free(appender_node);
		}
		list_del(layout_appenders_map.next);
		free(layout_node->format);
		/*if equals to global printers, do not free*/
		/*free(layout_node->printers_head); */
		if (&g_global_printers_head != layout_node->printers_head)
			free(layout_node->printers_head);
		free(layout_node);
	}
}

#if !COMPILER(MSVC) && !COMPILER(MINGW)
#include <unistd.h>
#include <pthread.h>
#endif

static ALWAYS_INLINE unsigned long threadId()
{
#if COMPILER(MSVC) || COMPILER(MINGW)
	return GetCurrentThreadId();
#else
#if defined(OS_WIN) && (__GNUC_MINOR__ < 6)
	return pthread_self().x;
#else
	return (unsigned long)pthread_self();
#endif
#endif
}

static ALWAYS_INLINE long pid()
{
#if COMPILER(MSVC) || COMPILER(MINGW)
	return GetCurrentProcessId();
#else
	return getpid();
#endif
}

//TODO: move to layout.c
void _ezlog_print(const char* level, const char* file, const int line, const char* func, const char* fmt, ...)
{
	struct list_head *layout_pos = &layout_appenders_map;
	struct list_head *appender_pos;
	layout_appenders_map_t *layout_node = 0;
	appenders_t *appender_node = 0;
	static char result_msg[1024];
	ezlog_context context; //static
	eztime t;
	char msg[512];
	va_list args;
	_ezmutex_lock();
	va_start(args, fmt);
	vsprintf(msg, fmt, args);
	va_end(args);
	//r += sprintf(msg + r, "\n");

	getTime(&t);
	context.level = level;
	context.file = file;
	context.func = func;
	context.line = line;
	context.t = &t;
	context.pid = pid();
	context.tid = threadId();
	context.msg = msg;


#if 0
	memset(result_msg, 0, sizeof(result_msg));
	__format_msg(result_msg, &context);
	__log_to_appenders(result_msg);
#else
	list_for_each(layout_pos, &layout_appenders_map) {
		layout_node = list_entry(layout_pos, layout_appenders_map_t, list);
		memset(result_msg, 0, sizeof(result_msg));
		__format_msg_with_printers(result_msg, &context, layout_node->printers_head);
		/*TODO: format msg with layout_node->printers*/
		list_for_each(appender_pos, &layout_node->appenders_head) {
			appender_node = list_entry(appender_pos, appenders_t, list);
            __log_to_appender(appender_node->appender, result_msg);
		}
	}
#endif
	_ezmutex_unlock();
}

void ezlog_fini() {
	ezlog_unregister_appenders();
	cleanup_layout_appender_map();
}
