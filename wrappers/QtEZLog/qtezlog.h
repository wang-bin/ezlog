/******************************************************************************
    QtEZLog:  an ezlog wrapper for Qt logging
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
#endif // QTEZLOG_H
