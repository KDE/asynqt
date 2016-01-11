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

template <typename _Result>
DelayedFutureInterface<typename std::decay<_Result>::type> *
newDelayedFutureInterface(_Result &&result, int milliseconds)
{
    return new DelayedFutureInterface<typename std::decay<_Result>::type>(
        std::forward<_Result>(result), milliseconds);
}

} // namespace detail
} // namespace AsynQt

