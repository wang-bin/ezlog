/******************************************************************************
    private.h: private types and functions
    Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/


#ifndef EZLOG_PRIVATE_H
#define EZLOG_PRIVATE_H

#include "eztime.h"
#include "list.h"

typedef struct {
    const char* level;
    const char* file;
    const char* func;
    int line;
    const eztime* t;
    unsigned long tid;
    long pid;
    const char* msg;
} ezlog_context;

typedef int (*print_call)(char*, ezlog_context* context);
typedef struct
{
    const char* key;
    char* delimiter; //0: if key != 0; a string between % and % if key == 0
    print_call print;
} key_print;

typedef struct {
    const key_print *printer;
    struct list_head list;
} key_print_node; /*TODO: printer_t*/


typedef void (*appender_t)(const char* msg);
typedef struct {
    appender_t handle;
    struct list_head list;
} appenders_t;

typedef struct {
    char *format;
    /*key_print_node *printers;*/
    struct list_head* printers_head; /*We need a pointer to global layout*/
    /*appenders_t *appenders;*/
    struct list_head appenders_head; /*MUST LIST_INIT this because we dynamically create this node*/
    struct list_head list;
} layout_appenders_map_t;



#endif // EZLOG_PRIVATE_H
    
