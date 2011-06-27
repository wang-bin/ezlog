/******************************************************************************
	global.h: Macros
	Copyright (C) 2011 Wang Bin <wbsecg1@gmail.com>

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

#ifndef GLOBAL_H
#define GLOBAL_H


#if __GNUC__
#else
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(WIN32) || defined(WINCE) || defined(_WIN32_WCE) || defined(UNDER_CE)
#define OS_WIN
#endif

#endif // GLOBAL_H
