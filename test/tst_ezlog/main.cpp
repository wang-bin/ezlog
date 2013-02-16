/******************************************************************************
	ezlog test
	Copyright (C) 2011 Wangbin <wbsecg1@gmail.com>

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

#include "ezlog.h"

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	ezlog_init_default();
	ezlog_registerAppender(file_appender);
	ezlog_set_appender_with_layout(file_appender, "%mm%: %msg");
	ezlog_set_appender_with_layout(file_appender, "[%level%] %hh%:%mm%:%ss%: %msg");
	ezlog_add_logfile("ezlog.txt", Append);
	ezlog_debug("Bye, cruel world!");
	ezlog(info);
	ezlog_set_global_layout("%tid%: %msg");
    ezlog_debug();
	//for (int i = 0; i < 1024; ++i)
	//	ezlog_debug("%d", i);
	//ezlog_fini();
	return 0;
}

