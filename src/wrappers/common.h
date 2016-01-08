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

#include <QFuture>
#include <QFutureInterface>
#include <QObject>

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

} // namespace detail

template <typename _Result>
QFuture<_Result> makeReadyFuture(_Result &&value)
{
    using namespace detail;

    auto futureInterface =
        new ReadyFutureInterface<_Result>(std::forward<_Result>(value));

    return futureInterface->start();
}

QFuture<void> makeReadyFuture();

} // namespace AsynQt

#endif // ASYNQT_CONS_VALUE_H

