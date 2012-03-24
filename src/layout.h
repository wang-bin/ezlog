/******************************************************************************
	layout.h: description
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


#ifndef LAYOUT_H
#define LAYOUT_H

#include "ezlog_global.h"

enum {
	DEFAULT_LAYOUT = 0
};

const char* const layout_format[] = {
	"YY%-%MM%-%DD% %hh%:%mm%:%ss% ~tid:%tid% pid:%pid%~[%file%] %func% @%line%: %msg"
};

Q_EXPORT void ezlog_init_layout(const char* format); //move to layout.h
//char* ezlog_layout_msg(const char* msg_extra);

#endif // LAYOUT_H
