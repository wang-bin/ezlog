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

#include "qtezlog.h"

#include <ezlog.h>

unsigned QtEZlog_Version()
{
    return QTEZLOG_VERSION;
}

namespace ezlog {
namespace QtEZLog {

class Initializer
{
public:
    Initializer() {
        ezlog_init_default();
    }
    ~Initializer() {
        ezlog_fini();
    }
};
static Initializer sInitInternal;


#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#define qInstallMessageHandler qInstallMsgHandler
void EZLogHandler(QtMsgType type, const char *msg)
{
#else
void EZLogHandler(QtMsgType type, const QMessageLogContext &ctx, const QString& qmsg)
{
    const char* msg = qPrintable(qmsg);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
     switch (type) {
     case QtDebugMsg:
         _ezlog_print("DEBUG", ctx.file, ctx.line, ctx.function, "%s", msg);
         break;
     case QtWarningMsg:
         _ezlog_print("WARN", ctx.file, ctx.line, ctx.function, "%s", msg);
         //if (QT_FATAL_WARNINGS)
         break;
     case QtCriticalMsg:
         _ezlog_print("ERROR", ctx.file, ctx.line, ctx.function, "%s", msg);
         break;
     case QtFatalMsg:
         _ezlog_print("FATAL", ctx.file, ctx.line, ctx.function, "%s", msg);
         abort();
     }
     fflush(0);
#endif //QT_VERSION
}

void install()
{
    qInstallMessageHandler(EZLogHandler);
}

void uninstall()
{
    qInstallMessageHandler(0);
}

void setLayout(const QString &fmt)
{
    ezlog_set_global_layout(qPrintable(fmt));
}

} //namespace QtEZLog
} //namespace ezlog

#if !WITH_EZLOG
#undef qDebug

void QtEZLogContext(const char* level, const char* file, const int line, const char* func, const char* fmt, ...)
{

}

#endif //WITH_EZLOG
