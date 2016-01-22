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

//
// W A R N I N G
// -------------
//
// This file is not part of the AsynQt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

namespace AsynQt {

namespace detail {

template <typename _Result>
QFuture<_Result> makeCanceledFuture()
{
    QFutureInterface<_Result> interface;
    auto future = interface.future();

    interface.reportStarted();
    interface.reportCanceled();
    interface.reportFinished();

    return future;
}

#ifndef QT_NO_EXCEPTIONS
template <typename _Result>
QFuture<_Result> makeCanceledFuture(const QException &exception)
{
    QFutureInterface<_Result> interface;
    auto future = interface.future();

    interface.reportStarted();
    interface.reportException(exception);
    interface.reportFinished();

    return future;
}
#endif


} // namespace detail
} // namespace AsynQt

#ifdef ENABLE_EVIL_QFUTURE_HACKS_THAT_SHOULD_BE_IN_QT

class AsynQt_QFuturePrivacyHack_hasException;
template <>
inline bool QFuture<AsynQt_QFuturePrivacyHack_hasException>::isCanceled() const
{
    return d.exceptionStore().hasException();
}

class AsynQt_QFuturePrivacyHack_throwPossibleException;
template <>
inline void QFuture<AsynQt_QFuturePrivacyHack_throwPossibleException>::cancel()
{
    return d.exceptionStore().throwPossibleException();
}

#endif


