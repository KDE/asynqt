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

#ifndef ASYNQT_PRIVATE_UTILS_P_H
#define ASYNQT_PRIVATE_UTILS_P_H

namespace AsynQt {
namespace detail {

#define IMPLEMENT_FUTURE_WATCHER_HANDLER(SignalName, EventName)                \
    template <typename T, typename Function>                                   \
    inline void EventName(const std::unique_ptr<QFutureWatcher<T>> &watcher,   \
                          Function function)                                   \
    {                                                                          \
        QObject::connect(watcher.get(), &QFutureWatcherBase::SignalName,       \
                         function);                                            \
    }

IMPLEMENT_FUTURE_WATCHER_HANDLER(finished, onFinished)
IMPLEMENT_FUTURE_WATCHER_HANDLER(canceled, onCanceled)
IMPLEMENT_FUTURE_WATCHER_HANDLER(resultReadyAt, onResultReadyAt)

template <typename Function>
inline void
onResultReadyAt(const std::unique_ptr<QFutureWatcher<void>> &watcher,
                Function function)
{
    Q_UNUSED(watcher)
    Q_UNUSED(function)
}

#undef IMPLEMENT_FUTURE_WATCHER_HANDLER

} // namespace detail
} // namespace AsynQt

#endif // ASYNQT_PRIVATE_UTILS_P_H

