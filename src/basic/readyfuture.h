/*
 *   Copyright (C) 2016 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library.
 *   If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ASYNQT_CONS_READY_FUTURE_H
#define ASYNQT_CONS_READY_FUTURE_H

#include <asynqt/asynqt_export.h>

#include <QFuture>
#include <QFutureInterface>
#include <QObject>
#include <QTimer>

#include "../private/basic/readyfuture_p.h"

namespace AsynQt {

/**
 * Creates a future that has already been completed,
 * and that contains the specified value
 */
template <typename _Result>
QFuture<typename std::decay<_Result>::type> makeReadyFuture(_Result &&value)
{
    return detail::makeReadyFuture(std::forward<_Result>(value));
}

/**
 * Creates a void future that has already been completed.
 */
ASYNQT_EXPORT
QFuture<void> makeReadyFuture();

} // namespace AsynQt

#endif // ASYNQT_CONS_READY_FUTURE_H

