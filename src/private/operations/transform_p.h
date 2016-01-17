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
struct TransformFutureInterfaceHelper {
    typedef typename std::result_of<_Transformation(_In)>::type result_type;
};

template <typename _Transformation>
struct TransformFutureInterfaceHelper<void, _Transformation> {
    typedef typename std::result_of<_Transformation()>::type result_type;
};


template <typename _In, typename _Transformation>
class TransformFutureInterface
    : public QObject
    , public QFutureInterface<
        typename TransformFutureInterfaceHelper<_In, _Transformation>::result_type
    > {

public:
    typedef typename TransformFutureInterfaceHelper<_In, _Transformation>::result_type result_type;

    TransformFutureInterface(QFuture<_In> future,
                                _Transformation transormation)
        : m_future(future)
        , m_transormation(transormation)
    {
    }

    ~TransformFutureInterface()
    {
    }

    inline
    void setFutureResult(
            std::true_type, // _In is void
            std::true_type  // result_type is void
        )
    {
        // no value, no result to create, but we still
        // want to call the transformation function
        m_transormation();
    }

    inline
    void setFutureResult(
            std::false_type, // _In is not void
            std::true_type   // result_type is void
        )
    {
        // nothing to do with the value, but we still
        // want to call the transformation function
        m_transormation(m_future.result());
    }

    inline
    void setFutureResult(
            std::true_type,  // _In is void
            std::false_type  // result_type is not void
        )
    {
        this->reportResult(m_transormation());
    }

    inline
    void setFutureResult(
            std::false_type, // _In is not void
            std::false_type  // result_type is not void
        )
    {
        this->reportResult(m_transormation(m_future.result()));
    }

    void callFinished()
    {
        deleteLater();

        if (m_future.isFinished()) {
            setFutureResult(
                    typename std::is_void<_In>::type(),
                    typename std::is_void<result_type>::type()
                );
            this->reportFinished();

        } else {
            this->reportCanceled();

        }
    }

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
QFuture<
    typename detail::TransformFutureInterface<_In, _Transformation>::result_type
    >
transform_impl(const QFuture<_In> &future, _Transformation &&transormation)
{
    return (new TransformFutureInterface<_In, _Transformation>(
                future, std::forward<_Transformation>(transormation)))
        ->start();
}


namespace operators {

    template <typename _Transformation>
    class TransformationModifier {
    public:
        TransformationModifier(_Transformation transormation)
            : m_transormation(transormation)
        {
        }

        _Transformation m_transormation;
    };

    template <typename _In, typename _Transformation>
    auto operator | (const QFuture<_In> &future,
                     TransformationModifier<_Transformation> &&modifier)
        -> decltype(transform_impl(future, modifier.m_transormation))
    {
        return transform_impl(future, modifier.m_transormation);
    }

} // namespace operators

} // namespace detail
} // namespace AsynQt

