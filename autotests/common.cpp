/*
 *   Copyright (C) 2016 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "common.h"

namespace common {

QByteArray _helloKdeMessage = "Hello KDE!\n";

QByteArray helloKdeMessage()
{
    return _helloKdeMessage;
}

QFuture<QByteArray> execHelloKde()
{
    return Process::getOutput("echo", { _helloKdeMessage.trimmed() });
}

QFuture<QByteArray> execEcho(const QString &message)
{
    return Process::getOutput("echo", { message.trimmed() });
}

QFuture<QString> fail(const QString &message)
{
    Q_UNUSED(message)
    return makeCanceledFuture<QString>();
}

} // namespace common

