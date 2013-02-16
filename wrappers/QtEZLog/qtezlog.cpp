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
void EZLogHandler(QtMsgType type, const QMessageLogContext &, const QString& qmsg)
{
    const char* msg = qPrintable(qmsg);
#endif
     switch (type) {
     case QtDebugMsg:
         ezlog_debug("%s", msg);
         break;
     case QtWarningMsg:
         ezlog_warn("%s", msg);
         //if (QT_FATAL_WARNINGS)
         break;
     case QtCriticalMsg:
         ezlog_error("%s", msg);
         break;
     case QtFatalMsg:
         ezlog_fatal("%s", msg);
         abort();
     }
     fflush(0);
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

} //namespace ezlog
} //namespace QtEZLog

