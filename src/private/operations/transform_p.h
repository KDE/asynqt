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

template <typename _In, typename _Transformation>
class TransformFutureInterface
    : public QObject
    , public QFutureInterface<
        typename std::result_of<_Transformation(_In)>::type
    > {

public:
    typedef typename std::result_of<_Transformation(_In)>::type result_type;

    TransformFutureInterface(QFuture<_In> future,
                                _Transformation transormation)
        : m_future(future)
        , m_transormation(transormation)
    {
    }

    ~TransformFutureInterface()
    {
    }

    void callFinished();

    QFuture<result_type> start()
    {
        m_futureWatcher.reset(new QFutureWatcher<_In>());

        QObject::connect(m_futureWatcher.get(),
                         &QFutureWatcherBase::finished,
                         [this] () { callFinished(); });
        QObject::connect(m_futureWatcher.get(),
                         &QFutureWatcherBase::canceled,
                         [this] () { callFinished(); });

        m_futureWatcher->setFuture(m_future);

        this->reportStarted();

        // if (m_future.isFinished()) {
        //     qDebug() << "TransformFutureInterface::start() -- Already finished";
        //     this->callFinished();
        // }

        return this->future();
    }

private:
    QFuture<_In> m_future;
    _Transformation m_transormation;
    std::unique_ptr<QFutureWatcher<_In>> m_futureWatcher;
};

template <typename _In, typename _Transformation>
void TransformFutureInterface<_In, _Transformation>::callFinished()
{
    deleteLater();

    if (m_future.isFinished()) {
        const auto result = m_future.result();
        this->reportResult(m_transormation(result));
        this->reportFinished();

    } else {
        this->reportCanceled();

    }

}

} // namespace detail
} // namespace AsynQt

