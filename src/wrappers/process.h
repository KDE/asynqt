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

#ifndef ASYNQT_CONS_PROCESS_H
#define ASYNQT_CONS_PROCESS_H

#include <QProcess>
#include <QFuture>
#include <QFutureInterface>

#include <asynqt/asynqt_export.h>

#include "../private/wrappers/process_p.h"

namespace AsynQt {

template <typename _Function>
auto makeFuture(QProcess *process, _Function map)
        -> QFuture<decltype(map(Q_NULLPTR))>
{
    using namespace detail;

    auto futureInterface =
        new ProcessFutureInterface<decltype(map(Q_NULLPTR)), _Function>
            (process, map);

    return futureInterface->start();
}

namespace Process {

    template <typename _Function>
    auto exec(const QString &command, const QStringList &arguments,
                          _Function &&map)
        -> QFuture<decltype(map(Q_NULLPTR))>
    {
        // TODO: Where to delete this process?
        auto process = new QProcess();

        // process->setProcessChannelMode(QProcess::ForwardedChannels);
        process->setProgram(command);
        process->setArguments(arguments);

        return AsynQt::makeFuture(process, std::forward<_Function>(map));
    }

    ASYNQT_EXPORT
    QFuture<QProcess *> exec(const QString &command,
                             const QStringList &arguments);

    ASYNQT_EXPORT
    QFuture<QByteArray> getOutput(const QString &command,
                                  const QStringList &arguments);

} // namespace Process

} // namespace qfuture

#endif // ASYNQT_CONS_PROCESS_H

