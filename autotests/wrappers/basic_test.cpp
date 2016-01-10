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

#include "basic_test.h"

#include <wrappers/common.h>

#include <QFuture>
#include <QCoreApplication>

#include <QtTest>

#include "common.h"

namespace wrappers {

BasicFuturesTest::BasicFuturesTest()
{
}

void BasicFuturesTest::testReadyFutures()
{
    {
        auto future = AsynQt::makeReadyFuture(42);

        // Without waiting!
        QVERIFY(future.isFinished());
        QVERIFY(!future.isCanceled());
        QVERIFY(future.isResultReadyAt(0));
        QCOMPARE(future.result(), 42);
        VERIFY_TYPE(future, QFuture<int>);
    }

    {
        auto future = AsynQt::makeReadyFuture();

        // Without waiting!
        QVERIFY(future.isFinished());
        QVERIFY(!future.isCanceled());
        VERIFY_TYPE(future, QFuture<void>);
    }
}

void BasicFuturesTest::testCanceledFutures()
{
    {
        auto future = AsynQt::makeCanceledFuture<QString>();

        // Without waiting!
        QVERIFY(!future.isFinished());
        QVERIFY(future.isCanceled());
        QVERIFY(!future.isResultReadyAt(0));
        VERIFY_TYPE(future, QFuture<QString>);
    }

    {
        auto future = AsynQt::makeCanceledFuture<void>();

        // Without waiting!
        QVERIFY(!future.isFinished());
        QVERIFY(future.isCanceled());
        VERIFY_TYPE(future, QFuture<void>);
    }
}

void BasicFuturesTest::testDelayedFutures()
{
    auto waitFor = 3000; // milliseconds

    {
        auto future = AsynQt::makeDelayedFuture(42, waitFor);

        COMPARE_BETWEEN(future, 42, 2 _seconds, 4 _seconds);
        VERIFY_TYPE(future, QFuture<int>);
    }

    {
        auto future = AsynQt::makeDelayedFuture(waitFor);

        QVERIFY(waitForFuture(future, 2 _seconds, 4 _seconds));
        VERIFY_TYPE(future, QFuture<void>);
    }
}

void BasicFuturesTest::testDelayedFuturesStdChrono()
{
    using namespace std::chrono;

    {
        auto future = AsynQt::makeDelayedFuture(42, seconds(3));

        COMPARE_BETWEEN(future, 42, 2 _seconds, 4 _seconds);
        VERIFY_TYPE(future, QFuture<int>);
    }

    {
        auto future = AsynQt::makeDelayedFuture(milliseconds(3000));

        QVERIFY(waitForFuture(future, 2 _seconds, 4 _seconds));
        VERIFY_TYPE(future, QFuture<void>);
    }
}

// void DBus
//
//     auto future = AsynQt::DBus::asyncCall<QString>(m_dbus, "GetId");
//
//     QVERIFY(waitForFuture(future, 1 _seconds));
//
//     auto result = future.result();
//
//     qDebug() << "DBus returned " << result << " for the id";
//
//     QCOMPARE(result.size(),
//              QString("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx").size());
// }
//
// void DBusExecutionTest::testDBusExecutionWithArgument()
// {
// }

void BasicFuturesTest::initTestCase()
{
}

void BasicFuturesTest::cleanupTestCase()
{
}

} // namespace wrappers

