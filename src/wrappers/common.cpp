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

namespace AsynQt {

namespace detail {
    class ReadyVoidFutureInterface
        : public QObject
        , QFutureInterface<void> {

    public:
        ReadyVoidFutureInterface()
        {
        }

        QFuture<void> start()
        {
            auto future = this->future();

            this->reportStarted();
            this->reportFinished();

            deleteLater();

            return future;
        }
    };

    class DelayedVoidFutureInterface
        : public QObject
        , QFutureInterface<void> {

    public:
        DelayedVoidFutureInterface(int milliseconds)
            : m_milliseconds(milliseconds)
        {
        }

        QFuture<void> start()
        {
            auto future = this->future();

            this->reportStarted();

            QTimer::singleShot(m_milliseconds, [this] {
                this->reportFinished();
                deleteLater();
            });

            deleteLater();

            return future;
        }

    private:
        int m_milliseconds;
    };

} // namespace detail

QFuture<void> makeReadyFuture()
{
    using namespace detail;
    return (new ReadyVoidFutureInterface())->start();
}

QFuture<void> makeDelayedFuture(int milliseconds)
{
    using namespace detail;
    return (new DelayedVoidFutureInterface(milliseconds))->start();
}

} // namespace AsynQt

