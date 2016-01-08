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

#ifndef ASYNQT_BASE_TRANSFORM_H
#define ASYNQT_BASE_TRANSFORM_H

#include <QFuture>
#include <QFutureWatcher>

#include <type_traits>
#include <memory>

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
            m_futureWatcher->setFuture(m_future);

            QObject::connect(m_futureWatcher.get(),
                             &QFutureWatcherBase::finished,
                             [this] () { callFinished(); });

            this->reportStarted();

            if (m_future.isFinished()) {
                qDebug() << "TransformFutureInterface::start() -- Already finished";
                this->callFinished();
            }

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
        }

        this->reportFinished();
    }

    // template <typename _Transformation>
    // void TransformFutureInterface<void, _Transformation>::callFinished()
    // {
    //     deleteLater();
    //     this->reportFinished();
    // }
} // namespace detail

template <typename _In, typename _Transformation>
QFuture<
    typename detail::TransformFutureInterface<_In, _Transformation>::result_type
    >
transform(const QFuture<_In> &future, _Transformation &&transormation)
{
    using namespace detail;

    return (new TransformFutureInterface<_In, _Transformation>(
                future, std::forward<_Transformation>(transormation)))
        ->start();
}

} // namespace AsynQt

#endif // ASYNQT_BASE_TRANSFORM_H

