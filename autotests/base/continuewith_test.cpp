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
        auto future = execHelloKde();
        auto connectedFuture = continueWith(future, execEcho);

        COMPARE_AFTER(connectedFuture, helloKdeMessage, 1 _seconds);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(connectedFuture, QFuture<QByteArray>);
    }

    TEST_CHUNK("Connecting a temporary future to a continuation")
    {
        auto connectedFuture =
            continueWith(execHelloKde(), execEcho);

        COMPARE_AFTER(connectedFuture, helloKdeMessage, 1 _seconds);
        VERIFY_TYPE(connectedFuture, QFuture<QByteArray>);
    }
}

void ContinueWithTest::testContinueWithOperator()
{
    using namespace AsynQt::operators;

    TEST_CHUNK("Connecting a declared future to a continuation")
    {
        auto future = execHelloKde();
        auto connectedFuture = future | execEcho;

        COMPARE_AFTER(connectedFuture, helloKdeMessage, 1 _seconds);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(connectedFuture, QFuture<QByteArray>);
    }

    TEST_CHUNK("Connecting a temporary future to a continuation")
    {
        auto connectedFuture = execHelloKde() | execEcho;

        COMPARE_AFTER(connectedFuture, helloKdeMessage, 1 _seconds);
        VERIFY_TYPE(connectedFuture, QFuture<QByteArray>);
    }
}

void ContinueWithTest::testContinueWithFailures()
{
    using namespace AsynQt::operators;

    TEST_CHUNK("Success connected to a failure")
    {
        auto future = execHelloKde();
        auto connectedFuture = future | fail;

        CANCELED_AFTER(connectedFuture, 1 _seconds);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(connectedFuture, QFuture<QString>);
    }

    TEST_CHUNK("Failure connected to a success")
    {
        auto connectedFuture = fail(QString()) | execEcho;

        CANCELED_AFTER(connectedFuture, 1 _seconds);
        VERIFY_TYPE(connectedFuture, QFuture<QByteArray>);
    }

    TEST_CHUNK("Fail twice just for the good measure")
    {
        auto connectedFuture = fail(QString()) | fail;

        CANCELED_AFTER(connectedFuture, 1 _seconds);
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

