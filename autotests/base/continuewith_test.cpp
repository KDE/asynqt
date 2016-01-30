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

#include "continuewith_test.h"

#include <wrappers/process.h>
#include <operations/continuewith.h>
#include <basic/all.h>

#include <QFuture>
#include <QCoreApplication>
#include <QtTest>

#include "common.h"

using namespace AsynQt;

namespace base {

ContinueWithTest::ContinueWithTest()
{
}

void ContinueWithTest::testContinueWith()
{
    TEST_CHUNK("Connecting a declared future to a continuation")
    {
        auto future = common::execHelloKde();
        auto connectedFuture = continueWith(future, common::execEcho);

        COMPARE_FINISHED_BEFORE(connectedFuture, common::helloKdeMessage(), 1 _seconds);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(connectedFuture, QFuture<QByteArray>);
    }

    TEST_CHUNK("Connecting a temporary future to a continuation")
    {
        auto connectedFuture =
            continueWith(common::execHelloKde(), common::execEcho);

        COMPARE_FINISHED_BEFORE(connectedFuture, common::helloKdeMessage(), 1 _seconds);
        VERIFY_TYPE(connectedFuture, QFuture<QByteArray>);
    }
}

void ContinueWithTest::testContinueWithOperator()
{
    using namespace AsynQt::operators;

    TEST_CHUNK("Connecting a declared future to a continuation")
    {
        auto future = common::execHelloKde();
        auto connectedFuture = future | common::execEcho;

        COMPARE_FINISHED_BEFORE(connectedFuture, common::helloKdeMessage(), 1 _seconds);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(connectedFuture, QFuture<QByteArray>);
    }

    TEST_CHUNK("Connecting a temporary future to a continuation")
    {
        auto connectedFuture = common::execHelloKde() | common::execEcho;

        COMPARE_FINISHED_BEFORE(connectedFuture, common::helloKdeMessage(), 1 _seconds);
        VERIFY_TYPE(connectedFuture, QFuture<QByteArray>);
    }
}

void ContinueWithTest::testContinueWithFailures()
{
    using namespace AsynQt::operators;

    TEST_CHUNK("Success connected to a failure")
    {
        auto future = common::execHelloKde();
        auto connectedFuture = future | common::fail;

        VERIFY_CANCELED_AROUND(connectedFuture, 1 _seconds);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(connectedFuture, QFuture<QString>);
    }

    TEST_CHUNK("Failure connected to a success")
    {
        auto connectedFuture = common::fail(QString()) | common::execEcho;

        VERIFY_CANCELED_AROUND(connectedFuture, 1 _seconds);
        VERIFY_TYPE(connectedFuture, QFuture<QByteArray>);
    }

    TEST_CHUNK("Fail twice just for the good measure")
    {
        auto connectedFuture = common::fail(QString()) | common::fail;

        VERIFY_CANCELED_AROUND(connectedFuture, 1 _seconds);
        VERIFY_TYPE(connectedFuture, QFuture<QString>);
    }

}

void ContinueWithTest::initTestCase()
{
}

void ContinueWithTest::cleanupTestCase()
{
}

} // namespace base

