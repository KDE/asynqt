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
class FlattenFutureInterface : public QObject
                             , public QFutureInterface<_Result> {
public:

    FlattenFutureInterface(QFuture<QFuture<_Result>> future)
        : m_firstFuture(future)
    {
    }

    ~FlattenFutureInterface()
    {
    }

    void firstCallFinished()
    {
        if (m_firstFuture.isFinished()) {
            m_secondFuture = m_firstFuture.result();

            m_secondFutureWatcher.reset(new QFutureWatcher<_Result>());
            m_secondFutureWatcher->setFuture(m_secondFuture);

            QObject::connect(m_secondFutureWatcher.get(),
                             &QFutureWatcherBase::finished,
                             [this] () { secondCallFinished(); });

            if (m_secondFuture.isFinished()) {
                qDebug() << "FlattenFutureInterface::start() -- Second one already finished";
                this->secondCallFinished();
            }

        } else {
            this->reportCanceled();
        }
    }

    void secondCallFinished()
    {
        if (m_secondFuture.isFinished()) {
            this->reportResult(m_secondFuture.result());
            this->reportFinished();

        } else {
            this->reportCanceled();
        }
    }

    QFuture<_Result> start()
    {
        m_firstFutureWatcher.reset(new QFutureWatcher<QFuture<_Result>>());
        m_firstFutureWatcher->setFuture(m_firstFuture);

        QObject::connect(m_firstFutureWatcher.get(),
                         &QFutureWatcherBase::finished,
                         [this] () { firstCallFinished(); });

        this->reportStarted();

        if (m_firstFuture.isFinished()) {
            qDebug() << "FlattenFutureInterface::start() -- Already finished";
            this->firstCallFinished();
        }

        return this->future();
    }

private:
    QFuture<QFuture<_Result>> m_firstFuture;
    std::unique_ptr<QFutureWatcher<QFuture<_Result>>> m_firstFutureWatcher;

    QFuture<_Result> m_secondFuture;
    std::unique_ptr<QFutureWatcher<_Result>> m_secondFutureWatcher;
};

} // namespace detail
} // namespace AsynQt

