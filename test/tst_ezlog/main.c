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
/******************************************************************************
	ezlog test
******************************************************************************/

#include "ezlog.h"

int main(int argc, char** argv)
{
	appender_t *filelog;
	(void)argc;
	(void)argv;
	ezlog_init_default();
    filelog = file_appender(0, New);
	ezlog_register_appender(filelog);
	ezlog_set_appender_with_layout(ezlog_get_default_appender(), "%mm%: %msg");
	ezlog_set_appender_with_layout(filelog, "[%level%] %hh%:%mm%:%ss%: %msg");
	ezlog_debug("Bye, cruel world!");
	ezlog(info, );
	ezlog_set_global_layout("%tid%: %msg");
    ezlog_debug("global layout: %%tid%%: %%msg");

	//for (int i = 0; i < 1024; ++i)
	//	ezlog_debug("%d", i);
	//ezlog_fini();
	return 0;
}

