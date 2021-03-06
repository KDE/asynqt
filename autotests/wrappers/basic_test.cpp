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

#include "basic_test.h"

#include <basic/all.h>

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
    TEST_CHUNK("Testing a ready int future")
    {
        auto future = AsynQt::makeReadyFuture(42);

        // Without waiting!
        QVERIFY(future.isFinished());
        QVERIFY(!future.isCanceled());
        QVERIFY(future.isResultReadyAt(0));
        QCOMPARE(future.result(), 42);
        VERIFY_TYPE(future, QFuture<int>);
    }

    TEST_CHUNK("Testing a ready void future")
    {
        auto future = AsynQt::makeReadyFuture();

        // Without waiting!
        QVERIFY(future.isFinished());
        QVERIFY(!future.isCanceled());
        VERIFY_TYPE(future, QFuture<void>);
    }
}

class TestException : public QException
{
public:
    void raise() const { throw *this; }
    TestException *clone() const { return new TestException(*this); }
};

void BasicFuturesTest::testCanceledFutures()
{
    TEST_CHUNK("Testing a canceled QString future")
    {
        auto future = AsynQt::makeCanceledFuture<QString>();

        // Without waiting!
        QVERIFY(future.isCanceled());
        QVERIFY(!future.isResultReadyAt(0));
        VERIFY_TYPE(future, QFuture<QString>);
    }

    TEST_CHUNK("Testing a canceled void future")
    {
        auto future = AsynQt::makeCanceledFuture<void>();

        // Without waiting!
        QVERIFY(future.isCanceled());
        VERIFY_TYPE(future, QFuture<void>);
    }

    TEST_CHUNK("Testing a QString future with an exception")
    {
        auto future = AsynQt::makeCanceledFuture<QString>(TestException());

        // Without waiting!
        evil::hasException(future);
        VERIFY_EXCEPTION_AROUND(future, TestException, 0 _seconds);
        QVERIFY(future.isCanceled());
        VERIFY_TYPE(future, QFuture<QString>);
    }

    TEST_CHUNK("Testing a void future with an exception")
    {
        auto future = AsynQt::makeCanceledFuture<void>(TestException());

        // Without waiting!
        evil::hasException(future);
        VERIFY_EXCEPTION_AROUND(future, TestException, 0 _seconds);
        QVERIFY(future.isCanceled());
        VERIFY_TYPE(future, QFuture<void>);
    }
}

void BasicFuturesTest::testDelayedFutures()
{
    auto delay = 1 _seconds;

    TEST_CHUNK("Testing a delayed int future")
    {
        auto future = AsynQt::makeDelayedFuture(42, delay);

        QVERIFY(!evil::hasException(future));
        COMPARE_FINISHED_AROUND(future, 42, delay);
        VERIFY_TYPE(future, QFuture<int>);
    }

    TEST_CHUNK("Testing a delayed void future")
    {
        auto future = AsynQt::makeDelayedFuture(delay);

        VERIFY_FINISHED_AROUND(future, delay);
        VERIFY_TYPE(future, QFuture<void>);
    }
}

void BasicFuturesTest::testDelayedFuturesStdChrono()
{
    using namespace std::chrono;
    auto delay = 1 _seconds;

    TEST_CHUNK("Testing a delayed int future with std::chrono")
    {
        auto future = AsynQt::makeDelayedFuture(42, seconds(1));

        COMPARE_FINISHED_AROUND(future, 42, delay);
        VERIFY_TYPE(future, QFuture<int>);
    }

    TEST_CHUNK("Testing a delayed void future with std::chrono")
    {
        auto future = AsynQt::makeDelayedFuture(milliseconds(1000));

        VERIFY_FINISHED_AROUND(future, delay);
        VERIFY_TYPE(future, QFuture<void>);
    }
}

void BasicFuturesTest::initTestCase()
{
}

void BasicFuturesTest::cleanupTestCase()
{
}

} // namespace wrappers

