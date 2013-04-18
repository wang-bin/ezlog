/******************************************************************************
    QtEZLog:  a Qt wrapper for ezlog
    Copyright (C) 2012-2013 Wang Bin <wbsecg1@gmail.com>

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

#ifndef QTEZLOG_H
#define QTEZLOG_H

#include "qtezlog_global.h"
#include <QtCore/QString>

/* runtime version. used to compare with compile time version */
Q_EXPORT unsigned QtEZlog_Version();

namespace ezlog {
namespace QtEZLog {

QTEZLOGSHARED_EXPORT void install();
QTEZLOGSHARED_EXPORT void uninstall();
/*set global layout*/
QTEZLOGSHARED_EXPORT void setLayout(const QString& fmt);

/*
 class QTEZLOGSHARED_EXPORT Appender
 {
    setLayout(...);
    install();
    uninstall();
    virtual void write(...);
 }
 */
} //namespace QtEZLog
} //namespace ezlog


#define WITH_EZLOG 1
#ifndef EZLOG_FUNC
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
#endif //EZLOG_FUNC

#if WITH_EZLOG
extern "C" {
void _ezlog_print(const char* level, const char* file, const int line, const char* func, const char* fmt, ...);
}
#define qDebug(fmt, ...) _ezlog_print("DEBUG", __FILE__, __LINE__, EZLOG_FUNC, #fmt, ##__VA_ARGS__)
#define qWarning(fmt, ...) _ezlog_print("DEBUG", __FILE__, __LINE__, EZLOG_FUNC, #fmt, ##__VA_ARGS__)
#define qCritical(fmt, ...) _ezlog_print("DEBUG", __FILE__, __LINE__, EZLOG_FUNC, #fmt, ##__VA_ARGS__)
#define qFatal(fmt, ...) _ezlog_print("DEBUG", __FILE__, __LINE__, EZLOG_FUNC, #fmt, ##__VA_ARGS__)
#else
/* for internal use. call qDebug() etc*/
QTEZLOGSHARED_EXPORT void QtEZLogContext(const char* level, const char* file, const int line, const char* func, const char* fmt, ...);

#define qDebug(fmt, ...) QtEZLogContext("DEBUG", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)
#define qWarning(fmt, ...) QtEZLogContext("WARN", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)
#define qCritical(fmt, ...) QtEZLogContext("ERROR", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)
#define qFatal(fmt, ...) QtEZLogContext("FATAL", __FILE__, __LINE__, EZLOG_FUNC, ""#fmt, ##__VA_ARGS__)

#endif //WITH_EZLOG

#endif // QTEZLOG_H
