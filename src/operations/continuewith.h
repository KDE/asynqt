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

#ifndef ASYNQT_BASE_CONTINUEWITH_H
#define ASYNQT_BASE_CONTINUEWITH_H

#include <QFuture>
#include <QFutureWatcher>

#include <type_traits>
#include <memory>

#include "flatten.h"
#include "transform.h"

namespace AsynQt {

/**
 * This method is similar to `transform`,
 * but it takes a transformation function (in this
 * case, called a continuation) that does not
 * return a normal value, but also a new future.
 * It returns the future that will be returned
 * by the continuation.
 *
 * It is equivalent to calling `flatten` on the result
 * of the `transform` function when a continuation
 * is passed to it.
 *
 * Example:
 *
 * <code>
 *     QFuture<QString> input = getUserInput();
 *     QFuture<Status> continueWith(input, [] (QString message) {
 *          return server.send(message);
 *     });
 * </code>
 *
 * @arg future the future to connect the continuation to
 * @arg continuation the continuation function
 * @returns the future that the continuation will return
 */
template <typename _In, typename _Continuation>
auto continueWith(const QFuture<_In> &future, _Continuation &&continuation)
    -> decltype(flatten(transform(future, std::forward<_Continuation>(continuation))))
{
    return flatten(transform(future, std::forward<_Continuation>(continuation)));
}

namespace operators {

    template <typename _In, typename _Continuation>
    auto operator | (const QFuture<_In> &future, _Continuation &&continuation)
        -> decltype(continueWith(future, continuation))
    {
        return continueWith(future, continuation);
    }

} // namespace operators

} // namespace AsynQt

#endif // ASYNQT_BASE_CONTINUEWITH_H

