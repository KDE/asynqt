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

#include <type_traits>

#include "../utils_p.h"

namespace AsynQt {
namespace detail {

template <typename _In, typename _Transformation>
struct TransformFutureInterfaceHelper {
    typedef typename std::result_of<_Transformation&(_In&&)>::type result_type;
};

template <typename _Transformation>
struct TransformFutureInterfaceHelper<void, _Transformation> {
    typedef typename std::result_of<_Transformation&()>::type result_type;
};


template <typename _In, typename _Transformation>
class TransformFutureInterface
    : public QObject
    , public QFutureInterface<
        typename TransformFutureInterfaceHelper<_In, _Transformation>::result_type
    > {

public:
    typedef typename TransformFutureInterfaceHelper<_In, _Transformation>::result_type result_type;
    typedef typename std::is_void<_In>::type   in_type_is_void;
    typedef typename std::is_void<result_type> result_type_is_void;

    TransformFutureInterface(QFuture<_In> future,
                                _Transformation transormation)
        : m_future(future)
        , m_transormation(transormation)
    {
    }

    ~TransformFutureInterface()
    {
    }

    // If _In is void, we are never going to get a result,
    // so, we need to pretend like we got one when the
    // future is successfully finished
    inline
    void setFutureResultOnFinished(
            std::true_type, // _In is void
            std::true_type  // result_type is void
        )
    {
        qDebug() << "value to void";
        // no value, no result to create, but we still
        // want to call the transformation function
        if (!m_future.isCanceled()) {
            m_transormation();
        }
    }

    inline
    void setFutureResultOnFinished(
            std::true_type,  // _In is void
            std::false_type  // result_type is not void
        )
    {
        qDebug() << "void to value";
        if (!m_future.isCanceled()) {
            this->reportResult(m_transormation());
        }
    }

    // Ignore id _In is not void
    template <typename T>
    inline
    void setFutureResultOnFinished(std::false_type, T) {}

    // If _In is not void, then all is as it should be
    inline
    void setFutureResultAt(
            int index,
            std::false_type, // _In is not void
            std::true_type   // result_type is void
        )
    {
        qDebug() << "value to void";
        // nothing to do with the value, but we still
        // want to call the transformation function
        m_transormation(m_future.resultAt(index));
    }

    inline
    void setFutureResultAt(
            int index,
            std::false_type, // _In is not void
            std::false_type  // result_type is not void
        )
    {
        qDebug() << "value to value";
        this->reportResult(m_transormation(m_future.resultAt(index)));
    }

    template <typename T>
    inline
    void setFutureResultAt(int, std::true_type, T) {}


    QFuture<result_type> start()
    {
        m_futureWatcher.reset(new QFutureWatcher<_In>());

        onFinished(m_futureWatcher, [this]() {
            setFutureResultOnFinished(in_type_is_void(), result_type_is_void());
            this->reportFinished();
        });

        onCanceled(m_futureWatcher, [this]() { this->reportCanceled(); });

        onResultReadyAt(m_futureWatcher, [this](int index) {
            setFutureResultAt(index, in_type_is_void(), result_type_is_void());
        });

        m_futureWatcher->setFuture(m_future);

        this->reportStarted();

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

