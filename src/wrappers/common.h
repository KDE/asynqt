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

#ifndef ASYNQT_CONS_VALUE_H
#define ASYNQT_CONS_VALUE_H

#include <asynqt_export.h>

#include <QFuture>
#include <QFutureInterface>
#include <QObject>
#include <QTimer>

namespace AsynQt {

namespace detail {

    template <typename _Result>
    class ReadyFutureInterface
        : public QObject
        , public QFutureInterface<_Result> {

    public:
        ReadyFutureInterface(_Result value)
            : m_value(value)
        {
        }

        QFuture<_Result> start()
        {
            auto future = this->future();

            this->reportStarted();
            this->reportResult(m_value);
            this->reportFinished();

            deleteLater();

            return future;
        }

    private:
        _Result m_value;

    };

    template <typename _Result>
    class CanceledFutureInterface
        : public QObject
        , public QFutureInterface<_Result> {

    public:
        CanceledFutureInterface()
        {
        }

        QFuture<_Result> start()
        {
            auto future = this->future();

            this->reportStarted();
            this->reportCanceled();

            deleteLater();

            return future;
        }

    };

    template <typename _Result>
    class DelayedFutureInterface
        : public QObject
        , public QFutureInterface<_Result> {

    public:
       DelayedFutureInterface(_Result value, int milliseconds)
            : m_value(value)
            , m_milliseconds(milliseconds)
        {
        }

        QFuture<_Result> start()
        {
            auto future = this->future();

            this->reportStarted();

            QTimer::singleShot(m_milliseconds, [this] {
                this->reportResult(m_value);
                this->reportFinished();
                deleteLater();
            });

            return future;
        }

    private:
        _Result m_value;
        int m_milliseconds;

    };

} // namespace detail

template <typename _Result>
QFuture<_Result> makeReadyFuture(_Result &&value)
{
    using namespace detail;

    return (new ReadyFutureInterface<_Result>(std::forward<_Result>(value)))
        ->start();
}

ASYNQT_EXPORT
QFuture<void> makeReadyFuture();

template <typename _Result = void>
QFuture<_Result> makeCanceledFuture()
{
    using namespace detail;

    return (new CanceledFutureInterface<_Result>())->start();
}

template <typename _Result>
QFuture<_Result> makeDelayedFuture(_Result &&value, int milliseconds)
{
    using namespace detail;

    return (new DelayedFutureInterface<_Result>(std::forward<_Result>(value),
                                                milliseconds))
        ->start();
}

ASYNQT_EXPORT
QFuture<void> makeDelayedFuture(int milliseconds);

} // namespace AsynQt

#ifndef ASYNQT_DISABLE_STD_CHRONO

#include <chrono>

namespace AsynQt {

template <typename _Result, typename Rep, typename Period>
QFuture<_Result> makeDelayedFuture(_Result &&value,
                                   std::chrono::duration<Rep, Period> duration)
{
    using namespace std::chrono;

    return makeDelayedFuture(std::forward<_Result>(value),
                             duration_cast<milliseconds>(duration).count());
}

template <typename Rep, typename Period>
QFuture<void> makeDelayedFuture(std::chrono::duration<Rep, Period> duration)
{
    using namespace std::chrono;

    return makeDelayedFuture(duration_cast<milliseconds>(duration).count());
}



} // namespace AsynQt


#endif // ASYNQT_DISABLE_STD_CHRONO


#endif // ASYNQT_CONS_VALUE_H

